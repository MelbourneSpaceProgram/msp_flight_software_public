#include <src/board/board.h>
#include <src/board/i2c/i2c.h>
#include <src/config/satellite.h>
#include <src/util/msp_exception.h>
#include <src/util/task_utils.h>
#include <ti/drivers/GPIO.h>
#include <ti/drivers/I2C.h>
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Mailbox.h>
#include <xdc/runtime/Log.h>

I2C_Handle I2c::I2c_busses[] = {NULL, NULL, NULL, NULL};
I2C_Params I2c::I2c_params[] = {{I2C_MODE_CALLBACK, NULL, I2C_400kHz, NULL},
                                {I2C_MODE_CALLBACK, NULL, I2C_400kHz, NULL},
                                {I2C_MODE_CALLBACK, NULL, I2C_100kHz, NULL},
                                {I2C_MODE_CALLBACK, NULL, I2C_400kHz, NULL}};

GateMutexPri_Params I2c::mutex_params;
GateMutexPri_Handle I2c::i2c_mutex = NULL;

void I2c::InitBusses() {
    if (!kI2cAvailable) {
        Log_info0("I2C has been disabled in unit_tests.cpp");
        return;
    }

    GateMutexPri_Params_init(&mutex_params);
    i2c_mutex = GateMutexPri_create(&mutex_params, NULL);
    if (i2c_mutex == NULL) {
        throw MspException("Failed to create mutex.", kI2cMutexFail, __FILE__,
                           __LINE__);
    }

    // Ensure the multiplexer is power cycled to clear it out of any error
    // conditions We could also do this by sending a reset command however a
    // power cycle is easier. 1ms exceeds the minimum cycle time of 500ns per
    // datasheet
    Log_info0("I2c mux a power-down");
    GPIO_write(I2C_MUX_nRST, 0);
    TaskUtils::SleepMilli(1);
    Log_info0("I2c mux a power-up");
    GPIO_write(I2C_MUX_nRST, 1);

    for (uint8_t i = 0; i < Board_I2CCOUNT; i++) {
        if (I2c_busses[i] == NULL) {
            I2C_Params_init(&I2c_params[i]);
            I2c_params[i].transferMode = I2C_MODE_CALLBACK;
            I2c_params[i].transferCallbackFxn =
                static_cast<I2C_CallbackFxn>(ManageI2cTimeout);
            I2c_params[i].bitRate = I2C_400kHz;

            I2c_busses[i] = I2C_open(i, &I2c_params[i]);
            if (I2c_busses[i] == NULL) {
                throw MspException(
                    "Failed to open I2C bus, possibly already in-use.",
                    kI2cOpenFail, __FILE__, __LINE__);
            }
        }
    }
}

I2c::I2c(uint8_t index) : index(index) { Open(); }

void I2c::Open() {
    handle = I2c_busses[index];
    i2c_params = I2c_params[index];
    if (handle == NULL) {
        if (kI2cAvailable) {
            Log_error0("Attempting to use an uninitialised I2C bus");
        }
    }
}

I2C_Params I2c::GetI2cParams() const { return i2c_params; }

I2C_Handle I2c::GetHandle() const { return handle; }

bool I2c::PerformTransaction(byte address, byte* read_buffer,
                             uint16_t read_buffer_length, byte* write_buffer,
                             uint16_t write_buffer_length) const {
    if (handle == NULL) {
        if (kI2cAvailable) {
            Log_error0("Attempting to use uninitialised I2C bus");
        }
        return false;
    }

    I2C_Transaction i2c_transaction;

    Mailbox_Params mailbox_params;
    Mailbox_Params_init(&mailbox_params);
    Mailbox_Handle i2c_mailbox =
        Mailbox_create(sizeof(bool), 1, &mailbox_params, NULL);
    if (i2c_mailbox == NULL) {
        throw MspException("Failed to create I2C bus timeout mailbox.",
                           kI2cMailboxFail, __FILE__, __LINE__);
    }

    i2c_transaction.slaveAddress = address;
    i2c_transaction.writeBuf = write_buffer;
    i2c_transaction.writeCount = write_buffer_length;
    i2c_transaction.readBuf = read_buffer;
    i2c_transaction.readCount = read_buffer_length;
    i2c_transaction.arg = i2c_mailbox;

    IArg key = GateMutexPri_enter(i2c_mutex);
    I2C_transfer(handle, &i2c_transaction);

    // Wait for callback to post the status of the I2C or timeout
    bool transfer_outcome = false;
    bool timed_out =
        !Mailbox_pend(i2c_mailbox, &transfer_outcome,
                      TaskUtils::MilliToCycles(kTimeoutMilliSeconds));
    Mailbox_delete(&i2c_mailbox);

    if (kLogI2c) {
        if (write_buffer_length) {
            Log_info3("I2c: %c-W0x%02x length %d", this->GetBusLabel(), address,
                      write_buffer_length);
            for (uint16_t i = 0; i < write_buffer_length; i++) {
                Log_info1("W: 0x%02x", write_buffer[i]);
            }
        }
        if (read_buffer_length) {
            Log_info3("I2c: %c-R0x%02x length %d", this->GetBusLabel(), address,
                      read_buffer_length);
            for (uint16_t i = 0; i < read_buffer_length; i++) {
                Log_info1("R: 0x%02x", read_buffer[i]);
            }
        }
    }

    if (timed_out) {
        I2C_cancel(handle);
        GateMutexPri_leave(i2c_mutex, key);
        return false;
    }

    GateMutexPri_leave(i2c_mutex, key);
    return transfer_outcome;
}

bool I2c::PerformWriteTransaction(byte address, byte* write_buffer,
                                  uint16_t write_buffer_length) const {
    if (handle == NULL) {
        if (kI2cAvailable) {
            Log_error0("Attempting to use uninitialised I2C bus");
        }
        return false;
    }
    return I2c::PerformTransaction(address, NULL, 0, write_buffer,
                                   write_buffer_length);
}

bool I2c::PerformReadTransaction(byte address, byte* read_buffer,
                                 uint16_t read_buffer_length) const {
    if (handle == NULL) {
        if (kI2cAvailable) {
            Log_error0("Attempting to use uninitialised I2C bus");
        }
        return false;
    }
    return I2c::PerformTransaction(address, read_buffer, read_buffer_length,
                                   NULL, 0);
}

void I2c::ManageI2cTimeout(I2C_Handle handle, I2C_Transaction* i2c_transaction,
                           bool success) {
    if (handle == NULL) {
        if (kI2cAvailable) {
            Log_error0("Attempting to use uninitialised I2C bus");
        }
        return;
    }
    if (i2c_transaction == NULL) {
        if (kI2cAvailable) {
            Log_error0("I2c transaction is NULL");
        }
        return;
    }
    // Check to see whether the I2C_Transaction struct has been given a
    // mailbox handle. If it has put the outcome of the transfer in the mail.
    if (i2c_transaction->arg != NULL) {
        Mailbox_Handle mailbox_handle =
            static_cast<Mailbox_Handle>(i2c_transaction->arg);
        bool transfer_outcome = success;
        if (mailbox_handle != NULL) {
            Mailbox_post(mailbox_handle, &transfer_outcome, 0);
        } else {
            Log_warning0("Mailbox handle couldn't be casted");
        }
    }
}

char I2c::GetBusLabel() const {
    // 65 is ASCII 'A' and Bus A == Index 0, so we are converting a numerical
    // index into a letter
    return index + 65;
}

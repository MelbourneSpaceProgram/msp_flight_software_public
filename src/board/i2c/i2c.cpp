#include <external/etl/exception.h>
#include <src/board/board.h>
#include <src/board/i2c/i2c.h>
#include <src/config/unit_tests.h>
#include <src/util/task_utils.h>
#include <ti/drivers/GPIO.h>
#include <ti/drivers/I2C.h>
#include <ti/sysbios/knl/Mailbox.h>
#include <xdc/runtime/Log.h>

I2C_Handle I2c::I2c_busses[] = {NULL, NULL, NULL, NULL};
I2C_Params I2c::I2c_params[] = {{I2C_MODE_CALLBACK, NULL, I2C_400kHz, NULL},
                                {I2C_MODE_CALLBACK, NULL, I2C_400kHz, NULL},
                                {I2C_MODE_CALLBACK, NULL, I2C_100kHz, NULL},
                                {I2C_MODE_CALLBACK, NULL, I2C_400kHz, NULL}};

void I2c::InitBusses() {
    if (!i2c_enabled) {
        Log_info0("I2C has been disabled in unit_tests.cpp");
        return;
    }

    // Ensure the multiplexer is power cycled to clear it out of any error
    // conditions We could also do this by sending a reset command however a
    // power cycle is easier. 1ms exceeds the minimum cycle time of 500ns per
    // datasheet
    GPIO_write(I2C_MUX_nRST, 0);
    TaskUtils::SleepMilli(1);
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
                throw etl::exception(
                    "Failed to open I2C bus, possibly already in-use.",
                    __FILE__, __LINE__);
            }
        }
    }
}

I2c::I2c(uint8_t index) : index(index) { Open(); }

void I2c::Open() {
    handle = I2c_busses[index];
    i2c_params = I2c_params[index];
    if (handle == NULL) {
        if (i2c_enabled) {
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
        if (i2c_enabled) {
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
        throw etl::exception("Failed to create I2C bus timeout mailbox.",
                             __FILE__, __LINE__);
    }

    i2c_transaction.slaveAddress = address;
    i2c_transaction.writeBuf = write_buffer;
    i2c_transaction.writeCount = write_buffer_length;
    i2c_transaction.readBuf = read_buffer;
    i2c_transaction.readCount = read_buffer_length;
    i2c_transaction.arg = i2c_mailbox;

    I2C_transfer(handle, &i2c_transaction);

    // Wait for callback to post the status of the I2C or timeout
    bool transfer_outcome = false;
    Mailbox_pend(i2c_mailbox, &transfer_outcome,
                 TaskUtils::MilliToCycles(kTimeoutMilliSeconds));

    Mailbox_delete(&i2c_mailbox);

    if (transfer_outcome == false) {
        I2C_cancel(handle);
        return false;
    }

    return true;
}

bool I2c::PerformWriteTransaction(byte address, byte* write_buffer,
                                  uint16_t write_buffer_length) const {
    if (handle == NULL) {
        if (i2c_enabled) {
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
        if (i2c_enabled) {
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
        if (i2c_enabled) {
            Log_error0("Attempting to use uninitialised I2C bus");
        }
        return;
    }
    if (i2c_transaction == NULL) {
        if (i2c_enabled) {
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
        Mailbox_post(mailbox_handle, &transfer_outcome, 0);
    }
}

char I2c::GetBusLabel() const {
    // 65 is ASCII 'A' and Bus A == Index 0, so we are converting a numerical
    // index into a letter
    return index + 65;
}

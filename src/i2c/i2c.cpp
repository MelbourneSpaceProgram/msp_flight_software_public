#include <MSP_EXP432P401R.h>
#include <external/etl/exception.h>
#include <src/config/unit_tests.h>
#include <src/i2c/i2c.h>
#include <src/util/task_utils.h>
#include <ti/drivers/GPIO.h>
#include <ti/drivers/I2C.h>
#include <ti/sysbios/knl/Mailbox.h>
#include <xdc/runtime/Log.h>

etl::array<I2C_Handle, 4> I2c::I2c_busses = {NULL, NULL, NULL, NULL};
etl::array<I2C_Params, 4> I2c::I2c_params = {
    {{I2C_MODE_CALLBACK, NULL, I2C_400kHz, NULL},
     {I2C_MODE_CALLBACK, NULL, I2C_400kHz, NULL},
     {I2C_MODE_CALLBACK, NULL, I2C_100kHz, NULL},
     {I2C_MODE_CALLBACK, NULL, I2C_400kHz, NULL}}};

void I2c::InitBusses() {
    etl::array<uint8_t, 4> scl = {I2C_BUS_A_SCL, I2C_BUS_B_SCL, I2C_BUS_C_SCL,
                                  I2C_BUS_D_SCL};
    etl::array<uint8_t, 4> sda = {I2C_BUS_A_SDA, I2C_BUS_B_SDA, I2C_BUS_C_SDA,
                                  I2C_BUS_D_SDA};

    if (!i2c_enabled) {
        return;
    }
    for (uint8_t i = 0; i < Board_I2CCOUNT; i++) {
        if (I2c_busses.at(i) == NULL) {
            int scl_pulled_up = GPIO_read(scl.at(i));
            int sda_pulled_up = GPIO_read(sda.at(i));

            if (!(scl_pulled_up && sda_pulled_up)) {
                // Missing bus pull up resistors or bus is disconnected.
                // Attempting to use the I2C bus will result in the CPU
                // locking up as the transaction timeout functionality only
                // works if the bus exists.
                Log_error1("Failed to sense pull-up resistors for I2C bus %d",
                           i);
                continue;
            }

            I2C_Params_init(&I2c_params.at(i));
            I2c_params.at(i).transferMode = I2C_MODE_CALLBACK;
            I2c_params.at(i).transferCallbackFxn =
                static_cast<I2C_CallbackFxn>(ManageI2cTimeout);

            I2c_busses.insert_at(i, I2C_open(i, &I2c_params.at(i)));
            if (I2c_busses.at(i) == NULL) {
                throw etl::exception(
                    "Failed to open I2C bus, possibly already in-use.",
                    __FILE__, __LINE__);
            }
        }
    }
}

I2c::I2c(uint8_t index) : index(index) { Open(); }

void I2c::Open() {
    handle = I2c_busses.at(index);
    i2c_params = I2c_params.at(index);
    if (handle == NULL) {
        Log_error0("Attempting to use an uninitialised I2C bus");
    }
}

I2C_Params I2c::GetI2cParams() const { return i2c_params; }

I2C_Handle I2c::GetHandle() const { return handle; }

bool I2c::PerformTransaction(byte address, byte* read_buffer,
                             uint16_t read_buffer_length, byte* write_buffer,
                             uint16_t write_buffer_length) const {
    if (handle == NULL) {
        Log_error0("Attempting to use uninitialised I2C bus");
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
        Log_error0("Attempting to use uninitialised I2C bus");
        return false;
    }
    return I2c::PerformTransaction(address, NULL, 0, write_buffer,
                                   write_buffer_length);
}

bool I2c::PerformReadTransaction(byte address, byte* read_buffer,
                                 uint16_t read_buffer_length) const {
    if (handle == NULL) {
        Log_error0("Attempting to use uninitialised I2C bus");
        return false;
    }
    return I2c::PerformTransaction(address, read_buffer, read_buffer_length,
                                   NULL, 0);
}

void I2c::ManageI2cTimeout(I2C_Handle handle, I2C_Transaction* i2c_transaction,
                           bool success) {
    if (handle == NULL) {
        Log_error0("Attempting to use uninitialised I2C bus");
        return;
    }
    if (i2c_transaction == NULL) {
        Log_error0("I2c transaction is NULL");
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

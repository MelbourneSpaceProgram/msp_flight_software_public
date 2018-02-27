#include <src/i2c/i2c.h>
#include <ti/drivers/I2C.h>
#include <ti/sysbios/knl/Mailbox.h>
#include <ti/sysbios/knl/Clock.h>

I2c::I2c(uint8_t index) : index(index) {
    I2C_Params_init(&i2c_params);
    i2c_params.transferMode = I2C_MODE_CALLBACK;
    i2c_params.transferCallbackFxn =
            static_cast<I2C_CallbackFxn>(MangeI2cTimeout);
    Open();
}

I2c::I2c(I2C_BitRate bit_rate, uint8_t index) : index(index) {
    I2C_Params_init(&i2c_params);
    i2c_params.bitRate = bit_rate;
    i2c_params.transferMode = I2C_MODE_CALLBACK;
    i2c_params.transferCallbackFxn =
            static_cast<I2C_CallbackFxn>(MangeI2cTimeout);
    Open();
}

I2c::~I2c() { Close(); }

void I2c::Open() {
    handle = I2C_open(index, &i2c_params);
    if (handle == NULL) {
        // TODO(wschuetz) Throw Exception
    }
}

void I2c::Close() {
    if (handle != NULL) {
        I2C_close(handle);
    }
}

I2C_Params I2c::GetI2cParams() const { return i2c_params; }

I2C_Handle I2c::GetHandle() const { return handle; }

bool I2c::PerformTransaction(byte address, byte* read_buffer,
                         uint16_t read_buffer_length, byte* write_buffer,
                         uint16_t write_buffer_length) const {
    I2C_Transaction i2c_transaction;

    Mailbox_Params mailbox_params;
    Mailbox_Params_init(&mailbox_params);
    Mailbox_Handle i2c_mailbox = Mailbox_create(sizeof(bool), 1,
                                                &mailbox_params, NULL);
    if (i2c_mailbox == NULL) {
        // TODO(wschuetz) Throw exception
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
                 kTimeoutMilliSeconds*1000/Clock_tickPeriod);

    if (transfer_outcome == false) {
        I2C_cancel(handle);
        // TODO(wschuetz) Throw exception
        Mailbox_delete(&i2c_mailbox);
        return false;
    }

    Mailbox_delete(&i2c_mailbox);

    return true;
}

bool I2c::PerformWriteTransaction(byte address, byte* write_buffer,
                                   uint16_t write_buffer_length) const {
    return I2c::PerformTransaction(address, NULL, 0, write_buffer,
                                   write_buffer_length);
}

bool I2c::PerformReadTransaction(byte address, byte* read_buffer,
                                  uint16_t read_buffer_length) const {
    return I2c::PerformTransaction(address, read_buffer, read_buffer_length,
                                   NULL, 0);
}

void I2c::MangeI2cTimeout(I2C_Handle handle, I2C_Transaction *i2c_transaction,
                          bool success) {
    // Check to see whether the I2C_Transaction struct has been given a
    // mailbox handle. If it has put the outcome of the transfer in the mail.
    if (i2c_transaction->arg != NULL) {
        Mailbox_Handle mailbox_handle =
                static_cast<Mailbox_Handle>(i2c_transaction->arg);
        bool transfer_outcome = success;
        Mailbox_post(mailbox_handle, &transfer_outcome, 0);
    }
}

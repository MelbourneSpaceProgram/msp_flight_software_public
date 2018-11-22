#include <src/board/spi/spi.h>
#include <src/util/msp_exception.h>
#include <ti/drivers/GPIO.h>
#include <ti/drivers/SPI.h>
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Clock.h>
#include <ti/sysbios/knl/Mailbox.h>
#include <ti/sysbios/knl/Semaphore.h>

Spi &Spi::GetInstance() {
    static Spi instance;
    return instance;
}

Spi::Spi() {
    SPI_Params_init(&spi_params);
    spi_params.mode = SPI_MASTER;
    spi_params.dataSize = 8;
    spi_params.transferMode = SPI_MODE_CALLBACK;
    spi_params.transferCallbackFxn =
        static_cast<SPI_CallbackFxn>(ManageSpiTimeout);

    Semaphore_Params semaphore_params;
    Semaphore_Params_init(&semaphore_params);
    transaction_complete = Semaphore_create(1, &semaphore_params, NULL);
    if (transaction_complete == NULL) {
        throw MspException("Failed to create Semaphore.", kSpiSemaphoreFail,
                           __FILE__, __LINE__);
    }

    Open();
}

void Spi::Open() {
    handle = SPI_open(kSpiIndex, &spi_params);
    if (handle == NULL) {
        throw MspException("Failed to open Spi", kSpiOpenFail, __FILE__,
                             __LINE__);
    }
}

void Spi::Close() {
    if (handle != NULL) {
        SPI_close(handle);
    }
}

SPI_Handle Spi::GetHandle() const { return handle; }

SPI_Params Spi::GetSpiParams() const { return spi_params; }

bool Spi::PerformTransaction(uint32_t slave_select_index, byte *read_buffer,
                             byte *write_buffer,
                             uint32_t buffer_lengths) const {
    SPI_Transaction spi_transaction;

    Mailbox_Params mailbox_params;
    Mailbox_Params_init(&mailbox_params);
    Mailbox_Handle spi_mailbox =
        Mailbox_create(sizeof(bool), 1, &mailbox_params, NULL);
    if (spi_mailbox == NULL) {
        throw MspException("Failed to create mailbox", kSpiMailboxFail,
                             __FILE__, __LINE__);
    }

    spi_transaction.count = buffer_lengths;
    spi_transaction.txBuf = write_buffer;
    spi_transaction.rxBuf = read_buffer;
    spi_transaction.arg = spi_mailbox;

    Semaphore_pend(transaction_complete, BIOS_WAIT_FOREVER);
    GPIO_write(slave_select_index, 1);
    GPIO_write(slave_select_index, 0);
    SPI_transfer(handle, &spi_transaction);

    bool transfer_outcome = false;
    Mailbox_pend(spi_mailbox, &transfer_outcome,
                 kTimeoutMilliSeconds * 1000 / Clock_tickPeriod);

    GPIO_write(slave_select_index, 1);
    Semaphore_post(transaction_complete);

    Mailbox_delete(&spi_mailbox);

    if (transfer_outcome == false) {
        SPI_transferCancel(handle);
        return false;
    }

    return true;
}

bool Spi::PerformReadTransaction(uint32_t slave_select_index, byte *read_buffer,
                                 uint32_t read_buffer_length) const {
    return PerformTransaction(slave_select_index, read_buffer, NULL,
                              read_buffer_length);
}

bool Spi::PerformWriteTransaction(uint32_t slave_select_index,
                                  byte *write_buffer,
                                  uint32_t write_buffer_length) const {
    return PerformTransaction(slave_select_index, NULL, write_buffer,
                              write_buffer_length);
}

void Spi::ManageSpiTimeout(SPI_Handle handle,
                           SPI_Transaction *spi_transaction) {
    if (spi_transaction->arg != NULL) {
        Mailbox_Handle mailbox_handle =
            static_cast<Mailbox_Handle>(spi_transaction->arg);
        bool transfer_outcome = true;
        Mailbox_post(mailbox_handle, &transfer_outcome, 0);
    }
}

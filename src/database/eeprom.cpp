#include <external/etl/exception.h>
#include <src/database/eeprom.h>
#include <src/database/hamming_coder.h>
#include <src/spi/spi.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Semaphore.h>
#include <ti/sysbios/BIOS.h>

Semaphore_Handle Eeprom::eeprom_in_use = NULL;

void Eeprom::Init() {
    Semaphore_Params semaphore_params;
    Semaphore_Params_init(&semaphore_params);
    eeprom_in_use = Semaphore_create(1, &semaphore_params, NULL);
    if (eeprom_in_use == NULL) {
        etl::exception e("Failed to create Semaphore", __FILE__, __LINE__);
        throw e;
    }
}

bool Eeprom::ReadStatusRegister(byte *status_register) {
    Semaphore_pend(eeprom_in_use, BIOS_WAIT_FOREVER);

    byte read_buffer[2] = {0}, write_buffer[2] = {0};
    write_buffer[0] = (byte)kEepromReadStatus;

    bool status;
    status = Spi::GetInstance()->PerformTransaction(slave_select_index, read_buffer, write_buffer, 2);
    Task_sleep(5);

    *status_register = read_buffer[1];

    Semaphore_post(eeprom_in_use);

    return status;
}

bool Eeprom::WriteStatusRegister(byte status_register) {
    Semaphore_pend(eeprom_in_use, BIOS_WAIT_FOREVER);

    bool status;
    status = WriteEnable();

    byte write_buffer[2] = {0};
    write_buffer[0] = (byte)kEepromWriteStatus;
    write_buffer[1] = status_register;

    status = status && Spi::GetInstance()->PerformWriteTransaction(slave_select_index, write_buffer, 2);
    Task_sleep(5);

    status = status && WriteDisable();

    Semaphore_post(eeprom_in_use);

    return status;
}

bool Eeprom::ReadData(uint16_t address, byte *read_buffer, uint16_t read_buffer_length, bool *valid_buffer, uint16_t valid_buffer_length) {
    Semaphore_pend(eeprom_in_use, BIOS_WAIT_FOREVER);

    uint16_t real_address = address * 2, real_read_buffer_length = read_buffer_length * 2;
    if (((uint16_t)(real_address + real_read_buffer_length) < real_address) || (real_address < address) || (real_read_buffer_length < read_buffer_length) || (read_buffer_length != valid_buffer_length)) {
        etl::exception e("Buffer overflow. Bad address", __FILE__, __LINE__);
        Semaphore_post(eeprom_in_use);
        throw e;
    }

    bool status = true;
    byte write_buffer[256] = {0}, read_buffer_2[256] = {0};
    uint32_t transaction_length;
    write_buffer[0] = (byte)kEepromReadData;

    for (uint32_t i = 0; i < real_read_buffer_length; i += 128) {
        write_buffer[1] = (byte)((real_address + i) >> 8);
        write_buffer[2] = (byte)(real_address + i);

        if (real_read_buffer_length - i > 128) {
            transaction_length = 128;
        } else {
            transaction_length = real_read_buffer_length - i;
        }

        status = status && Spi::GetInstance()->PerformTransaction(
                               slave_select_index, read_buffer_2, write_buffer,
                               transaction_length + 3);
        Task_sleep(5);

        HammingCoder::DecodeByteArray(
            &read_buffer[i / 2], transaction_length / 2, &valid_buffer[i / 2],
            transaction_length / 2, &read_buffer_2[3], transaction_length);
    }
    Semaphore_post(eeprom_in_use);

    return status;
}

bool Eeprom::WriteData(uint16_t address, byte *write_buffer, uint16_t write_buffer_length) {
    Semaphore_pend(eeprom_in_use, BIOS_WAIT_FOREVER);

    uint16_t real_address = address * 2, real_write_buffer_length = write_buffer_length * 2;
    if (((uint16_t)(real_address + real_write_buffer_length) < real_address) || (real_address < address) || (real_write_buffer_length < write_buffer_length)) {
        etl::exception e("Buffer overflow. Bad address", __FILE__, __LINE__);
        Semaphore_post(eeprom_in_use);
        throw e;
    }

    bool status = true;
    byte write_buffer_2[256] = {0};
    write_buffer_2[0] = (byte)kEepromWriteData;
    uint32_t transaction_length;

    for (uint32_t i = 0; i < real_write_buffer_length; i += 128) {
        write_buffer_2[1] = (byte)((real_address + i) >> 8);
        write_buffer_2[2] = (byte)(real_address + i);

        if (real_write_buffer_length - i > 128) {
            transaction_length = 128;
        } else {
            transaction_length = real_write_buffer_length - i;
        }

        HammingCoder::EncodeByteArray(&write_buffer_2[3], transaction_length, &write_buffer[i / 2], transaction_length / 2);

        status = status && WriteEnable();

        status = status && Spi::GetInstance()->PerformWriteTransaction(slave_select_index, write_buffer_2, transaction_length + 3);
        Task_sleep(5);
    }
    status = status && WriteDisable();

    Semaphore_post(eeprom_in_use);

    return status;
}

bool Eeprom::WriteEnable() {
    byte write_buffer[1];
    write_buffer[0] = (byte)kEepromWriteEnable;
    
    bool status;
    status = Spi::GetInstance()->PerformWriteTransaction(slave_select_index, write_buffer, 1);
    Task_sleep(5);

    return status;
}

bool Eeprom::WriteDisable() {
    byte write_buffer[1];
    write_buffer[0] = (byte)kEepromWriteDisable;

    bool status;
    status = Spi::GetInstance()->PerformWriteTransaction(slave_select_index, write_buffer, 1);
    Task_sleep(5);

    return status;
}


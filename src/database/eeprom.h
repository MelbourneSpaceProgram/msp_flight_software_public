#ifndef SRC_DATABASE_EEPROM_H_
#define SRC_DATABASE_EEPROM_H_

#include <src/board/board.h>
#include <src/util/data_types.h>
#include <ti/sysbios/knl/Semaphore.h>

enum EepromInstructions {
    kEepromWriteStatus = 0b001,
    kEepromWriteData = 0b010,
    kEepromReadData = 0b011,
    kEepromWriteDisable = 0b100,
    kEepromReadStatus = 0b101,
    kEepromWriteEnable = 0b110
};

class Eeprom {
   public:
    static void Init();
    static bool ReadStatusRegister(byte *status_register);
    static bool WriteStatusRegister(byte status_register);
    static bool ReadData(uint16_t address, byte *read_buffer,
                         uint16_t read_buffer_length, bool *valid_buffer,
                         uint16_t valid_buffer_length);
    static bool WriteData(uint16_t address, byte *write_buffer,
                          uint16_t write_buffer_length);

   private:
    static const uint32_t slave_select_index = SYS_nCS4;
    static Semaphore_Handle eeprom_in_use;

    static bool WriteEnable();
    static bool WriteDisable();
};

#endif  // SRC_DATABASE_EEPROM_H_

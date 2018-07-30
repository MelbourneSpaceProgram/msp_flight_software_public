#ifndef SRC_TELECOMMS_LITHIUM_COMMANDS_FAST_PA_LEVEL_H_
#define SRC_TELECOMMS_LITHIUM_COMMANDS_FAST_PA_LEVEL_H_

#include <src/messages/message.h>

class FastPaLevel : public Message {
   public:
    FastPaLevel(uint8_t pa_level);
    SerialisedMessage SerialiseTo(byte* serial_buffer) const;
    uint16_t GetSerialisedSize() const;

   private:
    uint8_t pa_level;
};

#endif  // SRC_TELECOMMS_LITHIUM_COMMANDS_FAST_PA_LEVEL_H_

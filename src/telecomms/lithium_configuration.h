#ifndef SRC_TELECOMMS_LITHIUM_CONFIGURATION_H_
#define SRC_TELECOMMS_LITHIUM_CONFIGURATION_H_

#include <src/messages/message.h>
#include <src/uart/uart_configuration.h>

class LithiumConfiguration : public Message {
   public:
    LithiumConfiguration();
    SerialisedMessage SerialiseTo(byte *serial_buffer);
    uint16_t GetSerialisedSize() const;
    uint32_t GetBaud() const;

   private:
    uint32_t baud_rate;
};

#endif  // SRC_TELECOMMS_LITHIUM_CONFIGURATION_H_

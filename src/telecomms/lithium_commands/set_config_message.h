#ifndef SRC_TELECOMMS_LITHIUM_COMMANDS_SET_CONFIG_MESSAGE_H_
#define SRC_TELECOMMS_LITHIUM_COMMANDS_SET_CONFIG_MESSAGE_H_

#include <src/messages/message.h>
#include <src/telecomms/lithium_configuration.h>

class SetConfigurationMessage : public Message {
   public:
    SetConfigurationMessage(LithiumConfiguration lithium_config);
    SerialisedMessage SerialiseTo(byte* serial_buffer) const;
    uint16_t GetSerialisedSize() const;

   private:
    LithiumConfiguration lithium_config;
    static constexpr uint16_t kLithiumConfigStructSize = 34;
};


#endif  //  SRC_TELECOMMS_LITHIUM_COMMANDS_SET_CONFIG_MESSAGE_H_

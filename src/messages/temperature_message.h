#ifndef SRC_MESSAGES_TEMPERATURE_MESSAGE_H_
#define SRC_MESSAGES_TEMPERATURE_MESSAGE_H_

#include <src/messages/message.h>
#include <src/messages/rebuildable_message.h>
#include <stdint.h>

class TemperatureMessage : public RebuildableMessage {
   public:
    SerialisedMessage SerialiseTo(byte *serial_buffer);
    uint16_t GetSerialisedSize() const;
    TemperatureMessage(float, uint8_t, uint8_t);
    explicit TemperatureMessage(byte *serialised_message_buffer);
    float temperature;
    uint8_t timestamp;
    uint8_t sensor_id;

   private:
    void SerialiseFrom(byte *serial_buffer);
    static const uint16_t kSerialisedSize = 8;
};

#endif  // SRC_MESSAGES_TEMPERATURE_MESSAGE_H_

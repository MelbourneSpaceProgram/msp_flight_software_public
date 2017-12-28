#ifndef SRC_MESSAGES_TEMPERATURE_MESSAGE_H_
#define SRC_MESSAGES_TEMPERATURE_MESSAGE_H_

#include <src/messages/message.h>
#include <stdint.h>

class TemperatureMessage : public Message {
   public:
    SerialisedMessage SerialiseTo(byte *serial_buffer);
    uint16_t GetSerialisedSize() const;
    TemperatureMessage(float, uint8_t, uint8_t);
    float temperature;
    uint8_t timestamp;
    uint8_t sensor_id;

   private:
    static const uint16_t kSerialisedSize = 8;
};

#endif  // SRC_MESSAGES_TEMPERATURE_MESSAGE_H_

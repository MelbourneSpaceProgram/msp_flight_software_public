#ifndef SRC_MESSAGES_TEMPERATURE_MESSAGE_H_
#define SRC_MESSAGES_TEMPERATURE_MESSAGE_H_

#include <stdint.h>
#include "src/messages/message.h"

class TemperatureMessage : public Message {
   public:
    TemperatureMessage(float, uint8_t, uint8_t);
    float temperature;
    uint8_t sensor_id;
    uint8_t timestamp;
    SerialisedMessage Serialise();
};

#endif /* SRC_MESSAGES_TEMPERATURE_MESSAGE_H_ */

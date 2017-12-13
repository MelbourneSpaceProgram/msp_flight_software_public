#ifndef TEMPERATURE_MESSAGE_H_
#define TEMPERATURE_MESSAGE_H_

#include <stdint.h>
#include "message.h"

class TemperatureMessage: public Message {
public:
    TemperatureMessage();
    TemperatureMessage(float, uint8_t, uint8_t);
    float temperature;
    uint8_t sensor_id;
    uint8_t timestamp;
    SerialisedMessage Serialise();
};

#endif /* TEMPERATURE_MESSAGE_H_ */

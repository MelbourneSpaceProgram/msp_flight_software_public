/*
 * TemperatureMessage.h
 *
 *  Created on: 19Nov.,2017
 *      Author: Anthony, Ben
 */

#ifndef TEMPERATUREMESSAGE_H_
#define TEMPERATUREMESSAGE_H_

#include <src/messages/Message.h>
#include <stdint.h>

class TemperatureMessage: public Message {
public:
    TemperatureMessage();
    TemperatureMessage(float, uint8_t, uint8_t);
    float temperature;
    uint8_t sensor_id;
    uint8_t timestamp;
    SerialisedMessage Serialise();
};

#endif /* TEMPERATUREMESSAGE_H_ */

/*
 * TemperatureMessage.h
 *
 *  Created on: 19Nov.,2017
 *      Author: Anthony
 */

#ifndef TEMPERATUREMESSAGE_H_
#define TEMPERATUREMESSAGE_H_

#include <src/CDH/util/MessageQueue.h>

class TemperatureMessage: public Message
{
public:
    TemperatureMessage();
    TemperatureMessage(float, int, int);
    float temperature;
    int sensorId;
    int timestamp;
};

#endif /* TEMPERATUREMESSAGE_H_ */

/*
 * TemperatureMessage.cpp
 *
 *  Created on: 19Nov.,2017
 *      Author: Anthony
 */

#include <src/messages/TemperatureMessage.h>

TemperatureMessage::TemperatureMessage()
{
    // TODO Auto-generated constructor stub

}

TemperatureMessage::TemperatureMessage(float temperature, int timestamp, int sensorId)
{
    this->temperature = temperature;
    this->timestamp = timestamp;
    this->sensorId = sensorId;

}

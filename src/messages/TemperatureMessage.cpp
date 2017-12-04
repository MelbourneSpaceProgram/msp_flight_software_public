/*
 * TemperatureMessage.cpp
 *
 *  Created on: 19Nov.,2017
 *      Author: Anthony
 */

#include <src/CDH/util/SerialisedMessageBuilder.h>
#include <src/messages/TemperatureMessage.h>

TemperatureMessage::TemperatureMessage() {
    // TODO Auto-generated constructor stub

}

TemperatureMessage::TemperatureMessage(float temperature, int timestamp, int sensorId) {
    this->temperature = temperature;
    this->timestamp = timestamp;
    this->sensorId = sensorId;
}

SerialisedMessage TemperatureMessage::serialise() {
    SerialisedMessageBuilder builder(SerialisedMessageBuilder::TEMPERATURE_SENSOR, SerialisedMessageBuilder::V1);
    builder.addData<int>(sensorId).addData<int>(timestamp).addData<float>(temperature);
    return builder.build();
}

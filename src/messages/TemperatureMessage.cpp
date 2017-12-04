/*
 * TemperatureMessage.cpp
 *
 *  Created on: 19Nov.,2017
 *      Author: Anthony
 */

#include <src/messages/TemperatureMessage.h>
#include <src/CDH/util/PackWriter.h>

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

SerialisedMessage TemperatureMessage::serialise(){
    PackWriter writer(PackWriter::TEMPERATURE_SENSOR, PackWriter::V1);
    writer.addData<int>(sensorId).addData<int>(timestamp).addData<float>(temperature);
    return SerialisedMessage(writer.getPackedLength(), writer.getPackedMessageBuffer());
}

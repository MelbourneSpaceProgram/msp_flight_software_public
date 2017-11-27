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
    // Create the new object

    int packed_data_length;

    PackWriter writer(PackWriter::TEMPERATURE_SENSOR, PackWriter::V1);
    writer.addInt(sensorId);
    writer.addInt(timestamp);
    writer.addFloat(temperature);

    packed_data_length = writer.packed_length;

    // Now that we know how big our packing is, encapsulate it
    SerialisedMessage serialMsg(packed_data_length);

    for(int i = 0; i < packed_data_length; i++){
        serialMsg.buffer[i] = writer.packed_message_buffer[i];
    }

    return serialMsg;
}

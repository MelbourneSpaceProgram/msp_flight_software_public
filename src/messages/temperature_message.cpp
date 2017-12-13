#include <src/CDH/util/serialised_message_builder.h>
#include "src/messages/temperature_message.h"

TemperatureMessage::TemperatureMessage() {
    // Auto-generated constructor stub

}

TemperatureMessage::TemperatureMessage(float temperature, uint8_t timestamp, uint8_t sensor_id) {
    this->temperature = temperature;
    this->timestamp = timestamp;
    this->sensor_id = sensor_id;
}

SerialisedMessage TemperatureMessage::Serialise() {
    SerialisedMessageBuilder builder(SerialisedMessageBuilder::kTemperatureSensor, SerialisedMessageBuilder::kV1);
    builder.AddData<uint8_t>(sensor_id).AddData<uint8_t>(timestamp).AddData<float>(temperature);
    return builder.Build();
}

#include <src/CDH/util/SerialisedMessageBuilder.h>
#include <src/messages/TemperatureMessage.h>

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
    builder.AddData<int>(sensor_id);
    builder.AddData<int>(timestamp);
    builder.AddData<float>(temperature);
    return builder.Build();
}

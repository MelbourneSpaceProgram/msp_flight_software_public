#include <src/messages/serialised_message_builder.h>
#include <src/messages/temperature_message.h>
#include <src/util/message_codes.h>

TemperatureMessage::TemperatureMessage(float temperature, uint8_t timestamp,
                                       uint8_t sensor_id)
    : temperature(temperature), timestamp(timestamp), sensor_id(sensor_id) {}

SerialisedMessage TemperatureMessage::SerialiseTo(byte *serial_buffer) {
    SerialisedMessageBuilder builder(serial_buffer, GetSerialisedSize(),
                                     kMockTemperatureSensor, kV1);
    builder.AddData<uint8_t>(sensor_id)
        .AddData<uint8_t>(timestamp)
        .AddData<float>(temperature);
    return builder.Build();
}

uint16_t TemperatureMessage::GetSerialisedSize() const {
    return kSerialisedSize;
}

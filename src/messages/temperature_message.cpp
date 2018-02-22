#include <src/messages/rebuildable_message_field_iterator.h>
#include <src/messages/serialised_message_builder.h>
#include <src/messages/temperature_message.h>
#include <src/util/message_codes.h>

TemperatureMessage::TemperatureMessage(float temperature, uint8_t timestamp,
                                       uint8_t sensor_id)
    : temperature(temperature), timestamp(timestamp), sensor_id(sensor_id) {}

TemperatureMessage::TemperatureMessage(byte *serialised_message_buffer) {
    SerialiseFrom(serialised_message_buffer);
}

SerialisedMessage TemperatureMessage::SerialiseTo(byte *serial_buffer) const {
    SerialisedMessageBuilder builder(serial_buffer, GetSerialisedSize(),
                                     kMockTemperatureSensor, kV1);
    builder.AddData<uint8_t>(sensor_id)
        .AddData<uint8_t>(timestamp)
        .AddData<float>(temperature);
    return builder.Build();
}

void TemperatureMessage::SerialiseFrom(byte *serial_buffer) {
    RebuildableMessageFieldIterator builder(serial_buffer, GetSerialisedSize());

    builder.FetchData<float>(&temperature);
    builder.FetchData<uint8_t>(&timestamp);
    builder.FetchData<uint8_t>(&sensor_id);
}

uint16_t TemperatureMessage::GetSerialisedSize() const {
    return kSerialisedSize;
}

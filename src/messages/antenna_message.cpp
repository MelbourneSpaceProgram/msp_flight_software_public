#include <src/messages/antenna_message.h>
#include <src/messages/serialised_message_builder.h>
#include <src/util/data_types.h>
#include <src/util/message_codes.h>

AntennaMessage::AntennaMessage(byte i2c_raw[3])
    : door_one_open(kDoorOneMask & i2c_raw[0]),
      door_two_open(kDoorTwoMask & i2c_raw[0]),
      door_three_open(kDoorThreeMask & i2c_raw[0]),
      door_four_open(kDoorFourMask & i2c_raw[0]),
      heaters_on(i2c_raw[1] > 0),
      antenna_state(kStateMask & i2c_raw[0]),
      active_heater(i2c_raw[1]),
      seconds_elapsed(i2c_raw[2]) {
    raw = memcpy(raw, i2c_raw, 3);
}

AntennaMessage::AntennaMessage(bool door_one_open, bool door_two_open,
                               bool door_three_open, bool door_four_open,
                               bool heaters_on, byte antenna_state,
                               byte active_heaters, byte seconds_elapsed)
    : door_one_open(door_one_open),
      door_two_open(door_two_open),
      door_three_open(door_three_open),
      door_four_open(door_four_open),
      heaters_on(heaters_on),
      antenna_state(antenna_state),
      active_heaters(active_heaters),
      seconds_elapsed(seconds_elapsed) {}

SerialisedMessage AntennaMessage::SerialiseTo(byte *serial_buffer) const {
    SerialisedMessageBuilder builder(serial_buffer, kSerialisedSize);

    builder.AddData<byte>(raw[1])
        .AddData<byte>(raw[2])
        .AddData<byte>(raw[3]);

    return builder.Build();
}

uint16_t AntennaMessage::GetSerialisedSize() const { return kSerialisedSize; }

byte AntennaMessage::GetAntennaState() const { return antenna_state; }

byte AntennaMessage::GetActiveHeaters() const { return active_heaters; }

byte AntennaMessage::GetAlgorithmElaspedTime() const { return seconds_elapsed; }

bool AntennaMessage::IsDoorOneOpen() const { return door_one_open; }

bool AntennaMessage::IsDoorTwoOpen() const { return door_two_open; }

bool AntennaMessage::IsDoorThreeOpen() const { return door_three_open; }

bool AntennaMessage::IsDoorFourOpen() const { return door_four_open; }

bool AntennaMessage::IsHeatersOn() const { return heaters_on; }

#include <src/messages/antenna_message.h>
#include <src/messages/serialised_message_builder.h>
#include <src/util/data_types.h>
#include <src/util/message_codes.h>

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

// TODO(dingbenjamin) This function is wasteful in terms of memory, the direct
// I2C reading from the antenna can be sent but this way is more clear and
// easier to debug. Still, consider implementing a more efficient version
SerialisedMessage AntennaMessage::SerialiseTo(byte *serial_buffer) const {
    SerialisedMessageBuilder builder(serial_buffer, kSerialisedSize, kAntenna,
                                     kV1);
    // bool is not well defined in the C++ standard, but the conversion from
    // bool to int is well defined as true -> 1 and false -> 0
    // Therefore to be platform independent convert the bool into an int first

    int int_door_one = door_one_open;
    int int_door_two = door_two_open;
    int int_door_three = door_three_open;
    int int_door_four = door_four_open;
    int int_heaters_on = heaters_on;

    // Typecast to uint8_t (int to uint8_t is well defined)

    uint8_t uint_door_one = int_door_one;
    uint8_t uint_door_two = int_door_two;
    uint8_t uint_door_three = int_door_three;
    uint8_t uint_door_four = int_door_four;
    uint8_t uint_heaters_on = int_heaters_on;

    builder.AddData<uint8_t>(uint_door_one)
        .AddData<uint8_t>(uint_door_two)
        .AddData<uint8_t>(uint_door_three)
        .AddData<uint8_t>(uint_door_four)
        .AddData<uint8_t>(uint_heaters_on)
        .AddData<byte>(antenna_state)
        .AddData<byte>(active_heaters)
        .AddData<byte>(seconds_elapsed);

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

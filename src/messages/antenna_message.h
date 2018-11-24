#ifndef SRC_MESSAGES_ANTENNA_MESSAGE_H_
#define SRC_MESSAGES_ANTENNA_MESSAGE_H_

#include <src/messages/message.h>
#include <src/messages/serialised_message.h>
#include <src/util/data_types.h>

class AntennaMessage : public Message {
   public:
	explicit AntennaMessage(byte i2c_raw[3]);
    SerialisedMessage SerialiseTo(byte *serial_buffer) const;
    uint16_t GetSerialisedSize() const;

    byte GetAntennaState() const;
    byte GetActiveHeaters() const;
    byte GetAlgorithmElaspedTime() const;
    bool IsDoorOneOpen() const;
    bool IsDoorTwoOpen() const;
    bool IsDoorThreeOpen() const;
    bool IsDoorFourOpen() const;
    bool IsHeatersOn() const;

   private:
    AntennaMessage(bool door_one_open, bool door_two_open, bool door_three_open,
                   bool door_four_open, bool heaters_on, byte antenna_state,
                   byte active_heaters, byte seconds_elapsed);
    static constexpr uint16_t kSerialisedSize = 3;

    bool door_one_open;
    bool door_two_open;
    bool door_three_open;
    bool door_four_open;
    bool heaters_on;
    byte antenna_state;
    byte active_heaters;
    byte seconds_elapsed;
	byte raw[3];
};

#endif  // SRC_MESSAGES_ANTENNA_MESSAGE_H_

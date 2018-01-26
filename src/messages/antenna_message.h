#ifndef SRC_MESSAGES_ANTENNA_MESSAGE_H_
#define SRC_MESSAGES_ANTENNA_MESSAGE_H_

#include <src/messages/message.h>
#include <src/messages/serialised_message.h>
#include <src/util/data_types.h>

class AntennaMessage : public Message {
   public:
    AntennaMessage(bool valid_status, bool door_one_open, bool door_two_open,
                   bool door_three_open, bool door_four_open,
                   byte antenna_state);
    SerialisedMessage SerialiseTo(byte *serial_buffer);
    uint16_t GetSerialisedSize() const;

    byte GetAntennaState() const;
    bool IsDoorOneOpen() const;
    bool IsDoorTwoOpen() const;
    bool IsDoorThreeOpen() const;
    bool IsDoorFourOpen() const;
    bool IsValidStatus() const;

   private:
    static const uint16_t kSerialisedSize = 8;

    bool valid_status;
    bool door_one_open;
    bool door_two_open;
    bool door_three_open;
    bool door_four_open;
    byte antenna_state;
};

#endif  // SRC_MESSAGES_ANTENNA_MESSAGE_H_

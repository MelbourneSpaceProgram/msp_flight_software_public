#ifndef SRC_MESSAGES_MESSAGE_H_
#define SRC_MESSAGES_MESSAGE_H_

#include <src/messages/serialised_message.h>
#include <src/util/data_types.h>

class Message {
   public:
    virtual ~Message() {}
    virtual SerialisedMessage SerialiseTo(byte *serial_buffer) = 0;
    virtual uint16_t GetSerialisedSize() const = 0;
    SerialisedMessage Serialise();
};

#endif  // SRC_MESSAGES_MESSAGE_H_

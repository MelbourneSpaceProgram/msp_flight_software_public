#ifndef SRC_MESSAGES_REBUILDABLE_MESSAGE_H_
#define SRC_MESSAGES_REBUILDABLE_MESSAGE_H_

#include <src/messages/message.h>
#include <stdint.h>

class RebuildableMessage : public Message {
    // public:
    // explicit RebuildableMessage(byte* serialised_message_buffer) {}

   private:
    virtual void SerialiseFrom(byte* serial_buffer) = 0;
};

#endif  // SRC_MESSAGES_REBUILDABLE_MESSAGE_H_

#ifndef SRC_MESSAGES_MESSAGE_H_
#define SRC_MESSAGES_MESSAGE_H_

#include "src/messages/serialised_message.h"

class Message {
   public:
    Message() {}
    virtual ~Message() {}
    virtual SerialisedMessage Serialise() = 0;
};

#endif /* SRC_MESSAGES_MESSAGE_H_ */

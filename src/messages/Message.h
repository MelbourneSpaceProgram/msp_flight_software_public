#ifndef MESSAGE_H_
#define MESSAGE_H_

#include <src/messages/SerialisedMessage.h>

class Message {
public:
    Message();
    virtual ~Message();
    virtual SerialisedMessage Serialise() = 0;
};

#endif /* MESSAGE_H_ */

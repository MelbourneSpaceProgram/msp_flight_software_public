/*
 * Message.h
 *
 *  Created on: 24Nov.,2017
 *      Author: Anthony
 */

#ifndef MESSAGE_H_
#define MESSAGE_H_

#include <src/messages/SerialisedMessage.h>

class Message {
public:
    Message();
    virtual ~Message();
    virtual SerialisedMessage serialise() = 0;
};

#endif /* MESSAGE_H_ */

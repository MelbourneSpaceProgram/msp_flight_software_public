/*
 * SerialisedMessage.h
 *
 *  Created on: 24Nov.,2017
 *      Author: Anthony
 */

#ifndef SERIALISEDMESSAGE_H_
#define SERIALISEDMESSAGE_H_

class SerialisedMessage {
public:
    char * buffer;
    int size;
    SerialisedMessage(int size);
    SerialisedMessage(int size, char* message);
    virtual ~SerialisedMessage();
};

#endif /* SERIALISEDMESSAGE_H_ */

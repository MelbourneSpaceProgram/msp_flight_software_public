#ifndef SERIALISEDMESSAGE_H_
#define SERIALISEDMESSAGE_H_

#include <stdint.h>

class SerialisedMessage {
public:
    char * buffer;
    uint16_t size;
    SerialisedMessage(uint16_t size);
    SerialisedMessage(uint16_t size, char* message);
    virtual ~SerialisedMessage();
};

#endif /* SERIALISEDMESSAGE_H_ */

#ifndef SRC_MESSAGES_REBUILDABLE_MESSAGE_FIELD_ITERATOR_H_
#define SRC_MESSAGES_REBUILDABLE_MESSAGE_FIELD_ITERATOR_H_

#include <src/messages/message.h>
#include <src/util/data_types.h>
#include <src/util/message_codes.h>

class RebuildableMessageFieldIterator {
   public:
    RebuildableMessageFieldIterator(byte* serial_buffer,
                                    uint16_t serialised_length)
        : serialised_message_buffer(serial_buffer),
          serialised_length(serialised_length) {}
    template <class T>
    void FetchData(T* data) {
        // The ordering of calls must be the reverse of AddData

        if (sizeof(T) > serialised_length) {
            // Attempting to read more bytes than are present in the message
            // TODO(akremor): Error handler
            return;
        }

        serialised_length -= sizeof(T);
        std::memcpy(data, serialised_message_buffer + serialised_length,
                    sizeof(T));
    }

   private:
    byte* serialised_message_buffer;
    uint16_t serialised_length;
};

#endif  // SRC_MESSAGES_REBUILDABLE_MESSAGE_FIELD_ITERATOR_H_

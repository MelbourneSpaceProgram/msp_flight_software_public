#include <src/messages/message.h>

SerialisedMessage Message::Serialise() const {
    // TODO(dingbenjamin): Perform size checks and remove var size array
    byte serial_buffer[GetSerialisedSize()];
    return SerialiseTo(serial_buffer);
}

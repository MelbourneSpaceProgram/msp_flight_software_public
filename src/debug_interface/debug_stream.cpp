#include <Board.h>
#include <src/config/board_definitions.h>
#include <src/debug_interface/debug_stream.h>
#include <src/messages/serialised_message_builder.h>
#include <src/messages/test_message.h>
#include <src/util/data_types.h>
#include <src/util/message_codes.h>

DebugStream *DebugStream::instance = NULL;

DebugStream::DebugStream()
    : debug_uart(UartConfiguration(UartConfiguration::kBaud115200), UARTA0),
      debug_message_queue(Queue_create(NULL, NULL)) {
    char echo_prompt[] = "Debug stream started.\r\n";
    debug_uart.PerformWriteTransaction(reinterpret_cast<byte *>(echo_prompt),
                                       sizeof(echo_prompt));
}

// (dingbenjamin) The purpose of serialising to heap before enqueuing is to
// avoid an otherwise necessary deep copy of the abstract message pointer passed
// in.
// A deep copy would be required to prevent the queue from being corrupted
// if a message passed in were to go out of scope Furthermore, a deep copy
// requires changing the message class such that all derived classes implement a
// clone function
void DebugStream::EnqueueSerialised(Message *message) {
    QueueMessage *queue_message = new QueueMessage();

    // Create Queue element on stack
    Queue_Elem *element = new Queue_Elem();
    queue_message->element = *element;
    queue_message->element_pointer = element;

    // Create SerialisedMessage with buffer on heap and rest on stack, then copy
    // all to heap
    byte *serial_msg_buffer = new byte[message->GetSerialisedSize()];
    SerialisedMessage stack_serialised_message =
        message->SerialiseTo(serial_msg_buffer);
    SerialisedMessage *heap_serialised_message =
        new SerialisedMessage(stack_serialised_message.GetSize(),
                              stack_serialised_message.GetBuffer());
    queue_message->serial_message = heap_serialised_message;

    Queue_put(debug_message_queue, &(queue_message->element));
}

SerialisedMessage *DebugStream::DequeueSerialised() {
    if (!Queue_empty(debug_message_queue)) {
        QueueMessage *queue_message =
            reinterpret_cast<QueueMessage *>(Queue_get(debug_message_queue));
        SerialisedMessage *serial_message = queue_message->serial_message;
        delete queue_message->element_pointer;
        delete queue_message;
        return serial_message;
    } else {
        return NULL;
    }
}

void DebugStream::SendMessage(const SerialisedMessage &serial_msg) const {
    debug_uart.PerformWriteTransaction(serial_msg.GetBuffer(),
                                       serial_msg.GetSize());
}

uint8_t DebugStream::ReceiveCode() const {
    uint8_t read_code[1];
    uint8_t read_code_length = 1;
    debug_uart.PerformReadTransaction(read_code, read_code_length);
    return read_code[0];
}

// GetInstance() is not thread safe on first initialisation
// It is recommended that it be called from a single thread only the first time
DebugStream *DebugStream::GetInstance() {
    if (instance == NULL) {
        instance = new DebugStream();
    }
    return instance;
}

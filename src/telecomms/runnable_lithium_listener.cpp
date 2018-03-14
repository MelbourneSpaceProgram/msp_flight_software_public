#include <src/tasks/runnable.h>
#include <src/telecomms/lithium.h>
#include <src/telecomms/lithium_commands/lithium_command_codes.h>
#include <src/telecomms/lithium_commands/transmit_command.h>
#include <src/telecomms/lithium_utils.h>
#include <src/telecomms/runnable_lithium_listener.h>
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Mailbox.h>

RunnableLithiumListener::RunnableLithiumListener() {}

fnptr RunnableLithiumListener::GetRunnablePointer() {
    return &RunnableLithiumListener::Receive;
}

void RunnableLithiumListener::Receive() {
    byte read_buffer[Lithium::kMaxReceivedSize];
    while (1) {
        // Ensure read buffer is reset
        read_buffer[0] = 0;
        read_buffer[1] = 0;

        // Blocking read that reads headers only
        Lithium::GetInstance()->GetUart()->PerformReadTransaction(
            read_buffer, Lithium::kLithiumHeaderSize);

        if (!LithiumUtils::IsValidHeader(read_buffer)) {
            // TODO(dingbenjamin): Throw exception
            continue;
        }

        uint16_t payload_size = LithiumUtils::GetPayloadSize(read_buffer);

        if (LithiumUtils::GetCommandCode(read_buffer) != kReceivedDataCode) {
            // Post first 8 bytes of read_buffer to writer task
            // TODO(dingbenjamin): Wrap mailbox posting in a function
            Mailbox_Handle response_mailbox_handle =
                Lithium::GetInstance()->GetCommandResponseMailbox();
            Mailbox_post(response_mailbox_handle, read_buffer,
                         BIOS_WAIT_FOREVER);  // TODO(akremor): Maybe don't
                                              // wait forever?
        }

        if (payload_size != 0) {
            // A non-zero payload implies there is a payload to be received

            // Read the payload bytes into the remainder of the buffer
            Lithium::GetInstance()->GetUart()->PerformReadTransaction(
                read_buffer + Lithium::kLithiumHeaderSize,
                payload_size + Lithium::kLithiumTailSize);

            // Post to message handler task
            Mailbox_Handle payload_mailbox_handle =
                Lithium::GetInstance()->GetMessageMailbox();
            Mailbox_post(payload_mailbox_handle, read_buffer,
                         BIOS_WAIT_FOREVER);
        }
    }
}

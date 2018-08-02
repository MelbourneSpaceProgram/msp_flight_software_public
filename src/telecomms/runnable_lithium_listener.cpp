#include <external/etl/exception.h>
#include <src/tasks/runnable.h>
#include <src/telecomms/lithium.h>
#include <src/telecomms/lithium_commands/lithium_command_codes.h>
#include <src/telecomms/lithium_commands/transmit_command.h>
#include <src/telecomms/lithium_utils.h>
#include <src/telecomms/runnable_lithium_listener.h>
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Mailbox.h>
#include <xdc/runtime/Log.h>

RunnableLithiumListener::RunnableLithiumListener() {}

fnptr RunnableLithiumListener::GetRunnablePointer() {
    return &RunnableLithiumListener::Receive;
}

void RunnableLithiumListener::Receive() {
    byte read_buffer[Lithium::kMaxReceivedSize];
    while (1) {
        // Grab sync characters (first two bytes of header/packet) one char at a
        // time Not two at a time so we can 'burn off' additional characters and
        // regain sync
        Lithium::GetInstance()->GetUart()->PerformReadTransaction(read_buffer,
                                                                  1);

        if (read_buffer[0] != Lithium::kSyncCharOne) {
            continue;
        }

        Lithium::GetInstance()->GetUart()->PerformReadTransaction(
            read_buffer + 1, 1);

        if (read_buffer[1] != Lithium::kSyncCharTwo) {
            continue;
        }

        // Blocking read that reads headers only
        Lithium::GetInstance()->GetUart()->PerformReadTransaction(
            read_buffer + Lithium::kLithiumSyncSize,
            Lithium::kLithiumHeaderSize - Lithium::kLithiumSyncSize);

        if (!LithiumUtils::IsValidHeader(read_buffer)) {
            Log_error0("Invalid Lithium header");
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
        } else {
            Lithium::rx_count =
                Lithium::rx_count == 255 ? 0 : Lithium::rx_count + 1;
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

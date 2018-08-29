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

bool RunnableLithiumListener::ReadLithiumUart(byte* read_buffer, uint8_t size) {
    return size == Lithium::GetInstance()->GetUart()->PerformReadTransaction(
        read_buffer, size);
}

void RunnableLithiumListener::Receive() {
    byte read_buffer[Lithium::kMaxReceivedUartSize] = {0};
    while (1) {
        // Grab sync characters (first two bytes of header/packet) one char at a
        // time Not two at a time so we can 'burn off' additional characters and
        // regain sync
        if (!ReadLithiumUart(read_buffer, 1)) continue;

        if (read_buffer[0] != Lithium::kSyncCharOne) {
            continue;
        }

        if (!ReadLithiumUart(read_buffer + 1, 1)) continue;

        if (read_buffer[1] != Lithium::kSyncCharTwo) {
            continue;
        }

        // Blocking read that reads headers only
        if (!ReadLithiumUart(
                read_buffer + Lithium::kLithiumSyncSize,
                Lithium::kLithiumHeaderSize - Lithium::kLithiumSyncSize))
            continue;

        if (!LithiumUtils::IsValidHeader(read_buffer)) {
            Log_error0("Invalid Lithium header");
            continue;
        }

        uint16_t payload_size = LithiumUtils::GetPayloadSize(read_buffer);
        uint8_t command_code = LithiumUtils::GetCommandCode(read_buffer);

        // Post first 8 bytes of read_buffer to header mailbox if not an
        // uplinked packet
        if (command_code != kReceivedDataCode) {
            Mailbox_post(Lithium::GetInstance()->GetHeaderMailbox(),
                         read_buffer, BIOS_WAIT_FOREVER);
        }

        if (payload_size > 0 &&
            payload_size <= Lithium::kMaxReceivedUplinkSize) {
            // TODO(dingbenjamin): Check tail checksum over payload
            if (!ReadLithiumUart(read_buffer + Lithium::kLithiumHeaderSize,
                                 payload_size))
                continue;

            Mailbox_Handle mailbox;
            if (command_code == kReceivedDataCode) {
                if (payload_size > Lithium::kMaxReceivedUplinkSize) {
                    Log_error0("Incoming packet too large, ignored");
                }
                Lithium::rx_count =
                    Lithium::rx_count == 255 ? 0 : Lithium::rx_count + 1;
                mailbox = Lithium::GetInstance()->GetUplinkMailbox();
            } else {
                if (payload_size > Lithium::kMaxReceivedLithiumResponseSize) {
                    Log_error0("Incoming lithium response too large, ignored");
                }
                mailbox = Lithium::GetInstance()->GetCommandResponseMailbox();
            }

            // Post payload/uplink to appropriate mailbox
            Mailbox_post(mailbox, read_buffer + Lithium::kLithiumHeaderSize,
                         BIOS_WAIT_FOREVER);
        } else if (payload_size > Lithium::kMaxReceivedUartSize) {
            Log_error0("Incoming uplink too large, ignored");
        }
    }
}

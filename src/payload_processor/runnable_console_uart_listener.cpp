#include <src/payload_processor/runnable_console_uart_listener.h>
#include <src/payload_processor/runnable_payload_processor.h>
#include <src/system/runnable_system_health_check.h>
#include <src/telecomms/lithium.h>
#include <src/util/message_codes.h>
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Mailbox.h>

RunnableConsoleUartListener::RunnableConsoleUartListener() {}

fnptr RunnableConsoleUartListener::GetRunnablePointer() {
    return &RunnableConsoleUartListener::Listen;
}

bool RunnableConsoleUartListener::ReadUart(byte* read_buffer, uint8_t size) {
    // TODO(dingbenjamin): Find a better place to initialise the debug
    // uart rather than stealing it from the health check
    return size == RunnableSystemHealthCheck::debug_uart.PerformReadTransaction(
                       read_buffer, size);
}

void RunnableConsoleUartListener::Listen() {
    while (1) {
        byte header_buffer[5];
        byte payload_buffer[Lithium::kMaxReceivedUplinkSize -
                            RunnablePayloadProcessor::kAx25Bytes];

        // Grab sync characters (first two bytes of header/packet) one char at
        // a time. Not two at a time so we can 'burn off' additional characters
        // and regain sync
        if (!ReadUart(header_buffer, 1)) continue;

        if (header_buffer[0] !=
            RunnableSystemHealthCheck::kMeasurableLoggerSyncChar1) {
            continue;
        }

        if (!ReadUart(header_buffer + 1, 1)) continue;

        if (header_buffer[1] !=
            RunnableSystemHealthCheck::kMeasurableLoggerSyncChar2) {
            continue;
        }

        if (!ReadUart(header_buffer + 2, 1)) continue;  // size
        if (!ReadUart(header_buffer + 3, 1)) continue;  // id
        // TODO(dingbenjamin): Actually check the checksum
        if (!ReadUart(header_buffer + 4, 1)) continue;  // checksum

        if (header_buffer[3] == 0) continue;
        if (!ReadUart(payload_buffer, header_buffer[2])) continue;

        // Debug ID 0 will signify an injection into the payload processor

        switch (header_buffer[3]) {
            case kPayloadProcessorInjection:
                // Create fake AX.25 bytes for the payload processor to throw
                // away
                byte fake_ax25_payload[sizeof(payload_buffer) +
                                       RunnablePayloadProcessor::kAx25Bytes];
                memset(fake_ax25_payload, 0x00,
                       RunnablePayloadProcessor::kAx25Bytes);
                memcpy(fake_ax25_payload + RunnablePayloadProcessor::kAx25Bytes,
                       payload_buffer,
                       Lithium::kMaxReceivedUplinkSize -
                           RunnablePayloadProcessor::kAx25Bytes);
                Mailbox_post(Lithium::GetInstance()->GetUplinkMailbox(),
                             fake_ax25_payload, BIOS_WAIT_FOREVER);
        }
    }
}

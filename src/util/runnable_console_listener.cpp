#include <external/etl/exception.h>
#include <src/payload_processor/runnable_payload_processor.h>
#include <src/system/runnable_system_health_check.h>
#include <src/telecomms/lithium.h>
#include <src/util/message_codes.h>
#include <src/util/runnable_console_logger.h>
#include <src/util/runnable_console_listener.h>
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Mailbox.h>

Uart* RunnableConsoleListener::debug_uart = NULL;

RunnableConsoleListener::RunnableConsoleListener(Uart* debug_uart) {
    if (RunnableConsoleListener::debug_uart == NULL) {
        // WARNING: This task should only ever READ from the debug UART
        RunnableConsoleListener::debug_uart = debug_uart;
    } else {
        throw etl::exception(
            "Only one instance of RunnableConsoleListener should ever be "
            "instantiated",
            __FILE__, __LINE__);
    }
}

fnptr RunnableConsoleListener::GetRunnablePointer() {
    return &RunnableConsoleListener::Listen;
}

bool RunnableConsoleListener::ReadUart(byte* read_buffer, uint8_t size) {
    return size == debug_uart->PerformReadTransaction(read_buffer, size);
}

void RunnableConsoleListener::Listen() {
    while (1) {
        byte header_buffer[5];
        byte payload_buffer[Lithium::kMaxReceivedUplinkSize -
                            RunnablePayloadProcessor::kUplinkAx25Length];

        // Grab sync characters (first two bytes of header/packet) one char at
        // a time. Not two at a time so we can 'burn off' additional characters
        // and regain sync
        if (!ReadUart(header_buffer, 1)) continue;

        if (header_buffer[0] !=
            RunnableConsoleLogger::kMeasurableLoggerSyncChar1) {
            continue;
        }

        if (!ReadUart(header_buffer + 1, 1)) continue;

        if (header_buffer[1] !=
            RunnableConsoleLogger::kMeasurableLoggerSyncChar2) {
            continue;
        }

        if (!ReadUart(header_buffer + 2, 1)) continue;  // size
        if (header_buffer[2] == 0) continue;

        if (!ReadUart(header_buffer + 3, 1)) continue;  // id

        if (!ReadUart(header_buffer + 4, 1))
            continue;  // TODO(dingbenjamin): Implement checksum
        if (!ReadUart(payload_buffer, header_buffer[2])) continue;

        uint8_t size = header_buffer[2];

        switch (header_buffer[3]) {
            case kPayloadProcessorInjection:
                // Create fake AX.25 bytes for the payload processor to throw
                // away
                byte fake_ax25_payload
                    [sizeof(payload_buffer) +
                     RunnablePayloadProcessor::kUplinkAx25Length];
                memset(fake_ax25_payload, 0x00,
                       RunnablePayloadProcessor::kUplinkAx25Length);
                memcpy(fake_ax25_payload +
                           RunnablePayloadProcessor::kUplinkAx25Length,
                       payload_buffer,
                       Lithium::kMaxReceivedUplinkSize -
                           RunnablePayloadProcessor::kUplinkAx25Length);
                Mailbox_post(Lithium::GetInstance()->GetUplinkMailbox(),
                             fake_ax25_payload, BIOS_WAIT_FOREVER);
        }
    }
}

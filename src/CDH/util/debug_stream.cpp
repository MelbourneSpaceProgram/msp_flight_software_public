#include "src/CDH/util/debug_stream.h"
#include <ti/sysbios/knl/Task.h>
#include <xdc/std.h>
#include "Board.h"
#include "src/CDH/util/serialised_message_builder.h"
#include "src/messages/test_message.h"
#include "src/util/data_types.h"

#define UARTA0 MSP_EXP432P401R_UARTA0
#define UARTA2 MSP_EXP432P401R_UARTA2

DebugStream::DebugStream() {
    char echo_prompt[] = "Debug stream started.\r\n";
    GPIO_init();
    UART_init();
    debug_config = UARTConfiguration(UARTConfiguration::BLOCKING_MODE,
                                     UARTConfiguration::BAUD_115200);
    debug = UART(&debug_config, UARTA0);
    debug.perform_write_transaction(echo_prompt, sizeof(echo_prompt));
}

void DebugStream::SendMessage(SerialisedMessage serial_msg) {
    // TODO(dingbenjamin): Remove explicit typecast after UART cleanup
    debug.perform_write_transaction(reinterpret_cast<char *>(serial_msg.buffer),
                                    serial_msg.size);
}

uint8_t DebugStream::ReceiveCode() {
    uint8_t read_code[1];
    uint8_t read_code_length = 1;
    // TODO(dingbenjamin): Remove explicit typecast after UART cleanup
    debug.perform_read_transaction(reinterpret_cast<char *>(read_code),
                                   read_code_length);
    return (uint8_t)read_code[0];
}

// TODO(dingbenjamin): Determine proper scoping of this function, should it be
// static?
void *DebugStream::InitDebugStream() {
    DebugStream debug_stream;
    while (1) {
        uint8_t code = debug_stream.ReceiveCode();
        switch (code) {
            case SerialisedMessageBuilder::kTemperatureSensor: {
                TemperatureMessage temp_msg(220.0, 44, 50);
                SerialisedMessage serial_msg = temp_msg.Serialise();
                debug_stream.SendMessage(serial_msg);
                break;
            }

            case SerialisedMessageBuilder::kRadiationSensor: {
                // TODO(dingbenjamin): Implement after RadiationSensor data is
                // confirmed
                break;
            }

            case SerialisedMessageBuilder::kTestSensor: {
                SendTestMessage(debug_stream, 0xF0);
                break;
            }

            default: {
                // TODO(dingbenjamin): Replace with invalid request message
                SendTestMessage(debug_stream, 0xFF);
                break;
            }
        }
        Task_sleep(500);
    }
}

void DebugStream::SendTestMessage(DebugStream debug_stream, byte data) {
    TestMessage msg(data);
    SerialisedMessage serial_msg = msg.Serialise();
    debug_stream.SendMessage(serial_msg);
}

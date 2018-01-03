#include <src/CDH/util/debug_stream.h>
#include <src/messages/temperature_message.h>
#include <src/messages/test_message.h>
#include <src/tasks/runnable_debug_stream.h>
#include <src/util/message_codes.h>

RunnableDebugStream::RunnableDebugStream() {}

fnptr RunnableDebugStream::GetRunnablePointer() {
    return &RunnableDebugStream::Run;
}

// cppcheck-suppress unusedFunction
void RunnableDebugStream::Run() {
    // TODO(dingbenjamin): This is currently a test implementation, upgrade to
    // actual when real sensor values are determined
    DebugStream debug_stream;
    while (true) {
        uint8_t code = debug_stream.ReceiveCode();
        switch (code) {
            case kMockTemperatureSensor: {
                TemperatureMessage temp_msg(220.0, 44, 50);
                debug_stream.SendMessage(temp_msg.Serialise());
                break;
            }

            case kMockRadiationSensor: {
                // TODO(dingbenjamin): Implement after RadiationSensor data is
                // confirmed
                break;
            }

            case kMockTestSensor: {
                TestMessage test_msg(0xF0);
                debug_stream.SendMessage(test_msg.Serialise());
                break;
            }

            default: {
                // TODO(dingbenjamin): Replace with invalid request message
                TestMessage default_msg(0xFF);
                debug_stream.SendMessage(default_msg.Serialise());
                break;
            }
        }
        Task_sleep(500);
    }
}

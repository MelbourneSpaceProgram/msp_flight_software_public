#include <src/debug_interface/debug_stream.h>
#include <src/debug_interface/runnable_debug_stream.h>
#include <src/messages/serialised_message.h>
#include <src/messages/temperature_message.h>
#include <src/messages/test_container_message.h>
#include <src/messages/test_message.h>
#include <src/util/message_codes.h>
#include <test_runners/debug_interface_tests.h>
#include <test_runners/unity.h>

static const DebugMessageVersion kCurrentVersion = kV1;

void DeleteAllocatedSerialMessage(SerialisedMessage *serial_message);

void TestTestDebugMessageQueue() {
    DebugStream *debug_stream = DebugStream::GetInstance();

    for (uint8_t i = 0; i < 16; i++) {
        TestMessage test_message(i);
        debug_stream->EnqueueSerialised(&test_message);
    }

    for (uint8_t i = 0; i < 16; i++) {
        SerialisedMessage *serial_message = debug_stream->DequeueSerialised();
        TEST_ASSERT_EQUAL_INT16(serial_message->GetSize(), 3);
        TEST_ASSERT_EQUAL_INT8(serial_message->GetBuffer()[0], kMockTestSensor);
        TEST_ASSERT_EQUAL_INT8(serial_message->GetBuffer()[1], kCurrentVersion);
        TEST_ASSERT_EQUAL_INT8(serial_message->GetBuffer()[2], i);

        DeleteAllocatedSerialMessage(serial_message);
    }
}

void TestTempDebugMessageQueue() {
    DebugStream *debug_stream = DebugStream::GetInstance();

    for (uint8_t i = 0; i < 16; i++) {
        uint8_t sensor_id = i;
        uint8_t timestamp = 255 - i;
        TemperatureMessage temp_message(i * 1.10, timestamp, sensor_id);
        debug_stream->EnqueueSerialised(&temp_message);
    }

    for (uint8_t i = 0; i < 16; i++) {
        SerialisedMessage *serial_message = debug_stream->DequeueSerialised();

        TEST_ASSERT_EQUAL_INT16(8, serial_message->GetSize());
        TEST_ASSERT_EQUAL_INT8(serial_message->GetBuffer()[0],
                               kMockTemperatureSensor);
        TEST_ASSERT_EQUAL_INT8(serial_message->GetBuffer()[1], kCurrentVersion);
        TEST_ASSERT_EQUAL_INT8(serial_message->GetBuffer()[2], i);
        TEST_ASSERT_EQUAL_INT8(serial_message->GetBuffer()[3], 255 - i);
        // TODO(dingbenjamin): Assert the next four bytes convert to the correct
        // float

        DeleteAllocatedSerialMessage(serial_message);
    }
}

void TestContainerDebugMessageQueue() {
    DebugStream *debug_stream = DebugStream::GetInstance();

    for (uint8_t i = 0; i < 16; i++) {
        uint8_t container_data = i;
        uint8_t inside_data = 255 - i;
        TestMessage test_message(inside_data);
        TestContainerMessage container_message(container_data, &test_message);
        debug_stream->EnqueueSerialised(&container_message);
    }

    for (uint8_t i = 0; i < 16; i++) {
        SerialisedMessage *serial_message = debug_stream->DequeueSerialised();
        TEST_ASSERT_EQUAL_INT16(serial_message->GetSize(), 6);
        TEST_ASSERT_EQUAL_INT8(serial_message->GetBuffer()[0],
                               kMockTestContainer);
        TEST_ASSERT_EQUAL_INT8(serial_message->GetBuffer()[1], kCurrentVersion);
        TEST_ASSERT_EQUAL_INT8(serial_message->GetBuffer()[2], i);
        TEST_ASSERT_EQUAL_INT8(serial_message->GetBuffer()[3], kMockTestSensor);
        TEST_ASSERT_EQUAL_INT8(serial_message->GetBuffer()[4], kCurrentVersion);
        TEST_ASSERT_EQUAL_INT8(serial_message->GetBuffer()[5], 255 - i);

        DeleteAllocatedSerialMessage(serial_message);
    }
}

void TestMixedDebugMessageQueue() {
    DebugStream *debug_stream = DebugStream::GetInstance();

    for (uint8_t i = 0; i < 16; i++) {
        TestMessage test_message(i);
        debug_stream->EnqueueSerialised(&test_message);
    }

    for (uint8_t i = 0; i < 16; i++) {
        uint8_t sensor_id = i;
        uint8_t timestamp = 255 - i;
        TemperatureMessage temp_message(i * 1.10, timestamp, sensor_id);
        debug_stream->EnqueueSerialised(&temp_message);
    }

    for (uint8_t i = 0; i < 16; i++) {
        uint8_t container_data = i;
        uint8_t inside_data = 255 - i;
        TestMessage test_message(inside_data);
        TestContainerMessage container_message(container_data, &test_message);
        debug_stream->EnqueueSerialised(&container_message);
    }

    for (uint8_t i = 0; i < 16; i++) {
        SerialisedMessage *serial_message = debug_stream->DequeueSerialised();
        TEST_ASSERT_EQUAL_INT16(serial_message->GetSize(), 3);
        TEST_ASSERT_EQUAL_INT8(serial_message->GetBuffer()[0], kMockTestSensor);
        TEST_ASSERT_EQUAL_INT8(serial_message->GetBuffer()[1], kCurrentVersion);
        TEST_ASSERT_EQUAL_INT8(serial_message->GetBuffer()[2], i);

        DeleteAllocatedSerialMessage(serial_message);
    }

    for (uint8_t i = 0; i < 16; i++) {
        SerialisedMessage *serial_message = debug_stream->DequeueSerialised();

        TEST_ASSERT_EQUAL_INT16(8, serial_message->GetSize());
        TEST_ASSERT_EQUAL_INT8(serial_message->GetBuffer()[0],
                               kMockTemperatureSensor);
        TEST_ASSERT_EQUAL_INT8(serial_message->GetBuffer()[1], kCurrentVersion);
        TEST_ASSERT_EQUAL_INT8(serial_message->GetBuffer()[2], i);
        TEST_ASSERT_EQUAL_INT8(serial_message->GetBuffer()[3], 255 - i);
        // TODO(dingbenjamin): Assert the next four bytes convert to the correct
        // float

        DeleteAllocatedSerialMessage(serial_message);
    }

    for (uint8_t i = 0; i < 16; i++) {
        SerialisedMessage *serial_message = debug_stream->DequeueSerialised();
        TEST_ASSERT_EQUAL_INT16(serial_message->GetSize(), 6);
        TEST_ASSERT_EQUAL_INT8(serial_message->GetBuffer()[0],
                               kMockTestContainer);
        TEST_ASSERT_EQUAL_INT8(serial_message->GetBuffer()[1], kCurrentVersion);
        TEST_ASSERT_EQUAL_INT8(serial_message->GetBuffer()[2], i);
        TEST_ASSERT_EQUAL_INT8(serial_message->GetBuffer()[3], kMockTestSensor);
        TEST_ASSERT_EQUAL_INT8(serial_message->GetBuffer()[4], kCurrentVersion);
        TEST_ASSERT_EQUAL_INT8(serial_message->GetBuffer()[5], 255 - i);

        DeleteAllocatedSerialMessage(serial_message);
    }
}

void DeleteAllocatedSerialMessage(SerialisedMessage *serial_message) {
    if (serial_message != NULL) {
        delete[] serial_message->GetBuffer();
        delete serial_message;
    }
}

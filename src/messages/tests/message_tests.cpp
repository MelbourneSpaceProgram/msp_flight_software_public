#include <src/messages/temperature_message.h>
#include <src/messages/test_container_message.h>
#include <src/messages/test_message.h>
#include <src/util/data_types.h>
#include <src/util/message_codes.h>
#include <test_runners/message_tests.h>
#include <test_runners/unity.h>
#include <src/messages/serialised_message_builder.h>

static const DebugMessageVersion kCurrentVersion = kV1;

void TestTestMessageSerialise(void) {
    for (int i = 0; i < 256; i++) {
        uint8_t data = static_cast<uint8_t>(i);
        TestMessage test_message(data);
        SerialisedMessage serial_message = test_message.Serialise();

        TEST_ASSERT_EQUAL_INT16(test_message.GetSerialisedSize(),
                                serial_message.GetSize());
        TEST_ASSERT_EQUAL_INT8(serial_message.GetBuffer()[0], kMockTestSensor);
        TEST_ASSERT_EQUAL_INT8(serial_message.GetBuffer()[1], kCurrentVersion);
        TEST_ASSERT_EQUAL_INT8(serial_message.GetBuffer()[2], data);
    }
}

void TestTempMessageSerialise(void) {
    for (int i = 0; i < 256; i++) {
        uint8_t sensor_id = static_cast<uint8_t>(i);
        uint8_t timestamp = static_cast<uint8_t>(255 - i);
        TemperatureMessage temp_message(i * 1.10, timestamp, sensor_id);
        SerialisedMessage serial_message = temp_message.Serialise();

        TEST_ASSERT_EQUAL_INT16(temp_message.GetSerialisedSize(),
                                serial_message.GetSize());
        TEST_ASSERT_EQUAL_INT8(serial_message.GetBuffer()[0],
                               kMockTemperatureSensor);
        TEST_ASSERT_EQUAL_INT8(serial_message.GetBuffer()[1], kCurrentVersion);
        TEST_ASSERT_EQUAL_INT8(serial_message.GetBuffer()[2], sensor_id);
        TEST_ASSERT_EQUAL_INT8(serial_message.GetBuffer()[3], timestamp);
        // TODO(dingbenjamin): Assert the next four bytes convert to the correct
        // float
    }
}

void TestContainerMessageSerialise(void) {
    for (int i = 0; i < 256; i++) {
        uint8_t container_data = static_cast<uint8_t>(i);
        uint8_t inside_data = static_cast<uint8_t>(255 - i);
        TestMessage test_message(inside_data);
        TestContainerMessage container_message(container_data, &test_message);
        SerialisedMessage serial_message = container_message.Serialise();

        TEST_ASSERT_EQUAL_INT16(container_message.GetSerialisedSize(), 6);
        TEST_ASSERT_EQUAL_INT16(container_message.GetSerialisedSize(),
                                serial_message.GetSize());
        TEST_ASSERT_EQUAL_INT8(serial_message.GetBuffer()[0],
                               kMockTestContainer);
        TEST_ASSERT_EQUAL_INT8(serial_message.GetBuffer()[1], kCurrentVersion);
        TEST_ASSERT_EQUAL_INT8(serial_message.GetBuffer()[2], container_data);
        TEST_ASSERT_EQUAL_INT8(serial_message.GetBuffer()[3], kMockTestSensor);
        TEST_ASSERT_EQUAL_INT8(serial_message.GetBuffer()[4], kCurrentVersion);
        TEST_ASSERT_EQUAL_INT8(serial_message.GetBuffer()[5], inside_data);
    }
}

void TestSerialisedMessageBuilder(void) {
    byte* buffer = new byte[10];
    SerialisedMessageBuilder builder(buffer, 10);
    TEST_ASSERT_EQUAL_INT16(builder.GetSerialisedLength(), 0);

    builder.AddData<byte>(0xF0);
    TEST_ASSERT_EQUAL_INT16(builder.GetSerialisedLength(), 1);

    builder.AddData<uint8_t>(0x0F);
    TEST_ASSERT_EQUAL_INT16(builder.GetSerialisedLength(), 2);

    TestMessage test_message(0xC0);
    builder.AddMessage(&test_message);

    TEST_ASSERT_EQUAL_INT8(buffer[0], 0xF0);
    TEST_ASSERT_EQUAL_INT8(buffer[1], 0x0F);
    TEST_ASSERT_EQUAL_INT8(buffer[2], kMockTestSensor);
    TEST_ASSERT_EQUAL_INT8(buffer[3], kCurrentVersion);
    TEST_ASSERT_EQUAL_INT8(buffer[4], 0xC0);
    TEST_ASSERT_EQUAL_INT8_ARRAY(builder.GetSerialisedMessageBuffer(), buffer,
                                 5);
    TEST_ASSERT_EQUAL_INT8_ARRAY(builder.GetSerialisedMessageBuffer(),
                                 builder.Build().GetBuffer(), 5);

    delete[] buffer;
}

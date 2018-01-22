#include <src/messages/temperature_message.h>
#include <src/messages/test_container_message.h>
#include <src/messages/test_message.h>
#include <src/util/data_types.h>
#include <src/util/message_codes.h>
#include <test_runners/message_tests.h>
#include <test_runners/unity.h>
#include <src/messages/serialised_message_builder.h>
// TODO(dingbenjamin): serialised_message_builder.h needs to be the last include
// or a linker error will occur, this needs to be fixed

static const DebugMessageVersion kCurrentVersion = kV1;

void TestTestMessageSerialise(void) {
    for (int i = 0; i < 256; i++) {
        uint8_t data = static_cast<uint8_t>(i);
        TestMessage test_message(data);
        SerialisedMessage serial_message = test_message.Serialise();

        TEST_ASSERT_EQUAL_INT16(test_message.GetSerialisedSize(),
                                serial_message.GetSize());
        TEST_ASSERT_EQUAL_INT8(kMockTestSensor, serial_message.GetBuffer()[0]);
        TEST_ASSERT_EQUAL_INT8(kCurrentVersion, serial_message.GetBuffer()[1]);
        TEST_ASSERT_EQUAL_INT8(data, serial_message.GetBuffer()[2]);
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
        TEST_ASSERT_EQUAL_INT8(kMockTemperatureSensor,
                               serial_message.GetBuffer()[0]);
        TEST_ASSERT_EQUAL_INT8(kCurrentVersion, serial_message.GetBuffer()[1]);
        TEST_ASSERT_EQUAL_INT8(sensor_id, serial_message.GetBuffer()[2]);
        TEST_ASSERT_EQUAL_INT8(timestamp, serial_message.GetBuffer()[3]);
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

        TEST_ASSERT_EQUAL_INT16(6, container_message.GetSerialisedSize());
        TEST_ASSERT_EQUAL_INT16(container_message.GetSerialisedSize(),
                                serial_message.GetSize());
        TEST_ASSERT_EQUAL_INT8(kMockTestContainer,
                               serial_message.GetBuffer()[0]);
        TEST_ASSERT_EQUAL_INT8(kCurrentVersion, serial_message.GetBuffer()[1]);
        TEST_ASSERT_EQUAL_INT8(container_data, serial_message.GetBuffer()[2]);
        TEST_ASSERT_EQUAL_INT8(kMockTestSensor, serial_message.GetBuffer()[3]);
        TEST_ASSERT_EQUAL_INT8(kCurrentVersion, serial_message.GetBuffer()[4]);
        TEST_ASSERT_EQUAL_INT8(inside_data, serial_message.GetBuffer()[5]);
    }
}

void TestSerialisedMessageBuilder(void) {
    byte* buffer = new byte[10];
    SerialisedMessageBuilder builder(buffer, 10);
    TEST_ASSERT_EQUAL_INT16(0, builder.GetSerialisedLength());

    builder.AddData<byte>(0xF0);
    TEST_ASSERT_EQUAL_INT16(1, builder.GetSerialisedLength());

    builder.AddData<uint8_t>(0x0F);
    TEST_ASSERT_EQUAL_INT16(2, builder.GetSerialisedLength());

    TestMessage test_message(0xC0);
    builder.AddMessage(&test_message);

    TEST_ASSERT_EQUAL_INT8(0xF0, buffer[0]);
    TEST_ASSERT_EQUAL_INT8(0x0F, buffer[1]);
    TEST_ASSERT_EQUAL_INT8(kMockTestSensor, buffer[2]);
    TEST_ASSERT_EQUAL_INT8(kCurrentVersion, buffer[3]);
    TEST_ASSERT_EQUAL_INT8(0xC0, buffer[4]);
    TEST_ASSERT_EQUAL_INT8_ARRAY(buffer, builder.GetSerialisedMessageBuffer(),
                                 5);
    TEST_ASSERT_EQUAL_INT8_ARRAY(builder.GetSerialisedMessageBuffer(),
                                 builder.Build().GetBuffer(), 5);

    delete[] buffer;
}

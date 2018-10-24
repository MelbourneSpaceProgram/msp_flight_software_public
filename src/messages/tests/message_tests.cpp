#include <CppUTest/TestHarness.h>
#include <external/etl/array.h>
#include <src/messages/serialised_message_builder.h>
#include <src/messages/temperature_message.h>
#include <src/messages/test_container_message.h>
#include <src/messages/test_message.h>
#include <src/util/data_types.h>
#include <src/util/message_codes.h>

static constexpr DebugMessageVersion kCurrentVersion = kV1;

TEST_GROUP(Message){};

TEST(Message, TestMessageSerialise) {
    for (int i = 0; i < 256; i++) {
        uint8_t data = static_cast<uint8_t>(i);
        TestMessage test_message(data);
        SerialisedMessage serial_message = test_message.Serialise();

        CHECK_EQUAL(test_message.GetSerialisedSize(), serial_message.GetSize());
        BYTES_EQUAL(kMockTestSensor, serial_message.GetBuffer()[0]);
        BYTES_EQUAL(kCurrentVersion, serial_message.GetBuffer()[1]);
        BYTES_EQUAL(data, serial_message.GetBuffer()[2]);
    }
}

TEST(Message, TempMessageSerialise) {
    for (int i = 0; i < 256; i++) {
        uint8_t sensor_id = static_cast<uint8_t>(i);
        uint8_t timestamp = static_cast<uint8_t>(255 - i);
        TemperatureMessage temp_message(i * 1.10, timestamp, sensor_id);
        SerialisedMessage serial_message = temp_message.Serialise();

        CHECK_EQUAL(temp_message.GetSerialisedSize(), serial_message.GetSize());
        BYTES_EQUAL(kMockTemperatureSensor, serial_message.GetBuffer()[0]);
        BYTES_EQUAL(kCurrentVersion, serial_message.GetBuffer()[1]);
        BYTES_EQUAL(sensor_id, serial_message.GetBuffer()[2]);
        BYTES_EQUAL(timestamp, serial_message.GetBuffer()[3]);
        // TODO(dingbenjamin): Assert the next four bytes convert to the correct
        // float
    }
}

TEST(Message, ContainerMessageSerialise) {
    for (int i = 0; i < 256; i++) {
        uint8_t container_data = static_cast<uint8_t>(i);
        uint8_t inside_data = static_cast<uint8_t>(255 - i);
        TestMessage test_message(inside_data);
        TestContainerMessage container_message(container_data, &test_message);
        SerialisedMessage serial_message = container_message.Serialise();

        CHECK_EQUAL(6, container_message.GetSerialisedSize());
        CHECK_EQUAL(container_message.GetSerialisedSize(),
                    serial_message.GetSize());
        BYTES_EQUAL(kMockTestContainer, serial_message.GetBuffer()[0]);
        BYTES_EQUAL(kCurrentVersion, serial_message.GetBuffer()[1]);
        BYTES_EQUAL(container_data, serial_message.GetBuffer()[2]);
        BYTES_EQUAL(kMockTestSensor, serial_message.GetBuffer()[3]);
        BYTES_EQUAL(kCurrentVersion, serial_message.GetBuffer()[4]);
        BYTES_EQUAL(inside_data, serial_message.GetBuffer()[5]);
    }
}

TEST(Message, SerialisedMessageBuilder) {
    byte buffer[10];
    SerialisedMessageBuilder builder(buffer, 10);
    CHECK_EQUAL(0, builder.GetSerialisedLength());

    builder.AddData<byte>(0xF0);
    CHECK_EQUAL(1, builder.GetSerialisedLength());

    builder.AddData<uint8_t>(0x0F);
    CHECK_EQUAL(2, builder.GetSerialisedLength());

    TestMessage test_message(0xC0);
    builder.AddMessage(&test_message);

    BYTES_EQUAL(0xF0, buffer[0]);
    BYTES_EQUAL(0x0F, buffer[1]);
    BYTES_EQUAL(kMockTestSensor, buffer[2]);
    BYTES_EQUAL(kCurrentVersion, buffer[3]);
    BYTES_EQUAL(0xC0, buffer[4]);
    MEMCMP_EQUAL(buffer, builder.GetSerialisedMessageBuffer(), 5);
    MEMCMP_EQUAL(builder.GetSerialisedMessageBuffer(),
                 builder.Build().GetBuffer(), 5);
}

TEST(Message, PadWithZero) {
    byte buffer[10];
    SerialisedMessageBuilder builder(buffer, 10);

    builder.AddData<byte>(0xF0);
    CHECK_EQUAL(1, builder.GetSerialisedLength());

    builder.AddData<uint8_t>(0x0F);
    CHECK_EQUAL(2, builder.GetSerialisedLength());

    builder.PadZeroes();

    SerialisedMessage serial_message = builder.Build();
    BYTES_EQUAL(0xF0, serial_message.GetBuffer()[0]);
    BYTES_EQUAL(0x0F, serial_message.GetBuffer()[1]);
    for (uint8_t i = 2; i < 10; i++) {
        BYTES_EQUAL(0x00, serial_message.GetBuffer()[i]);
    }
}

TEST(Message, SerialiseEtlArray) {
    etl::array<uint8_t, 3> test_array = {1, 2, 3};
    byte buffer[3];
    SerialisedMessageBuilder builder(buffer, 3);

    builder.AddEtlArray<uint8_t, 3>(test_array);
    SerialisedMessage serial_message = builder.Build();

    CHECK_EQUAL(1, serial_message.GetBuffer()[0]);
    BYTES_EQUAL(2, serial_message.GetBuffer()[1]);
    BYTES_EQUAL(3, serial_message.GetBuffer()[2]);
}

TEST(Message, SerialiseArray) {
    uint8_t test_array[3] = {1, 2, 3};
    byte buffer[3];
    SerialisedMessageBuilder builder(buffer, 3);

    builder.AddArray<uint8_t>(test_array, 3);
    SerialisedMessage serial_message = builder.Build();

    CHECK_EQUAL(1, serial_message.GetBuffer()[0]);
    BYTES_EQUAL(2, serial_message.GetBuffer()[1]);
    BYTES_EQUAL(3, serial_message.GetBuffer()[2]);
}

TEST(Message, RebuildableMessageFieldIterator) {
    byte buffer[10];
    TemperatureMessage message(123, 1, 2);
    message.SerialiseTo(buffer);

    // Now try rebuilding
    TemperatureMessage rebuilt(buffer);

    BYTES_EQUAL(message.sensor_id, rebuilt.sensor_id);
    BYTES_EQUAL(message.timestamp, rebuilt.timestamp);
    DOUBLES_EQUAL(message.temperature, rebuilt.temperature, 0.01);
}

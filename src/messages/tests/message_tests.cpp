#include <src/messages/serialised_message.h>
#include <src/messages/test_message.h>
#include <test_runners/message_tests.h>
#include <test_runners/unity.h>

void TestSerialiseMessage(void) {
    TestMessage test_message(0x10);
    SerialisedMessage serial_message = test_message.Serialise();

    TEST_ASSERT_EQUAL_INT16(3, serial_message.size);

    TEST_ASSERT_EQUAL_INT8(serial_message.buffer[0], 0x03);
    TEST_ASSERT_EQUAL_INT8(serial_message.buffer[1], 0x00);
    TEST_ASSERT_EQUAL_INT8(serial_message.buffer[2], 0x10);
}

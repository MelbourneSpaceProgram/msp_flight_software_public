#include <CppUTest/TestHarness.h>
#include <src/board/board.h>
#include <src/board/i2c/i2c.h>
#include <src/board/i2c/io_expander/io_expander.h>
#include <src/board/i2c/multiplexers/i2c_multiplexer.h>
#include <src/config/unit_tests.h>
#include <src/telecomms/lithium.h>
#include <src/telecomms/msp_payloads/test_ones_payload.h>
#include <src/util/data_types.h>
#include <src/util/task_utils.h>

TEST_GROUP(IoExpander) {
    void setup() {
        if (!kI2cAvailable) {
            TEST_EXIT;
        }
    };
};

TEST(IoExpander, TestIoExpander) {
    IoExpander::GetIoExpander(IoExpander::kCommsIoExpander)
        ->SetPin(IoExpander::kIoExpanderPinTelecomsEn1, false);
    IoExpander::GetIoExpander(IoExpander::kCommsIoExpander)
        ->SetPin(IoExpander::kIoExpanderPinTelecomsEn3, false);
    IoExpander::GetIoExpander(IoExpander::kCommsIoExpander)
        ->SetPin(IoExpander::kIoExpanderPinTelecomsEn4, false);
    IoExpander::GetIoExpander(IoExpander::kCommsIoExpander)
        ->SetPin(IoExpander::kIoExpanderPinTelecomsEn5, false);

    TaskUtils::SleepMilli(2000);

    TestOnesPayload ones;
    CHECK_FALSE(Lithium::GetInstance()->Transmit(&ones));

    IoExpander::GetIoExpander(IoExpander::kCommsIoExpander)
        ->SetPin(IoExpander::kIoExpanderPinTelecomsEn1, true);
    IoExpander::GetIoExpander(IoExpander::kCommsIoExpander)
        ->SetPin(IoExpander::kIoExpanderPinTelecomsEn3, true);
    IoExpander::GetIoExpander(IoExpander::kCommsIoExpander)
        ->SetPin(IoExpander::kIoExpanderPinTelecomsEn4, true);
    IoExpander::GetIoExpander(IoExpander::kCommsIoExpander)
        ->SetPin(IoExpander::kIoExpanderPinTelecomsEn5, true);

    TaskUtils::SleepMilli(2000);

    CHECK(Lithium::GetInstance()->Transmit(&ones));
}

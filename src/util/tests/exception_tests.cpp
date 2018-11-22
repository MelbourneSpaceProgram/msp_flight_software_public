#include <CppUTest/TestHarness.h>
#include <src/config/stacks.h>
#include <src/util/message_codes.h>
#include <src/util/msp_exception.h>

TEST_GROUP(MspException) {
    void setup() { MspException::ClearAll(); };

    void teardown() { MspException::ClearAll(); };
};

TEST(MspException, TestExceptionLog) {
    for (uint8_t i = 0; i < kNumExceptionTypes; ++i) {
        try {
            throw MspException("This is a test", static_cast<ErrorId>(i),
                               __FILE__, __LINE__, 2, 3);
        } catch (MspException& e) {
            MspException::LogException(e, true);
        }
    }

    const uint8_t* num_exceptions = MspException::GetNumAll();
    for (uint8_t i = 0; i < kNumExceptionTypes; ++i) {
        CHECK_EQUAL(1, num_exceptions[i]);
    }

    for (uint8_t i = 0; i < MspException::kNumEachException; ++i) {
        try {
            throw MspException("This is a test", kFileWriteFail, __FILE__,
                               __LINE__, i, i);
        } catch (MspException& e) {
            MspException::LogException(e, static_cast<CatchId>(i), true);
        }
    }

    const SerialisedException* exceptions_of_type =
        MspException::GetExceptions(kFileWriteFail);

    for (uint8_t i = 1; i < MspException::kNumEachException; ++i) {
        SerialisedException current_exception = exceptions_of_type[i];
        CHECK_EQUAL(kFileWriteFail, current_exception.error_id);
        CHECK_EQUAL(i - 1, current_exception.catch_id);
        CHECK_EQUAL('U', static_cast<uint8_t>(current_exception.task_name));
        CHECK(current_exception.task_stack_used > 0);
        CHECK(current_exception.task_stack_used < kUnitTestsStackSize);
        CHECK_EQUAL(3, current_exception.task_priority);
        CHECK_EQUAL(i - 1, current_exception.exception_param_1);
        CHECK_EQUAL(i - 1, current_exception.exception_param_2);
    }

    CHECK_EQUAL(MspException::GetNumType(kFileWriteFail),
                MspException::kNumEachException);

    // Check the overflow worked correctly
    CHECK_EQUAL(kFileWriteFail, exceptions_of_type[0].error_id);
    CHECK_EQUAL(MspException::kNumEachException - 1,
                exceptions_of_type[0].catch_id);
    CHECK_EQUAL('U', exceptions_of_type[0].task_name);
    CHECK(exceptions_of_type[0].task_stack_used > 0);
    CHECK(exceptions_of_type[0].task_stack_used < kUnitTestsStackSize);
    CHECK_EQUAL(3, exceptions_of_type[0].task_priority);
    CHECK_EQUAL(MspException::kNumEachException - 1,
                exceptions_of_type[0].exception_param_1);
    CHECK_EQUAL(MspException::kNumEachException - 1,
                exceptions_of_type[0].exception_param_2);
}

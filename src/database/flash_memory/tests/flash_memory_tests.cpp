#include <CppUTest/TestHarness.h>
#include <src/config/unit_tests.h>
#include <src/database/flash_memory/flash_memory_management.h>
#include <src/database/flash_memory/flash_storables/reset_info_container.h>
#include <src/util/reset_management.h>

TEST_GROUP(FlashMemory){};

TEST(FlashMemory, ResetCounter) {
    if (kResetUnitTestEnabled) {
        ResetInfoContainer *reset_info_container =
            ResetInfoContainer::GetInstance();
        switch (reset_info_container->GetMostRecentResetMessage()) {
            case kResetUnitTestMessage1:
                CHECK(reset_info_container->GetMostRecentResetCause() ==
                      kSoftwareReset);
                ResetManagement::ResetSystemWithMessage(kResetUnitTestMessage2);
                FAIL_TEST("System did not reset.");
                break;
            case kResetUnitTestMessage2:
                CHECK(reset_info_container->GetMostRecentResetCause() ==
                      kSoftwareReset);
                TEST_EXIT;
                break;
            default:
                ResetManagement::ResetSystemWithMessage(kResetUnitTestMessage1);
                FAIL_TEST("System did not reset");
                break;
        }
    }
}

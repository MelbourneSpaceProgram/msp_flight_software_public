#include <src/config/unit_tests.h>
#include <src/util/memory_troubleshooter.h>
#include <test_runners/memory_troubleshooter_tests.h>
#include <test_runners/unity.h>

void TestMemoryLeak() {
    if (!memory_troubleshooter_test_enabled) {
        TEST_IGNORE_MESSAGE("Memory Troubleshooter test ignored");
    }

    MemoryTroubleshooter *memory_test = new MemoryTroubleshooter();

    int *test = new int[100];

    bool memory_leak_true = memory_test->MemoryLeakTest();
    delete test;
    bool memory_leak_false = memory_test->MemoryLeakTest();

    TEST_ASSERT(memory_leak_true);
    TEST_ASSERT(!memory_leak_false);

    memory_test->~MemoryTroubleshooter();
}

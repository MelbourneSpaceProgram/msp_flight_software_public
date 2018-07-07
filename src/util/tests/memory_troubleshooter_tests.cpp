#include <CppUTest/TestHarness.h>
#include <src/config/unit_tests.h>
#include <src/util/memory_troubleshooter.h>

TEST_GROUP(MemoryLeakTroubleshooter){};

TEST(MemoryLeakTroubleshooter, TestMemoryLeak) {
    MemoryTroubleshooter memory_test;

    int *test = new int[100];

    CHECK(memory_test.MemoryLeakDetected());
    delete[] test;
    CHECK_FALSE(memory_test.MemoryLeakDetected());
}

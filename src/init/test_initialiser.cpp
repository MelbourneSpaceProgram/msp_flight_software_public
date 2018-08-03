#include <CppUTest/CommandLineTestRunner.h>
#include <src/config/unit_tests.h>
#include <src/init/test_initialiser.h>

TestInitialiser* TestInitialiser::instance = NULL;

TestInitialiser* TestInitialiser::GetInstance() {
    if (instance == NULL) {
        instance = new TestInitialiser();
    }
    return instance;
}

fnptr TestInitialiser::GetRunnablePointer() { return &RunUnitTests; }

void TestInitialiser::RunUnitTests() {
    // Size of the array of POINTERS divided by the size of one POINTER
    int num_test_args = sizeof(test_args) / sizeof(test_args[0]);

    RUN_ALL_TESTS(num_test_args, test_args);
}

#include <CppUTest/CommandLineTestRunner.h>
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
    // See CppUTest Guide.md in top folder for how-to. The below examples can be
    // chained together to run all but some tests, or to only run a certain set
    // of tests. A test that has been bypassed due to a configuration in
    // src/config/unit_tests.h will report as a success.
    // Examples:

    // Run all tests quietly, printing only final statistics and any failures
    // const char* test_args[] = {"exe"};

    // Run all tests verbosely
    // const char* test_args[] = {"exe", "-v"};

    // Run only this group of tests
    // const char* test_args[] = {"exe", "-sg", "NameOfTestGroup"};

    // Run everything but this group of tests
    // const char* test_args[] = {"exe", "-xg", "NameOfTestGroup"};

    // Run only this test
    // const char* test_args[] = {"exe", "-sn", "NameOfTestGroup"};

    // Run everything but this test
    // const char* test_args[] = {"exe", "-xn", "NameOfTest"};

    const char* test_args[] = {"exe"};
    // Size of the array of POINTERS divided by the size of one POINTER
    int num_test_args = sizeof(test_args) / sizeof(test_args[0]);

    RUN_ALL_TESTS(num_test_args, test_args);
}

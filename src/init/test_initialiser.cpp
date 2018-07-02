#include <src/init/test_initialiser.h>
#include <test_runners/master_runner.h>

TestInitialiser* TestInitialiser::instance = NULL;

TestInitialiser* TestInitialiser::GetInstance() {
    if (instance == NULL) {
        instance = new TestInitialiser();
    }
    return instance;
}

fnptr TestInitialiser::GetRunnablePointer() { return &RunUnitTests; }

void TestInitialiser::RunUnitTests() {
    const char* av[] = {"exe", "-sg", "Lithium"};
    int ac = 3;
    RUN_ALL_TESTS(ac, av);
}
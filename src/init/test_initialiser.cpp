#include <src/init/test_initialiser.h>
#include <test_runners/master_runner.h>

TestInitialiser* TestInitialiser::instance = NULL;

TestInitialiser* TestInitialiser::GetInstance() {
    if (instance == NULL) {
        instance = new TestInitialiser();
    }
    return instance;
}

TestInitialiser::TestInitialiser() : test_finished(NULL) {}

void TestInitialiser::InitSemaphore(Semaphore_Handle sem_handle) {
    if (test_finished == NULL) {
        test_finished = sem_handle;
    }
}

void TestInitialiser::FinishTests() {
    Semaphore_post(test_finished);
}

fnptr TestInitialiser::GetRunnablePointer() { return &RunUnitTests; }

void TestInitialiser::RunUnitTests() {
    master_tests_thread();
    TestInitialiser::GetInstance()->FinishTests();
}

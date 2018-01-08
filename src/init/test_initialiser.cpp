#include <src/init/test_initialiser.h>
#include <test_runners/master_runner.h>

TestInitialiser::TestInitialiser() {}

fnptr TestInitialiser::GetRunnablePointer() { return &master_tests_thread; }

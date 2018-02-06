#ifndef SRC_INIT_TEST_INITIALISER_H_
#define SRC_INIT_TEST_INITIALISER_H_

#include <src/tasks/runnable.h>
#include <ti/sysbios/knl/Semaphore.h>

class TestInitialiser : public Runnable {
   public:
    static TestInitialiser* GetInstance();
    fnptr GetRunnablePointer();
    void InitSemaphore(Semaphore_Handle test_finished);
    void FinishTests();

   private:
    static TestInitialiser* instance;
    static void RunUnitTests();

    TestInitialiser();
    Semaphore_Handle test_finished;
};

#endif  // SRC_INIT_TEST_INITIALISER_H_

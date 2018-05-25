#ifndef SRC_INIT_TEST_INITIALISER_H_
#define SRC_INIT_TEST_INITIALISER_H_

#include <src/tasks/runnable.h>
#include <src/util/data_types.h>

class TestInitialiser : public Runnable {
   public:
    static TestInitialiser* GetInstance();
    fnptr GetRunnablePointer();

   private:
    static TestInitialiser* instance;
    static void RunUnitTests();
};

#endif  // SRC_INIT_TEST_INITIALISER_H_

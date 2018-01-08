#ifndef SRC_INIT_TEST_INITIALISER_H_
#define SRC_INIT_TEST_INITIALISER_H_

#include <src/tasks/runnable.h>

class TestInitialiser : public Runnable {
   public:
    fnptr GetRunnablePointer();
    TestInitialiser();
};

#endif  // SRC_INIT_TEST_INITIALISER_H_

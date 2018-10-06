#ifndef SRC_TASKS_RUNNABLE_H_
#define SRC_TASKS_RUNNABLE_H_

#include <ti/sysbios/knl/Task.h>

typedef Task_FuncPtr fnptr;
// Equivalent of below
// typedef void (*fnptr)(uintptr_t, uintptr_t);

class Runnable {
   public:
    virtual fnptr GetRunnablePointer() = 0;
    virtual ~Runnable();
};

#endif  // SRC_TASKS_RUNNABLE_H_

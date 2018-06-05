#ifndef SRC_SYSTEM_TASKS_RUNNABLE_STATE_MANAGEMENT_H_
#define SRC_SYSTEM_TASKS_RUNNABLE_STATE_MANAGEMENT_H_

#include <src/tasks/runnable.h>

class RunnableStateManagement : public Runnable {
   public:
    fnptr GetRunnablePointer();
    RunnableStateManagement();

   private:
    static void StateManagement();
};

#endif  //  SRC_SYSTEM_TASKS_RUNNABLE_STATE_MANAGEMENT_H_

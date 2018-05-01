#ifndef SRC_SYSTEM_RUNNABLE_SYSTEM_HEALTH_CHECK_H_
#define SRC_SYSTEM_RUNNABLE_SYSTEM_HEALTH_CHECK_H_

#include <src/tasks/runnable.h>

class RunnableSystemHealthCheck : public Runnable {
   public:
    fnptr GetRunnablePointer();
    RunnableSystemHealthCheck();

   private:
    static void SystemHealthCheck();
};

#endif  //  SRC_SYSTEM_RUNNABLE_SYSTEM_HEALTH_CHECK_H_

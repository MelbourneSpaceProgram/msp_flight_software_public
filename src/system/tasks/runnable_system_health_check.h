#ifndef SRC_SYSTEM_RUNNABLE_SYSTEM_HEALTH_CHECK_H_
#define SRC_SYSTEM_RUNNABLE_SYSTEM_HEALTH_CHECK_H_

#include <src/tasks/runnable.h>

class RunnableSystemHealthCheck : public Runnable {
   public:
    fnptr GetRunnablePointer();
    RunnableSystemHealthCheck();

   private:
    static void SystemHealthCheck();
    static void PollSensors();
    //TODO(naverill) set  kMaxCacheTimeMilliseconds
    static const uint64_t kMaxCacheTimeMilliseconds;

};

#endif  //  SRC_SYSTEM_RUNNABLE_SYSTEM_HEALTH_CHECK_H_

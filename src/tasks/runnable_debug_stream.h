#ifndef SRC_TASKS_RUNNABLE_DEBUG_STREAM_H_
#define SRC_TASKS_RUNNABLE_DEBUG_STREAM_H_

#include <src/tasks/runnable.h>

class RunnableDebugStream : public Runnable {
   public:
    RunnableDebugStream();
    fnptr GetRunnablePointer();

   private:
    // cppcheck-suppress unusedFunction
    static void Run();
};

#endif  // SRC_TASKS_RUNNABLE_DEBUG_STREAM_H_

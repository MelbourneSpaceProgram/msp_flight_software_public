#ifndef SRC_DEBUG_INTERFACE_RUNNABLE_DEBUG_STREAM_H_
#define SRC_DEBUG_INTERFACE_RUNNABLE_DEBUG_STREAM_H_

#include <src/tasks/runnable.h>

class RunnableDebugStream : public Runnable {
   public:
    RunnableDebugStream();
    fnptr GetRunnablePointer();

   private:
    // TODO(dingbenjamin): Suprress cppcheck error on unused function here
    static void Run();
};

#endif  // SRC_DEBUG_INTERFACE_RUNNABLE_DEBUG_STREAM_H_

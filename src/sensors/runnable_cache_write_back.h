#ifndef SRC_SENSORS_RUNNABLE_CACHE_WRITE_BACK_H_
#define SRC_SENSORS_RUNNABLE_CACHE_WRITE_BACK_H_

#include <src/tasks/runnable.h>

class RunnableCacheWriteBack : public Runnable {
    friend void TestCacheWriteBack();

   public:
    RunnableCacheWriteBack();
    fnptr GetRunnablePointer();
    static void WriteBack();

   private:
    static const uint16_t kNumStoredReadings = 1000;
    static const uint16_t kMaxCachedAgeMilli = 1000;
    static const uint16_t kWriteBackPeriod = 60000;

    static void WriteBackTemp(uint16_t measurable_id);
};

#endif  // SRC_SENSORS_RUNNABLE_CACHE_WRITE_BACK_H_

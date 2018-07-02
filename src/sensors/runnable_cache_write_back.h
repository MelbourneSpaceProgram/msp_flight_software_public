#ifndef SRC_SENSORS_RUNNABLE_CACHE_WRITE_BACK_H_
#define SRC_SENSORS_RUNNABLE_CACHE_WRITE_BACK_H_

#include <src/tasks/runnable.h>

class RunnableCacheWriteBack : public Runnable {
   public:
    RunnableCacheWriteBack();
    fnptr GetRunnablePointer();
    static void WriteBack();
    // TODO(akremor): Have made this public and removed the friend. Needing to
    // use friend might suggest we are testing this in the wrong way (testing
    // implementation details not the public interface).
    static void WriteBackTemp(uint16_t measurable_id);

   private:
    static const uint16_t kNumStoredReadings = 1000;
    static const uint16_t kMaxCachedAgeMilli = 1000;
    static const uint16_t kWriteBackPeriod = 60000;
};

#endif  // SRC_SENSORS_RUNNABLE_CACHE_WRITE_BACK_H_

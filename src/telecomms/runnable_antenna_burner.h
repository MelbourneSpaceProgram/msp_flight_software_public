#ifndef SRC_TELECOMMS_RUNNABLE_ANTENNA_BURNER_H_
#define SRC_TELECOMMS_RUNNABLE_ANTENNA_BURNER_H_

#include <src/database/flash_memory/flash_storables/antenna_burner_info.h>
#include <src/tasks/runnable.h>
#include <src/util/data_types.h>

class RunnableAntennaBurner : public Runnable {
   public:
    fnptr GetRunnablePointer();
    static AntennaBurnerInfo *GetAntennaBurnerInfo();

   private:
    static void PeriodicAntennaBurner();
    static constexpr uint16_t kAntennaBurnDisableWait = 10*1000;

    static AntennaBurnerInfo *antenna_burner_info;
};

#endif  // SRC_TELECOMMS_RUNNABLE_ANTENNA_BURNER_H_

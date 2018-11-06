#ifndef SRC_INIT_INIT_H_
#define SRC_INIT_INIT_H_

#include <src/util/data_types.h>

void PreBiosInit();
void EnterLimpMode();
void InitRtcInterrupt();
void InitSdCs();

static constexpr uint32_t kMaxNumResets = 100;

#endif  // SRC_INIT_INIT_H_

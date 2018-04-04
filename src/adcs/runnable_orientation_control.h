#ifndef SRC_ADCS_RUNNABLE_ORIENTATION_CONTROL_H_
#define SRC_ADCS_RUNNABLE_ORIENTATION_CONTROL_H_

#include <external/nanopb/pb.h>
#include <src/tasks/runnable.h>
#include <src/util/data_types.h>

class RunnableOrientationControl : public Runnable {
   public:
    fnptr GetRunnablePointer();
    RunnableOrientationControl();

   private:
    static void ControlOrientation();
};

#endif  // SRC_ADCS_RUNNABLE_ORIENTATION_CONTROL_H_

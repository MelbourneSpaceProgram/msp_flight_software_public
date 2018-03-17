#ifndef SRC_DATA_DASHBOARD_RUNNABLE_DATA_DASHBOARD_H_
#define SRC_DATA_DASHBOARD_RUNNABLE_DATA_DASHBOARD_H_

#include <external/nanopb/pb.h>
#include <src/tasks/runnable.h>
#include <src/util/data_types.h>

class RunnableDataDashboard : public Runnable {
   public:
    fnptr GetRunnablePointer();
    RunnableDataDashboard();
    static void TransmitMessage(const byte message_code,
                                const uint8_t message_size,
                                const pb_field_t* message_fields,
                                const void* message_struct);

   private:
    static void DataDashboard();
};

#endif  // SRC_DATA_DASHBOARD_RUNNABLE_DATA_DASHBOARD_H_

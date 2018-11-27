#ifndef SRC_SENSORS_SOFTWARE_MEASURABLES_RESET_INFO_CONTAINER_MEASURABLE_H_
#define SRC_SENSORS_SOFTWARE_MEASURABLES_RESET_INFO_CONTAINER_MEASURABLE_H_

#include <src/database/flash_memory/flash_storables/reset_info_container.h>
#include <src/messages/ResetInfoContainerReading.pb.h>
#include <src/sensors/nanopb_measurable.h>

class ResetInfoContainerMeasurable
    : public NanopbMeasurable<ResetInfoContainerReading> {
   public:
    ResetInfoContainerMeasurable();
    std::string GetInfoString();
    ResetInfoContainerReading TakeDirectNanopbReading();
   private:
    std::string info_string;
};

#endif  //  SRC_SENSORS_SOFTWARE_MEASURABLES_RESET_INFO_CONTAINER_MEASURABLE_H_

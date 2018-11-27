#ifndef SRC_SENSORS_SOFTWARE_MEASURABLES_ANTENNA_BURNER_INFO_MEASURABLE_H_
#define SRC_SENSORS_SOFTWARE_MEASURABLES_ANTENNA_BURNER_INFO_MEASURABLE_H_

#include <src/messages/AntennaBurnerInfoReading.pb.h>
#include <src/sensors/nanopb_measurable.h>

class AntennaBurnerInfoMeasurable
    : public NanopbMeasurable<AntennaBurnerInfoReading> {
   public:
    AntennaBurnerInfoMeasurable();
    std::string GetInfoString();
    AntennaBurnerInfoReading TakeDirectNanopbReading();
   private:
    std::string info_string;
};

#endif  //  SRC_SENSORS_SOFTWARE_MEASURABLES_ANTENNA_BURNER_INFO_MEASURABLE_H_

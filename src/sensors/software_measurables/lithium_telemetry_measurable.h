#ifndef SRC_SENSORS_SOFTWARE_MEASURABLES_LITHIUM_TELEMTRY_READING_H_
#define SRC_SENSORS_SOFTWARE_MEASURABLES_LITHIUM_TELEMTRY_READING_H_

#include <src/messages/LithiumTelemetryReading.pb.h>
#include <src/sensors/nanopb_measurable.h>

class LithiumTelemetryMeasurable
    : public NanopbMeasurable<LithiumTelemetryReading> {
   public:
    LithiumTelemetryMeasurable();
    std::string GetInfoString();
    LithiumTelemetryReading TakeDirectNanopbReading();
   private:
    std::string info_string;
    static constexpr uint8_t kNumTimeCountBytes = 3;
};

#endif  //  SRC_SENSORS_SOFTWARE_MEASURABLES_LITHIUM_TELEMTRY_READING_H_

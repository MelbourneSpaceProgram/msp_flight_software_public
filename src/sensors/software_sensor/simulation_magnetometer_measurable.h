#ifndef SRC_SENSORS_SOFTWARE_SENSOR_SIMULATION_MAGNETOMETER_MEASURABLE_H_
#define SRC_SENSORS_SOFTWARE_SENSOR_SIMULATION_MAGNETOMETER_MEASURABLE_H_

#include <src/sensors/nanopb_measurable.h>
#include <src/util/matrix.h>

class SimulationMagnetometerMeasurable
    : public NanopbMeasurable<MagnetometerReading> {
   public:
    SimulationMagnetometerMeasurable();
    MagnetometerReading TakeDirectNanopbReading();
    std::string GetInfoString();
};

#endif  //  SRC_SENSORS_SOFTWARE_SENSOR_SIMULATION_MAGNETOMETER_MEASURABLE_H_

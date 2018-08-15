#ifndef RADIATION_MEASURABLE_H_
#define RADIATION_MEASURABLE_H_

#include <src/sensors/i2c_sensors/measurables/i2c_measurable.h>
#include <src/sensors/i2c_sensors/rad_driver.h>

class RadiationMeasurable : public I2cMeasurable<double> {
   public:
    RadiationMeasurable(RadDriver* rad_sensor);
    double TakeDirectI2cReading();

   private:
    // TODO(daniel632): Is this a suitable flag?
    static const double kFailedRadiationReading = -9999;
};

#endif /* RADIATION_MEASURABLE_H_ */

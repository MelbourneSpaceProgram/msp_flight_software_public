#ifndef SRC_SENSORS_I2C_SENSORS_MEASURABLES_IR_MEASURABLE_H_
#define SRC_SENSORS_I2C_SENSORS_MEASURABLES_IR_MEASURABLE_H_

#include <src/sensors/i2c_sensors/measurables/i2c_measurable.h>
#include <src/sensors/i2c_sensors/mlx90615.h>
#include <src/sensors/magnetometer_calibration.h>
#include <src/util/matrix.h>

class InfraredMeasurable : public I2cMeasurable<InfraredReading> {
   public:
    explicit InfraredMeasurable(Mlx90615 *ir_sensor);
    InfraredReading TakeDirectI2cReading();
};

#endif  //  SRC_SENSORS_I2C_SENSORS_MEASURABLES_IR_MEASURABLE_H_

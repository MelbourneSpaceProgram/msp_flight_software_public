#include <external/etl/exception.h>
#include <src/sensors/i2c_sensors/measurables/infrared_measurable.h>

InfraredMeasurable::InfraredMeasurable(Mlx90615* ir_sensor)
    : I2cMeasurable<InfraredReading>(ir_sensor, InfraredReading_init_default) {}

InfraredReading InfraredMeasurable::TakeDirectI2cReading() {
    Mlx90615* ir_sensor = static_cast<Mlx90615*>(I2cMeasurable::sensor);
    InfraredReading reading = InfraredReading_init_default;
    reading.infrared = ir_sensor->ReadInfrared();
    return reading;
}

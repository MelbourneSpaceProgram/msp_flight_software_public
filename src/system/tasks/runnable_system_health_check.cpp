#include <src/sensors/i2c_measurable_manager.h>
#include <src/sensors/i2c_sensors/measurables/temperature_measurable.h>
#include <src/sensors/measurable_id.h>
#include <src/system/sensor_state_machines/battery_temp_state_machine.h>
#include <src/system/state_manager.h>
#include <src/system/tasks/runnable_system_health_check.h>
#include <src/util/task_utils.h>
#include <src/util/system_watchdog.h>

RunnableSystemHealthCheck::RunnableSystemHealthCheck() {}

fnptr RunnableSystemHealthCheck::GetRunnablePointer() {
    return &RunnableSystemHealthCheck::SystemHealthCheck;
}

void RunnableSystemHealthCheck::SystemHealthCheck() {
    I2cMeasurableManager* measurable_manager =
        I2cMeasurableManager::GetInstance();

    while (1) {
        measurable_manager->ReadI2cMeasurable<double>(kPowerBmsBatteryTemp1, 1);
        measurable_manager->ReadI2cMeasurable<double>(kPowerBmsBatteryTemp2, 1);

        SystemWatchdog::ResetTimer();
        TaskUtils::SleepMilli(1000);
    }
}

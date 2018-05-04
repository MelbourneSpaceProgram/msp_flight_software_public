#include <src/sensors/i2c_measurable_manager.h>
#include <src/sensors/i2c_sensors/measurables/temperature_measurable.h>
#include <src/sensors/measurable_id.h>
#include <src/system/sensor_state_machines/battery_temp_state_machine.h>
#include <src/system/state_manager.h>
#include <src/system/tasks/runnable_system_health_check.h>
#include <src/util/task_utils.h>

RunnableSystemHealthCheck::RunnableSystemHealthCheck() {}

fnptr RunnableSystemHealthCheck::GetRunnablePointer() {
    return &RunnableSystemHealthCheck::SystemHealthCheck;
}

void RunnableSystemHealthCheck::SystemHealthCheck() {
    while (1) {
        PollSensors();
        TaskUtils::SleepMilli(1000);
    }
}

void RunnableStateManagement::PollSensors(){
  I2cMeasurableManager *measurable_manager = I2cMeasurableManager::GetInstance();

  for(int id=0; id<MeasurableId::kNumMeasurables; id++){
    if(measurable_manager->CheckValidMeasurable(id)){
        //do something
        measurable_manager->ReadI2cMeasurable(id, kMaxCacheTimeMilliseconds)
    }
  }
}

#include <src/sensors/i2c_measurable_manager.h>
#include <src/sensors/measurable_id.h>
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

  for(int i=0; i<kNumMeasurables; i++){
    MeasurableId id = static_cast<MeasurableId>i;
    if(measurable_manager->CheckValidMeasurable(id)){
        //do something
        measurable_manager->ReadI2cMeasurable(id, kMaxCacheTimeMilliseconds)
    }
  }
}

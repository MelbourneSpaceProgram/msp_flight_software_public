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

void RunnableSystemHealthCheck::PollSensors(){
  I2cMeasurableManager *measurable_manager = I2cMeasurableManager::GetInstance();

  for(uint16_t i=0; i<kNumMeasurables; i++){
    MeasurableId id = static_cast<MeasurableId>(i);
    if(measurable_manager->CheckValidMeasurable(id)){
        //do something
        //TODO(naverill) remove bool type, replace with Measurable T type
        measurable_manager->ReadI2cMeasurable<bool>(i, kMaxCacheTimeMilliseconds);
    }
  }
}

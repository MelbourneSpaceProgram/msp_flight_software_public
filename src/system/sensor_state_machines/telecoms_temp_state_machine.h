#ifndef SRC_SYSTEM_SENSOR_STATE_MACHINES_TELECOMS_TEMP_STATE_MACHINE_H_
#define SRC_SYSTEM_SENSOR_STATE_MACHINES_TELECOMS_TEMP_STATE_MACHINE_H_

#include <src/system/sensor_state_machine.h>

class TestI2cSensor;

// TODO(wschuetz) Set correct type for sensor state machine when known
// Changes required at forward declaration, constructor, Update() and
// .cpp include
class TelecomsTempStateMachine : public SensorStateMachine<TestI2cSensor> {
   public:
    explicit TelecomsTempStateMachine(StateManager* state_manager);
    void Update();

   private:
    void UpdateState(float temp);

    static const uint8_t kTempTelecomsOperationalMax = 80;
    static const uint8_t kHysteresis = 1;
};

#endif  //  SRC_SYSTEM_SENSOR_STATE_MACHINES_TELECOMS_TEMP_STATE_MACHINE_H_

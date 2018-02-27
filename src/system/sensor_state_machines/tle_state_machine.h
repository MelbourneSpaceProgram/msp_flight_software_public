#ifndef SRC_SYSTEM_SENSOR_STATE_MACHINES_TLE_STATE_MACHINE_H_
#define SRC_SYSTEM_SENSOR_STATE_MACHINES_TLE_STATE_MACHINE_H_

#include <src/system/sensor_state_machine.h>

class SoftwareSensor;

// TODO(wschuetz) Set correct type for sensor state machine when known
// Changes required at forward declaration, constructor, Update() and
// .cpp include
class TleStateMachine : public SensorStateMachine<SoftwareSensor> {
   public:
    explicit TleStateMachine(StateManager* state_manager);
    void Update();

   private:
    void UpdateState(double reading);
};

#endif  //  SRC_SYSTEM_SENSOR_STATE_MACHINES_TLE_STATE_MACHINE_H_

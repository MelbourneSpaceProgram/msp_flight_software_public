#ifndef SRC_SYSTEM_SENSOR_STATE_MACHINES_DETUMBLED_STATE_MACHINE_H_
#define SRC_SYSTEM_SENSOR_STATE_MACHINES_DETUMBLED_STATE_MACHINE_H_

#include <src/system/sensor_state_machine.h>

class SoftwareSensor;

// TODO(wschuetz) Set correct type for sensor state machine when known
// Changes required at forward declaration, constructor, Update() and
// .cpp include
class DetumbledStateMachine : public SensorStateMachine<SoftwareSensor> {
   public:
    explicit DetumbledStateMachine(StateManager* state_manager);
    void Update();

   private:
    void UpdateState(double spin_rate);
};

#endif  //  SRC_SYSTEM_SENSOR_STATE_MACHINES_DETUMBLED_STATE_MACHINE_H_

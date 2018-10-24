#ifndef SRC_SYSTEM_SENSOR_STATE_MACHINES_BATTERY_CHARGE_STATE_MACHINE_H_
#define SRC_SYSTEM_SENSOR_STATE_MACHINES_BATTERY_CHARGE_STATE_MACHINE_H_

#include <src/system/sensor_state_machine.h>

class TestI2cMeasurable;

// TODO(wschuetz) Set correct type for sensor state machine when known
// Changes required at forward declaration, constructor, Update() and
// .cpp include
class BatteryChargeStateMachine : public SensorStateMachine<TestI2cMeasurable> {
   public:
    explicit BatteryChargeStateMachine(StateManager* state_manager);
    void Update();

   private:
    void UpdateState(double soc);

    static constexpr float kTwoEclipseCapacity = 0.2;
    static constexpr float kThreePassCapacity = 0.4;
    static constexpr float kHysteresis = 0.02;
};

#endif  //  SRC_SYSTEM_SENSOR_STATE_MACHINES_BATTERY_CHARGE_STATE_MACHINE_H_

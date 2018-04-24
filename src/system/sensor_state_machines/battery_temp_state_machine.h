#ifndef SRC_SYSTEM_SENSOR_STATE_MACHINES_BATTERY_TEMP_STATE_MACHINE_H_
#define SRC_SYSTEM_SENSOR_STATE_MACHINES_BATTERY_TEMP_STATE_MACHINE_H_

#include <src/sensors/i2c_sensors/measurables/bms_temperature_measurable.h>
#include <src/system/sensor_state_machine.h>

// TODO(wschuetz) Set correct type for sensor state machine when known
// Changes required at forward declaration, constructor, Update() and
// .cpp include
class BatteryTempStateMachine
    : public SensorStateMachine<BmsTemperatureMeasurable> {
   public:
    explicit BatteryTempStateMachine(StateManager* state_manager);
    void Update();

   private:
    void UpdateState(double temp);

    static const uint8_t kTempBatteryOperationalHigh = 60;
    static const uint8_t kHysteresis = 5;
};

#endif  //  SRC_SYSTEM_SENSOR_STATE_MACHINES_BATTERY_TEMP_STATE_MACHINE_H_

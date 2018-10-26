#ifndef SRC_SYSTEM_SENSOR_STATE_MACHINES_TELECOMS_TEMP_STATE_MACHINE_H_
#define SRC_SYSTEM_SENSOR_STATE_MACHINES_TELECOMS_TEMP_STATE_MACHINE_H_

#include <src/sensors/i2c_sensors/measurables/temperature_measurable.h>
#include <src/sensors/measurable_id.h>
#include <src/system/sensor_state_machine.h>
#include <src/system/state_definitions.h>

class TestI2cMeasurable;

// TODO(wschuetz) Set correct type for sensor state machine when known
// Changes required at forward declaration, constructor, Update() and
// .cpp include
class TelecomsTempStateMachine
    : public SensorStateMachine<TemperatureMeasurable> {
   public:
    explicit TelecomsTempStateMachine(StateManager* state_manager);
    void Update();

   private:
    enum TelecomsIndex { kTelecoms1, kTelecoms2, kTotalTelecoms };
    float telecoms_1_temp;
    float telecoms_2_temp;

    void UpdateState(float temp, TelecomsIndex id);

    static const int8_t kTempTelecomsOperationalMax = 55;
    static const int8_t kTempTelecomsOperationalMin = -15;
    static const uint8_t kHysteresis = 5;
};

#endif  //  SRC_SYSTEM_SENSOR_STATE_MACHINES_TELECOMS_TEMP_STATE_MACHINE_H_

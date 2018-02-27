#include <src/system/sensor_state_machines/tle_state_machine.h>
#include <src/sensors/software_sensor/software_sensor.h>

TleStateMachine::TleStateMachine(StateManager* state_manager)
    : SensorStateMachine<SoftwareSensor>(state_manager, kTleInvalid) {}

void TleStateMachine::Update() {
    SoftwareSensor* sensor_with_reading = GetSensorWithReading();
    if (sensor_with_reading != NULL) {
        UpdateState(sensor_with_reading->GetReading());
    }
}

void TleStateMachine::UpdateState(double reading) {
    // TODO(wschuetz) Add state transition logic here
}

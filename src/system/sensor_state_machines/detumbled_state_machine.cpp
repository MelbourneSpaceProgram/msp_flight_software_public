#include <src/sensors/software_sensor/software_sensor.h>
#include <src/system/sensor_state_machines/detumbled_state_machine.h>

DetumbledStateMachine::DetumbledStateMachine(StateManager* state_manager)
    : SensorStateMachine<SoftwareSensor>(state_manager, kDetumbledFalse) {}

void DetumbledStateMachine::Update() {
    SoftwareSensor* sensor_with_reading = GetSensorWithReading();
    if (sensor_with_reading != NULL) {
        UpdateState(sensor_with_reading->GetReading());
    }
}

void DetumbledStateMachine::UpdateState(double spin_rate) {
    // TODO(wschuetz) Add state transition logic here
}

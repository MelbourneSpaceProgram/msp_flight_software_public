#ifndef SRC_SYSTEM_SENSOR_STATE_MACHINE_H_
#define SRC_SYSTEM_SENSOR_STATE_MACHINE_H_

#include <src/observers/generic_observer.h>
#include <src/system/state_definitions.h>
#include <src/system/state_machine.h>
#include <src/system/state_manager.h>
#include <src/i2c/sensors/i2c_sensor.hpp>

template <typename T>
class SensorStateMachine : public GenericObserver<T>, public StateMachine {
   protected:
    SensorStateMachine<T>(StateManager* state_manager, StateId initial_state)
        : StateMachine(state_manager, initial_state) {}
    void SetStateAndNotify(StateId new_state) {
        SetState(new_state);
        NotifyManagerOfStateChange();
    }

   private:
    void NotifyManagerOfStateChange() {
        Semaphore_post(GetStateManager()->GetSemaphoreHandle());
    }
};

#endif  // SRC_SYSTEM_SENSOR_STATE_MACHINE_H_

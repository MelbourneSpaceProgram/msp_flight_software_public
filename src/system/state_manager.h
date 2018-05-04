#ifndef SRC_SYSTEM_STATE_MANAGER_H_
#define SRC_SYSTEM_STATE_MANAGER_H_

#include <external/etl/array.h>
#include <external/etl/vector.h>
#include <src/system/state_definitions.h>
#include <ti/sysbios/knl/Semaphore.h>

class SystemStateMachine;
class StateMachine;

class StateManager {
   public:
    StateManager();
    static StateManager* GetStateManager();
    void ProcessStateChanges();
    Semaphore_Handle GetSemaphoreHandle() const;
    StateMachine* GetStateMachine(StateMachineIndex index);
    void AddStateMachine(StateMachineIndex index, StateMachine* state_machine);
    bool AddSystemStateMachine(SystemStateMachine* system_state_machine);
    void CreateStateMachines();
    void RemoveLastSystemStateMachine();
    void DeleteInstance();
    StateId GetCurrentStateOfStateMachine(StateMachineIndex index);

   private:
    ~StateManager();
    void NotifySystems();
    void InitFunctionEnableMailboxes();
    static StateManager* instance;
    static const uint8_t TotalSystemStateMachines = 3;
    Semaphore_Handle state_update_semaphore_handle;
    etl::array<StateMachine*, kNumStateMachines> state_machines;
    etl::vector<SystemStateMachine*, TotalSystemStateMachines>
        system_state_machines;
};

#endif  // SRC_SYSTEM_STATE_MANAGER_H_

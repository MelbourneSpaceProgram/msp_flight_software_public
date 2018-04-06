#include <src/system/sensor_state_machine.h>
#include <src/system/sensor_state_machines/battery_charge_state_machine.h>
#include <src/system/sensor_state_machines/battery_temp_state_machine.h>
#include <src/system/sensor_state_machines/detumbled_state_machine.h>
#include <src/system/sensor_state_machines/telecoms_temp_state_machine.h>
#include <src/system/sensor_state_machines/tle_state_machine.h>
#include <src/system/state_manager.h>
#include <src/system/system_state_machine.h>
#include <src/system/system_state_machines/adcs_state_machine.h>
#include <src/system/system_state_machines/power_state_machine.h>
#include <src/system/system_state_machines/telecoms_state_machine.h>
#include <src/util/data_types.h>
#include <external/etl/exception.h>
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Semaphore.h>

StateManager* StateManager::instance = NULL;

StateManager::StateManager() {
    Semaphore_Params semaphore_params;
    Semaphore_Params_init(&semaphore_params);
    state_update_semaphore_handle =
        Semaphore_create(0, &semaphore_params, NULL);
    if (state_update_semaphore_handle == NULL) {
        etl::exception e("Unable to create state update semaphore", __FILE__,
                         __LINE__);
        throw e;
    }
}

StateManager* StateManager::GetStateManager() {
    if (instance == NULL) {
        instance = new StateManager();
    }
    return instance;
}

void StateManager::NotifySystems() {
    for (etl::vector<SystemStateMachine*, TotalSystemStateMachines>::iterator
             it = system_state_machines.begin();
         it != system_state_machines.end(); it++) {
        (*it)->CheckUpstreamStates();
    }
}

void StateManager::AddStateMachine(StateMachineIndex index,
                                   StateMachine* state_machine) {
    state_machines.insert_at(index, state_machine);
}

StateMachine* StateManager::GetStateMachine(StateMachineIndex index) {
    // TODO(rskew): check for vaild indices
    return state_machines[index];
}

bool StateManager::AddSystemStateMachine(
    SystemStateMachine* system_state_machine) {
    if (system_state_machines.available()) {
        system_state_machines.push_back(system_state_machine);
        return true;
    } else {
        // If exceptions are not enabled undefined behavior occurs.
        return false;
    }
}

void StateManager::ProcessStateChanges() {
    // Block until a state change has occurred
    Semaphore_pend(state_update_semaphore_handle, BIOS_WAIT_FOREVER);
    NotifySystems();
}

Semaphore_Handle StateManager::GetSemaphoreHandle() const {
    return state_update_semaphore_handle;
}

void StateManager::CreateStateMachines() {
    BatteryChargeStateMachine* battery_charge_state_machine =
        new BatteryChargeStateMachine(this);
    AddStateMachine(kBatteryChargeStateMachine, battery_charge_state_machine);

    BatteryTempStateMachine* battery_temp_state_machine =
        new BatteryTempStateMachine(this);
    AddStateMachine(kBatteryTempStateMachine, battery_temp_state_machine);

    TelecomsTempStateMachine* telecoms_temp_state_machine =
        new TelecomsTempStateMachine(this);
    AddStateMachine(kTelecomsTempStateMachine, telecoms_temp_state_machine);

    TleStateMachine* tle_state_machine = new TleStateMachine(this);
    AddStateMachine(kTleStateMachine, tle_state_machine);

    DetumbledStateMachine* detumbled_state_machine =
        new DetumbledStateMachine(this);
    AddStateMachine(kDetumbledStateMachine, detumbled_state_machine);

    PowerStateMachine* power_state_machine = new PowerStateMachine(
        this, battery_charge_state_machine, battery_temp_state_machine);
    AddStateMachine(kPowerStateMachine, power_state_machine);
    AddSystemStateMachine(power_state_machine);

    TelecomsStateMachine* telecoms_state_machine = new TelecomsStateMachine(
        this, power_state_machine, telecoms_temp_state_machine);
    AddStateMachine(kTelecomsStateMachine, telecoms_state_machine);
    AddSystemStateMachine(telecoms_state_machine);

    AdcsStateMachine* adcs_state_machine = new AdcsStateMachine(
        this, power_state_machine, tle_state_machine, detumbled_state_machine);
    AddStateMachine(kAdcsStateMachine, adcs_state_machine);
    AddSystemStateMachine(adcs_state_machine);
}

void StateManager::RemoveLastSystemStateMachine() {
    if (system_state_machines.available()) {
        system_state_machines.pop_back();
    }
}

void StateManager::DeleteInstance() {
    delete instance;
}

StateManager::~StateManager() {
    for (etl::array<StateMachine*, kNumStateMachines>::iterator
             it = state_machines.begin();
         it != state_machines.end(); it++) {
        delete (*it);
    }
    state_machines.erase_range(0, kNumStateMachines - 1);
    system_state_machines.clear();
    Semaphore_delete(&state_update_semaphore_handle);
    state_update_semaphore_handle = NULL;
    instance = NULL;
}

#ifndef SRC_SYSTEM_STATE_DEFINITIONS_H_
#define SRC_SYSTEM_STATE_DEFINITIONS_H_

enum StateId {
    kBatteryChargeCriticalLow,
    kBatteryChargeLow,
    kBatteryChargeNominal,
    kBatteryTempCriticalLow,
    kBatteryTempNominal,
    kBatteryTempCriticalHigh,
    kPowerEverythingOff,
    kPowerLimited,
    kPowerNominal,
    kPowerNoState,
    // TODO(rskew) label telecoms states descriptively
    kTelecoms0,
    kTelecoms1,
    kTelecoms2,
    kTelecomsNoState,
    kTelecomsTempNominal,
    kTelecomsTempCriticalHigh,
    kMcuRedundancy0,
    kMcuRedundancy1,
    kMcuIntegrity0,
    kMcuIntegrity1,
    kTleInvalid,
    kTleValid,
    kDetumbledFalse,
    kDetumbledTrue,
    kAdcsOff,
    kAdcsDetumbling,
    kAdcsNadirPointing,
    kAdcsNominal,
    kAdcsNoState,
    kStateIdLength
};

enum StateMachineIndex {
    kBatteryChargeStateMachine,
    kBatteryTempStateMachine,
    kTelecomsTempStateMachine,
    kTleStateMachine,
    kDetumbledStateMachine,
    kPowerStateMachine,
    kTelecomsStateMachine,
    kAdcsStateMachine,
    kNumStateMachines
};

enum FunctionEnableHandle {
    kOrientationControlEnable,
    kTotalFunctionEnableHandles
};

#endif  // SRC_SYSTEM_STATE_DEFINITIONS_H_

#include <src/util/msp_exception.h>
#include <src/util/satellite_power.h>
#include <src/util/task_utils.h>
#include <xdc/runtime/Log.h>

void SatellitePower::Initialize() {
    const IoExpander* io_expander_bms = IoExpander::GetIoExpander(0);
    const IoExpander* io_expander_telecoms = IoExpander::GetIoExpander(1);

    try {
        io_expander_bms->SetPin(kIoExpanderPinBms1En, true);
        io_expander_bms->SetPin(kIoExpanderPinBms2En, true);
        io_expander_bms->SetPin(kIoExpanderPinFSEn, false);

        io_expander_bms->SetDirection(kIoExpanderPinBms1En,
                                      IoExpander::kIoOutput);
        io_expander_bms->SetDirection(kIoExpanderPinBms2En,
                                      IoExpander::kIoOutput);
        io_expander_bms->SetPolarity(kIoExpanderPinBms1En,
                                     IoExpander::kIoActiveHigh);
        io_expander_bms->SetPolarity(kIoExpanderPinBms2En,
                                     IoExpander::kIoActiveHigh);

        io_expander_bms->SetDirection(kIoExpanderPinFSEn,
                                      IoExpander::kIoOutput);
        io_expander_bms->SetPolarity(kIoExpanderPinFSEn,
                                     IoExpander::kIoActiveHigh);
    } catch (etl::exception& e) {
        MspException::LogException(e);
        Log_error0("BMS IO expander failed to initialise properly");
    }

    try {
        io_expander_telecoms->SetPin(kIoExpanderPinTelecomsEn1, false);
        io_expander_telecoms->SetPin(kIoExpanderPinTelecomsEn4, false);

        io_expander_telecoms->SetDirection(kIoExpanderPinTelecomsEn1,
                                           IoExpander::kIoOutput);
        io_expander_telecoms->SetDirection(kIoExpanderPinTelecomsEn4,
                                           IoExpander::kIoOutput);
        io_expander_telecoms->SetPolarity(kIoExpanderPinTelecomsEn1,
                                          IoExpander::kIoActiveHigh);
        io_expander_telecoms->SetPolarity(kIoExpanderPinTelecomsEn4,
                                          IoExpander::kIoActiveHigh);
    } catch (etl::exception& e) {
        MspException::LogException(e);
        Log_error0("Telecomms IO expander failed to initialise properly");
    }
}

void SatellitePower::CutPowerFromPanels() {
    Log_info0("Cutting power from solar panels");
    const IoExpander* io_expander_bms =
        IoExpander::GetIoExpander(IoExpander::kEpsIoExpander);
    io_expander_bms->SetPin(kIoExpanderPinBms1En, false);
    io_expander_bms->SetPin(kIoExpanderPinBms2En, false);
}

void SatellitePower::RestorePowerFromPanels() {
    Log_info0("Restoring power from solar panels");
    const IoExpander* io_expander_bms =
        IoExpander::GetIoExpander(IoExpander::kEpsIoExpander);
    io_expander_bms->SetPin(kIoExpanderPinBms1En, true);
    io_expander_bms->SetPin(kIoExpanderPinBms2En, true);
}

void SatellitePower::CutPowerToFlightSystems() {
    Log_info0("Cutting power to Flight Systems");
    const IoExpander* io_expander_bms =
        IoExpander::GetIoExpander(IoExpander::kEpsIoExpander);
    io_expander_bms->SetPin(kIoExpanderPinFSEn, false);
}

void SatellitePower::RestorePowerToFlightSystems() {
    Log_info0("Restoring power to Flight Systems");
    const IoExpander* io_expander_bms =
        IoExpander::GetIoExpander(IoExpander::kEpsIoExpander);
    io_expander_bms->SetPin(kIoExpanderPinFSEn, true);
}

void SatellitePower::CutPowerToTelecoms() {
    Log_info0("Cutting power to Telecoms");
    const IoExpander* io_expander_telecoms =
        IoExpander::GetIoExpander(IoExpander::kCommsIoExpander);
    io_expander_telecoms->SetPin(kIoExpanderPinTelecomsEn1, false);
    io_expander_telecoms->SetPin(kIoExpanderPinTelecomsEn4, false);
}

void SatellitePower::RestorePowerToTelecoms() {
    Log_info0("Restoring power to Telecoms");
    const IoExpander* io_expander_telecoms =
        IoExpander::GetIoExpander(IoExpander::kCommsIoExpander);
    io_expander_telecoms->SetPin(kIoExpanderPinTelecomsEn1, true);
    io_expander_telecoms->SetPin(kIoExpanderPinTelecomsEn4, true);
}

#include <src/util/drop_baseload.h>
#include <src/util/task_utils.h>
#include <xdc/runtime/Log.h>

void DropBaseload::Initialize() {
    const IoExpander* io_expander_bms = IoExpander::GetIoExpander(0);
    const IoExpander* io_expander_telecoms = IoExpander::GetIoExpander(1);

    io_expander_bms->SetDirection(kIoExpanderPinBms1En, IoExpander::kIoOutput);
    io_expander_bms->SetDirection(kIoExpanderPinBms2En, IoExpander::kIoOutput);
    io_expander_bms->SetPolarity(kIoExpanderPinBms1En,
                                 IoExpander::kIoActiveHigh);
    io_expander_bms->SetPolarity(kIoExpanderPinBms2En,
                                 IoExpander::kIoActiveHigh);

    io_expander_bms->SetDirection(kIoExpanderPinFSEn, IoExpander::kIoOutput);
    io_expander_bms->SetPolarity(kIoExpanderPinFSEn, IoExpander::kIoActiveHigh);

    io_expander_telecoms->SetDirection(kIoExpanderPinTelecomsEn1,
                                       IoExpander::kIoOutput);
    io_expander_telecoms->SetDirection(kIoExpanderPinTelecomsEn4,
                                       IoExpander::kIoOutput);
    io_expander_telecoms->SetPolarity(kIoExpanderPinTelecomsEn1,
                                      IoExpander::kIoActiveHigh);
    io_expander_telecoms->SetPolarity(kIoExpanderPinTelecomsEn4,
                                      IoExpander::kIoActiveHigh);
}

void DropBaseload::CutPowerFromPanels() {
    Log_info0("Cutting power from solar panels");
    const IoExpander* io_expander_bms = IoExpander::GetIoExpander(0);
    io_expander_bms->SetPin(kIoExpanderPinBms1En, false);
    io_expander_bms->SetPin(kIoExpanderPinBms2En, false);
}

void DropBaseload::RestorePowerFromPanels() {
    Log_info0("Restoring power from solar panels");
    const IoExpander* io_expander_bms = IoExpander::GetIoExpander(0);
    io_expander_bms->SetPin(kIoExpanderPinBms1En, true);
    io_expander_bms->SetPin(kIoExpanderPinBms2En, true);
}

void DropBaseload::CutPowerToFlightSystems() {
    Log_info0("Cutting power to Flight Systems");
    const IoExpander* io_expander_bms = IoExpander::GetIoExpander(0);
    io_expander_bms->SetPin(kIoExpanderPinFSEn, false);
}

void DropBaseload::RestorePowerToFlightSystems() {
    Log_info0("Restoring power to Flight Systems");
    const IoExpander* io_expander_bms = IoExpander::GetIoExpander(0);
    io_expander_bms->SetPin(kIoExpanderPinFSEn, true);
}

void DropBaseload::CutPowerToTelecoms() {
    Log_info0("Cutting power to Telecoms");
    const IoExpander* io_expander_telecoms = IoExpander::GetIoExpander(1);
    io_expander_telecoms->SetPin(kIoExpanderPinTelecomsEn1, false);
    io_expander_telecoms->SetPin(kIoExpanderPinTelecomsEn4, false);
}

void DropBaseload::RestorePowerToTelecoms() {
    Log_info0("Restoring power to Telecoms");
    const IoExpander* io_expander_telecoms = IoExpander::GetIoExpander(1);
    io_expander_telecoms->SetPin(kIoExpanderPinTelecomsEn1, true);
    io_expander_telecoms->SetPin(kIoExpanderPinTelecomsEn4, true);
}

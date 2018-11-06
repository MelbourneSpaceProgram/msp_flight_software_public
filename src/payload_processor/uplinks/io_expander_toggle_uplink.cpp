#include <external/etl/exception.h>
#include <src/messages/IoExpanderToggleUplinkPayload.pb.h>
#include <src/payload_processor/uplinks/io_expander_toggle_uplink.h>
#include <src/util/nanopb_utils.h>
#include <src/util/satellite_power.h>
#include <src/util/task_utils.h>

IoExpanderToggleUplink::IoExpanderToggleUplink(byte* payload)
    : Uplink(IoExpanderToggleUplinkPayload_size),
      decoded_command(NanopbDecode(IoExpanderToggleUplinkPayload)(payload)),
      io_expander_id(decoded_command.io_expander_id),
      pin(static_cast<IoExpander::IoPin>(decoded_command.pin)),
      toggle_type(static_cast<ToggleType>(decoded_command.toggle_type)),
      toggle_duration(decoded_command.toggle_duration) {}

bool IoExpanderToggleUplink::ExecuteUplink() {
    // TODO(dingbenjamin): Include invalid value
    // in exception
    if (pin >= IoExpander::kNumIoExpanderPins) {
        throw etl::exception("Invalid pin number", __FILE__, __LINE__);
    } else if (io_expander_id >= IoExpander::kNumIoExpanders) {
        throw etl::exception("Invalid IO expander ID", __FILE__, __LINE__);
    }

    const IoExpander* expander = IoExpander::GetIoExpander(io_expander_id);

    switch (toggle_type) {
        case kToggleOffOn:
            expander->SetPin(pin, false);
            TaskUtils::SleepMilli(toggle_duration);
            expander->SetPin(pin, true);
            break;

        case kToggleOn:
            expander->SetPin(pin, true);
            break;

        case kToggleOff:
            expander->SetPin(pin, false);
            break;

        case kToggleOffOnFlightSystems:
            SatellitePower::CutPowerToFlightSystems();
            TaskUtils::SleepMilli(toggle_duration);
            SatellitePower::RestorePowerToFlightSystems();
            break;

        case kToggleOnFlightSystems:
            SatellitePower::RestorePowerToFlightSystems();
            break;

        case kToggleOffFlightSystems:
            SatellitePower::CutPowerToFlightSystems();
            break;

        case kToggleOffOnTelecomms:
            SatellitePower::CutPowerToTelecoms();
            TaskUtils::SleepMilli(toggle_duration);
            SatellitePower::RestorePowerToTelecoms();
            break;

        case kToggleOnTelecomms:
            SatellitePower::RestorePowerToTelecoms();
            break;

        case kToggleOffTelecomms:
            SatellitePower::CutPowerToTelecoms();
            break;

        case kToggleOffOnPanels:
            SatellitePower::CutPowerFromPanels();
            TaskUtils::SleepMilli(toggle_duration);
            SatellitePower::RestorePowerFromPanels();
            break;

        case kToggleOnPanels:
            SatellitePower::RestorePowerFromPanels();
            break;

        case kToggleOffPanels:
            SatellitePower::CutPowerFromPanels();
            break;

        default:
            // TODO(dingbenjamin): Include invalid toggle type in exception
            throw etl::exception(
                "Invalid toggle type for IoExpanderToggleUplink", __FILE__,
                __LINE__);
    }
    return true;
}

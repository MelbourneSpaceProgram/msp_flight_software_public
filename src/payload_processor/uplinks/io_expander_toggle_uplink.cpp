#include <external/etl/exception.h>
#include <src/messages/IoExpanderToggleUplinkPayload.pb.h>
#include <src/payload_processor/uplinks/io_expander_toggle_uplink.h>
#include <src/util/nanopb_utils.h>
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

    if (toggle_type == kToggleOffOn) {
        expander->SetPin(pin, false);
        TaskUtils::SleepMilli(toggle_duration);
        expander->SetPin(pin, true);
    } else if (toggle_type == kToggleOn) {
        expander->SetPin(pin, true);
    } else if (toggle_type == kToggleOff) {
        expander->SetPin(pin, false);
    } else {
        // TODO(dingbenjamin): Include invalid toggle type in exception
        throw etl::exception("Invalid toggle type for IoExpanderToggleUplink",
                             __FILE__, __LINE__);
    }
    return true;
}

#include <src/sensors/software_measurables/reset_info_container_measurable.h>

ResetInfoContainerMeasurable::ResetInfoContainerMeasurable()
    : NanopbMeasurable<ResetInfoContainerReading>(
          ResetInfoContainerReading_init_default),
      info_string("Reset info container"){};

std::string ResetInfoContainerMeasurable::GetInfoString() {
    return info_string;
}

ResetInfoContainerReading
ResetInfoContainerMeasurable::TakeDirectNanopbReading() {
    ResetInfoContainer* reset_info_container =
        ResetInfoContainer::GetInstance();
    ResetInfoContainerStruct container_struct =
        reset_info_container->GetResetInfoContainerStruct();
    ResetInfoContainerReading reading = ResetInfoContainerReading_init_default;
    reading.most_recent_reset_cause = container_struct.most_recent_reset_cause;
    reading.most_recent_reset_message =
        container_struct.most_recent_reset_message;
    reading.num_resets = container_struct.num_resets;

    for (uint8_t i = 0; i < kNumResetCauses; i++) {
        reading.num_resets_of_cause.bytes[i] =
            container_struct.num_resets_of_cause[i];
    }

    for (uint8_t i = 0; i < ResetMessage::kNumResetMessages; i++) {
        reading.num_resets_with_message.bytes[i] =
            container_struct.num_resets_with_message[i];
    }

    reading.first_wakeup = container_struct.first_wakeup;
    reading.expected_reset = container_struct.expected_reset;

    return reading;
}

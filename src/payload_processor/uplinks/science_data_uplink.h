#ifndef SRC_PAYLOAD_PROCESSOR_COMMANDS_SCIENCE_DATA_UPLINK_H_
#define SRC_PAYLOAD_PROCESSOR_COMMANDS_SCIENCE_DATA_UPLINK_H_

#include <src/database/circular_buffer_nanopb.h>
#include <src/messages/Time.pb.h>
#include <src/payload_processor/uplinks/uplink.h>
#include <src/sensors/measurable_manager.h>
#include <src/telecomms/lithium.h>
#include <src/telecomms/lithium_commands/transmit_command.h>
#include <src/telecomms/msp_payloads/science_payload.h>
#include <src/util/data_types.h>
#include <src/util/nanopb_utils.h>
#include <stdio.h>
#include <xdc/runtime/Log.h>

#define RetrieveAndSendDataMacro(NanopbMessageType)                  \
    RetrieveAndSendData<NanopbMessageType, NanopbMessageType##_size, \
                        NanopbMessageType##_fields>

class ScienceDataUplink : public Uplink {
   public:
    ScienceDataUplink(byte* payload);
    bool ExecuteUplink();

   private:
    static constexpr uint8_t kScienceUplinkArgumentLength =
        sizeof(uint16_t) + Time_size;
    static constexpr uint8_t kMeasurableIdIndex = 0;
    static constexpr uint8_t kRequestedTimeIndex = sizeof(uint16_t);

    template <typename NanopbMessageType, uint16_t NanopbMessageType_size,
              const pb_field_t* NanopbMessageType_fields>
    bool RetrieveAndSendData(uint16_t measurable_id) {
        char filename[4];
        snprintf(filename, sizeof(filename), "%03d", requested_id);

        TransmitPayload* payload;

        try {
            NanopbMessageType result;
            if (!requested_time.is_valid) {
                // Retrieve the latest reading
                result = MeasurableManager::GetInstance()
                             ->ReadNanopbMeasurable<NanopbMessageType>(
                                 measurable_id, 0, true);
            } else {
                // Retrieve a historical reading in the sd card
                result = CircularBufferNanopb(NanopbMessageType)::Search(
                    filename, requested_time.timestamp_ms);
            }
            SciencePayload<NanopbMessageType, NanopbMessageType_size,
                           NanopbMessageType_fields>
                science_payload(result);
            payload = &science_payload;
        } catch (MspException& e) {
            // TODO(dingbejamin): Be able to differentiate different exceptions
            // to determine cause of failure
            return false;
        }

        if (!Lithium::GetInstance()->Transmit(payload)) {
            Log_error0("Failed to transmit science payload");
            return false;
        }
        return true;
    }

    uint16_t requested_id;
    Time requested_time;
};

#endif  // SRC_PAYLOAD_PROCESSOR_COMMANDS_SCIENCE_DATA_UPLINK_H_

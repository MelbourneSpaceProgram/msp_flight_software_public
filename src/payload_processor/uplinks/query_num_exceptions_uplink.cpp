#include <src/payload_processor/uplinks/query_num_exceptions_uplink.h>
#include <src/telecomms/lithium.h>
#include <src/telecomms/msp_payloads/num_exceptions_payload.h>
#include <src/util/msp_exception.h>

QueryNumExceptionsUplink::QueryNumExceptionsUplink(byte* payload)
    : Uplink(kQueryNumExceptionsUplinkArgumentLength) {}

bool QueryNumExceptionsUplink::ExecuteUplink() {
    try {
        NumExceptionsPayload payload;
        return Lithium::GetInstance()->Transmit(&payload);
    } catch (etl::exception& e) {
        MspException::LogException(e);
        return false;
    }
}

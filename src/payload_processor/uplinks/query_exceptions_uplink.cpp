#include <src/payload_processor/uplinks/query_exceptions_uplink.h>
#include <src/telecomms/lithium.h>
#include <src/telecomms/msp_payloads/exceptions_payload.h>

QueryExceptionsUplink::QueryExceptionsUplink(byte* payload)
    : Uplink(kQueryExceptionsUplinkArgumentLength), error_id(*payload) {}

bool QueryExceptionsUplink::ExecuteUplink() {
    ExceptionsPayload exceptions_payload(error_id);
    return Lithium::GetInstance()->Transmit(&exceptions_payload);
}

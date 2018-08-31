#include <src/messages/rebuildable_message_field_iterator.h>
#include <src/telecomms/lithium_commands/lithium_command_codes.h>
#include <src/telecomms/lithium_commands/telemetry_query_command.h>

TelemetryQueryCommand::TelemetryQueryCommand()
    : LithiumResponseCommand(kGetTelemetryCommandCode, NULL) {}

uint16_t TelemetryQueryCommand::GetLithiumPayloadSize() const { return 0; }

uint16_t TelemetryQueryCommand::TwoBytesToUint16(byte input[2]) {
    return input[0] << 8 | input[1];
}

LithiumTelemetry TelemetryQueryCommand::GetParsedResponse() {
    LithiumTelemetry parsed_telemetry = {0, 0, {0, 0, 0}, 0, 0, 0};
    RebuildableMessageFieldIterator iterator(GetReplyBuffer(),
                                             kSerialisedTelemetrySize);

    // TODO(dingbenjamin): Ensure this code calls in the correct order despite function chaining
    iterator.FetchData<uint32_t>(&parsed_telemetry.bytes_transmitted)->
        FetchData<uint32_t>(&parsed_telemetry.bytes_received)->
        FetchData<uint8_t>(&parsed_telemetry.rssi)->
        FetchData<uint8_t>(&parsed_telemetry.time_count[2])->
        FetchData<uint8_t>(&parsed_telemetry.time_count[1])->
        FetchData<uint8_t>(&parsed_telemetry.time_count[0])->
        FetchData<int16_t>(&parsed_telemetry.msp430_temp)->
        FetchData<uint16_t>(&parsed_telemetry.op_counter);

    return parsed_telemetry;
}

const byte& TelemetryQueryCommand::GetCommandCode() const {
    return kGetTelemetryCommandCode;
}

uint16_t TelemetryQueryCommand::GetReplyPayloadSize() const {
    return kSerialisedTelemetrySize;
}

bool TelemetryQueryCommand::CheckValidTelemetry(LithiumTelemetry telemetry) {
    // TODO(dingbenjamin): Check for msp4320_temp as well, accounting for
    // signedness
    if (telemetry.op_counter == 0 && telemetry.time_count[0] == 0 &&
        telemetry.time_count[1] == 0 && telemetry.time_count[2] == 0 &&
        telemetry.rssi == 0 && telemetry.bytes_received == 0 &&
        telemetry.bytes_transmitted == 0) {
        return false;
    }
    return true;
}

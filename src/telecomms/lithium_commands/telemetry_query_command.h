#ifndef SRC_TELECOMMS_LITHIUM_COMMANDS_TELEMETRY_QUERY_COMMAND_H_
#define SRC_TELECOMMS_LITHIUM_COMMANDS_TELEMETRY_QUERY_COMMAND_H_

#include <src/telecomms/lithium_commands/lithium_command.h>
#include <src/telecomms/lithium_commands/lithium_response_command.h>
#include <src/telecomms/lithium_telemetry.h>
#include <src/util/data_types.h>

class TelemetryQueryCommand : public LithiumResponseCommand<LithiumTelemetry> {
   public:
    TelemetryQueryCommand();
    uint16_t GetLithiumPayloadSize() const;
    const byte& GetCommandCode() const;
    uint16_t GetReplyPayloadSize() const;
    LithiumTelemetry GetParsedResponse();

    static bool CheckValidTelemetry(LithiumTelemetry telemetry);

   private:
    static const uint16_t kSerialisedTelemetrySize = 16;
    uint16_t TwoBytesToUint16(byte input[2]);
};

#endif  // SRC_TELECOMMS_LITHIUM_COMMANDS_TELEMETRY_QUERY_COMMAND_H_

#include <src/messages/serialised_message_builder.h>
#include <src/telecomms/msp_payloads/limp_mode_beacon_payload.h>
#include <src/util/data_types.h>
#include <src/util/message_codes.h>

std::string LimpModeBeaconPayload::kLimpModeMessage = "Satellite is fucked";

LimpModeBeaconPayload::LimpModeBeaconPayload() {}

SerialisedMessage LimpModeBeaconPayload::SerialiseTo(byte* serial_buffer) const {
    SerialisedMessageBuilder builder(serial_buffer, GetSerialisedSize());

	uint8_t char_index = 0;
	while (builder.GetSerialisedLength() != GetSerialisedSize()) {
		if (char_index >= kLimpModeMessage.length()) {
			char_index = 0;
		}
        builder.AddData<char>(kLimpModeMessage[char_index]);
		char_index++;
	}
    return builder.Build();
}

uint16_t LimpModeBeaconPayload::GetSerialisedSize() const { return kPayloadSize; }

uint8_t LimpModeBeaconPayload::GetPayloadCode() const { return kLimpModeBeaconPayloadCode; }

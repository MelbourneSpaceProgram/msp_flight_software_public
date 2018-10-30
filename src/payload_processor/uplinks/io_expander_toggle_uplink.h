#ifndef SRC_PAYLOAD_PROCESSOR_COMMANDS_IO_EXPANDER_TOGGLE_UPLINK_H_
#define SRC_PAYLOAD_PROCESSOR_COMMANDS_IO_EXPANDER_TOGGLE_UPLINK_H_

#include <src/board/i2c/multiplexers/i2c_multiplexer.h>
#include <src/messages/IoExpanderToggleUplinkPayload.pb.h>
#include <src/util/data_types.h>
#include <src/board/i2c/io_expander/io_expander.h>
#include <src/payload_processor/uplinks/uplink.h>

class IoExpanderToggleUplink : public Uplink {
   public:
    enum ToggleType { kToggleOffOn = 0, kToggleOn = 1, kToggleOff = 2 };

    explicit IoExpanderToggleUplink(byte* payload);
    bool ExecuteUplink();

   private:
    IoExpanderToggleUplinkPayload decoded_command;
    uint8_t io_expander_id;
	IoExpander::IoPin pin;
    ToggleType toggle_type;
    uint16_t toggle_duration;
};

#endif  // SRC_PAYLOAD_PROCESSOR_COMMANDS_IO_EXPANDER_TOGGLE_UPLINK_H_

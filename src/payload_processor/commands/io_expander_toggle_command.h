#ifndef SRC_PAYLOAD_PROCESSOR_COMMANDS_IO_EXPANDER_TOGGLE_COMMAND_H_
#define SRC_PAYLOAD_PROCESSOR_COMMANDS_IO_EXPANDER_TOGGLE_COMMAND_H_

#include <src/board/i2c/multiplexers/i2c_multiplexer.h>
#include <src/messages/IoExpanderToggleCommandPayload.pb.h>
#include <src/payload_processor/commands/command.h>
#include <src/util/data_types.h>
#include <src/board/i2c/io_expander/io_expander.h>

class IoExpanderToggleCommand : public Command {
   public:
    enum ToggleType { kToggleOffOn = 0, kToggleOn = 1, kToggleOff = 2 };

    explicit IoExpanderToggleCommand(byte* payload);
    bool ExecuteCommand();

   private:
    IoExpanderToggleCommandPayload decoded_command;
    uint8_t io_expander_id;
	IoExpander::IoPin pin;
    ToggleType toggle_type;
    uint16_t toggle_duration;
};

#endif  // SRC_PAYLOAD_PROCESSOR_COMMANDS_IO_EXPANDER_TOGGLE_COMMAND_H_

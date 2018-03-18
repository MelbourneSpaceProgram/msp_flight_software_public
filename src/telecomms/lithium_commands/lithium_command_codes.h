#ifndef SRC_TELECOMMS_LITHIUM_COMMANDS_LITHIUM_COMMAND_CODES_H_
#define SRC_TELECOMMS_LITHIUM_COMMANDS_LITHIUM_COMMAND_CODES_H_

#include <src/util/data_types.h>

// TODO(dingbenjamin): Figure out why including these in the classes themselves
// results in a linker error

const byte kNoOpCommandCode = 0x01;
const byte kResetSystemCommandCode = 0x02;
const byte kTransmitCommandCode = 0x03;
const byte kReceivedDataCode = 0x04;
const byte kGetConfigurationCommandCode = 0x05;
const byte kWriteFlashCommandCode = 0x08;

#endif  // SRC_TELECOMMS_LITHIUM_COMMANDS_LITHIUM_COMMAND_CODES_H_

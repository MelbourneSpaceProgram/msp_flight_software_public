#ifndef SRC_TELECOMMS_LITHIUM_COMMANDS_LITHIUM_COMMAND_CODES_H_
#define SRC_TELECOMMS_LITHIUM_COMMANDS_LITHIUM_COMMAND_CODES_H_

#include <src/util/data_types.h>

const byte kNoOpCommandCode = 0x01;
const byte kResetSystemCommandCode = 0x02;
const byte kTransmitCommandCode = 0x03;
const byte kReceivedDataCode = 0x04;
const byte kGetConfigurationCommandCode = 0x05;
const byte kSetConfigurationCommandCode = 0x06;
const byte kGetTelemetryCommandCode = 0x07;
const byte kWriteFlashCommandCode = 0x08;
const byte kRfConfigureCommandCode = 0x09;
const byte kSetBeaconDataCommandCode = 0x10;
const byte kConfigureBeaconCommandCode = 0x11;
const byte kGetFirmwareRevCommandCode = 0x12;
const byte kDioKeyWriteCommandCode = 0x13;
const byte kFastSetPaCommandCode = 0x20;

#endif  // SRC_TELECOMMS_LITHIUM_COMMANDS_LITHIUM_COMMAND_CODES_H_

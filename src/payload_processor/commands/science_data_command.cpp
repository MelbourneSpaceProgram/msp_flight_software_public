#include <external/etl/exception.h>
#include <external/nanopb/pb_decode.h>
#include <src/database/circular_buffer_nanopb.h>
#include <src/messages/AccelerometerReading.pb.h>
#include <src/messages/BmsReadings.pb.h>
#include <src/messages/CurrentReading.pb.h>
#include <src/messages/GyroscopeReading.pb.h>
#include <src/messages/MagnetometerReading.pb.h>
#include <src/messages/StateMachineStateReading.pb.h>
#include <src/messages/TemperatureReading.pb.h>
#include <src/messages/TorqueOutputReading.pb.h>
#include <src/messages/VoltageReading.pb.h>
#include <src/payload_processor/commands/science_data_command.h>
#include <src/sensors/i2c_measurable_manager.h>
#include <src/sensors/measurable_id.h>
#include <xdc/runtime/Log.h>

ScienceDataCommand::ScienceDataCommand(byte* payload)
    : Command(kScienceCommandArgumentLength),
      requested_id((payload[kMeasurableIdIndex]
                    << 8) |  // TODO(dingbenjamin): Check endianness
                   payload[kMeasurableIdIndex + 1]),
      requested_time(NanopbDecode(Time)(payload + kRequestedTimeIndex)) {}

bool ScienceDataCommand::ExecuteCommand() {
    if (requested_id >= kMeasurableIdEnd) {
        Log_error1("Invalid measurable ID %d requested by Science Data Command",
                   requested_id);
        return false;
    }
    // TODO(dingbenjamin): Check if the id has a corresponding measurable that
    // is not null in the manager

    // Try all the different NanopbMessageTypes and use the one that works
    if (RetrieveAndSendDataMacro(VoltageReading)(requested_id)) return true;
    if (RetrieveAndSendDataMacro(TemperatureReading)(requested_id)) return true;
    if (RetrieveAndSendDataMacro(CurrentReading)(requested_id)) return true;
    if (RetrieveAndSendDataMacro(AccelerometerReading)(requested_id))
        return true;
    if (RetrieveAndSendDataMacro(GyroscopeReading)(requested_id)) return true;
    if (RetrieveAndSendDataMacro(MagnetometerReading)(requested_id))
        return true;
    if (RetrieveAndSendDataMacro(TorqueOutputReading)(requested_id))
        return true;
    if (RetrieveAndSendDataMacro(BmsReadings)(requested_id)) return true;
    if (RetrieveAndSendDataMacro(StateMachineStateReading)(requested_id))
        return true;

    Log_error1(
        "Could not find the correct nanopb message type/retrieve the nanopb "
        "for message %d",
        requested_id);
    return false;
}

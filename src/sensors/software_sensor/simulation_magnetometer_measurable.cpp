#include <external/nanopb/pb_decode.h>
#include <external/nanopb/pb_encode.h>
#include <src/board/debug_interface/debug_stream.h>
#include <src/messages/MagnetometerReading.pb.h>
#include <src/sensors/software_sensor/simulation_magnetometer_measurable.h>
#include <src/util/message_codes.h>
#include <src/util/nanopb_utils.h>

SimulationMagnetometerMeasurable::SimulationMagnetometerMeasurable()
    : NanopbMeasurable<MagnetometerReading>(MagnetometerReading_init_default) {}

MagnetometerReading
SimulationMagnetometerMeasurable::TakeDirectNanopbReading() {
    DebugStream* debug_stream = DebugStream::GetInstance();
    uint8_t buffer[MagnetometerReading_size];

    bool success = debug_stream->RequestMessageFromSimulator(
        kMagnetometerReadingRequestCode, buffer, MagnetometerReading_size);

    if (success) {
        last_reading = NanopbDecode(MagnetometerReading)(buffer);
    } else {
        etl::exception e("Failed to request message from simulator", __FILE__,
                         __LINE__);
        throw e;
    }
    return last_reading;
}

std::string SimulationMagnetometerMeasurable::GetInfoString() {
    return "simulation magnetometer";
}

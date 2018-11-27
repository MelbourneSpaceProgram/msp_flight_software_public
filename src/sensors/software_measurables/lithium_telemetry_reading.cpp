#include <src/sensors/software_measurables/lithium_telemetry_measurable.h>
#include <src/telecomms/lithium.h>
#include <src/telecomms/lithium_telemetry.h>

LithiumTelemetryMeasurable::LithiumTelemetryMeasurable()
    : NanopbMeasurable<LithiumTelemetryReading>(
          LithiumTelemetryReading_init_default),
      info_string("Lithium telemetry"){};

std::string LithiumTelemetryMeasurable::GetInfoString() { return info_string; }

LithiumTelemetryReading LithiumTelemetryMeasurable::TakeDirectNanopbReading() {
    Lithium* lithium = Lithium::GetInstance();
    LithiumTelemetry telemetry;
    lithium->DoTelemetryQuery(telemetry);

    LithiumTelemetryReading reading;
    reading.op_counter = telemetry.op_counter;
    reading.msp430_temp = telemetry.msp430_temp;
    for(uint8_t i = 0; i < kNumTimeCountBytes; i++) {
        reading.time_count.bytes[i] =telemetry.time_count[i];
    }
    reading.rssi = telemetry.rssi;
    reading.bytes_received = telemetry.bytes_transmitted;

    return reading;
}

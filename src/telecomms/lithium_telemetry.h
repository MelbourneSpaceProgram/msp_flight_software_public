#ifndef SRC_TELECOMMS_LITHIUM_TELEMETRY_H_
#define SRC_TELECOMMS_LITHIUM_TELEMETRY_H_

// See Lithium Radio Interface Datasheet
typedef struct LithiumTelemetry {
    uint16_t op_counter;
    int16_t msp430_temp;
    uint8_t time_count[3];
    uint8_t rssi;
    uint32_t bytes_received;
    uint32_t bytes_transmitted;
} LithiumTelemetry;

#endif  // SRC_TELECOMMS_LITHIUM_TELEMETRY_H_

#include <external/etl/exception.h>
#include <src/board/uart/uart.h>
#include <src/config/unit_tests.h>
#include <src/messages/AccelerometerReading.pb.h>
#include <src/messages/BmsReadings.pb.h>
#include <src/messages/CurrentReading.pb.h>
#include <src/messages/GyroscopeReading.pb.h>
#include <src/messages/MagnetometerReading.pb.h>
#include <src/messages/TemperatureReading.pb.h>
#include <src/messages/VoltageReading.pb.h>
#include <src/sensors/i2c_sensors/measurables/temperature_measurable.h>
#include <src/sensors/measurable_id.h>
#include <src/system/runnable_system_health_check.h>
#include <src/telecomms/lithium.h>
#include <src/util/satellite_time_source.h>
#include <src/util/system_watchdog.h>
#include <src/util/task_utils.h>
#include <ti/drivers/utils/RingBuf.h>

Uart* RunnableSystemHealthCheck::debug_uart = NULL;
bool RunnableSystemHealthCheck::datalogger_enabled = true;

// These are created outside of the class to make getting C linkage easier
extern "C" {
RingBuf_Object ring_buffer;
byte buffer[5000];
}

void RunnableSystemHealthCheck::Init() {
    RingBuf_construct(&ring_buffer, buffer, sizeof(buffer));
}

RunnableSystemHealthCheck::RunnableSystemHealthCheck(Uart* debug_uart) {
    if (RunnableSystemHealthCheck::debug_uart == NULL) {
        // WARNING: This task should only ever WRITE to the debug UART
        RunnableSystemHealthCheck::debug_uart = debug_uart;
    } else {
        throw etl::exception(
            "Only one instance of RunnableSystemHealthCheck should ever be "
            "instantiated",
            __FILE__, __LINE__);
    }
}

fnptr RunnableSystemHealthCheck::GetRunnablePointer() {
    return &RunnableSystemHealthCheck::SystemHealthCheck;
}

void RunnableSystemHealthCheck::WriteToDataLogger(uint8_t measurable_id,
                                                  byte encoded_message[],
                                                  uint8_t message_size) {
    byte packet[5] = {NULL};
    packet[0] = kMeasurableLoggerSyncChar1;
    packet[1] = kMeasurableLoggerSyncChar2;
    packet[2] = message_size;  // Length of packet except header
    packet[3] = measurable_id;
    packet[4] = 0x00;  // TODO(dingbenjamin): Implement checksum

    debug_uart->PerformWriteTransaction(packet, 5);
    debug_uart->PerformWriteTransaction(encoded_message, message_size);
}

bool RunnableSystemHealthCheck::IsEnabled() { return datalogger_enabled; }

void RunnableSystemHealthCheck::EnableDatalogger(bool enable_logger) {
    datalogger_enabled = enable_logger;
}

void RunnableSystemHealthCheck::SystemHealthCheck(uintptr_t arg1,
                                                  uintptr_t arg2) {
    while (1) {
        if (datalogger_enabled) {
            if (kTcomBoardAvailable) {
                LogMeasurableMacro(VoltageReading)(kComInV1);
                LogMeasurableMacro(VoltageReading)(kComOutV1);
                LogMeasurableMacro(VoltageReading)(kComInV2);
                LogMeasurableMacro(VoltageReading)(kComOutV2);
                LogMeasurableMacro(CurrentReading)(kComInI1);
                LogMeasurableMacro(CurrentReading)(kComOutI1);
                LogMeasurableMacro(CurrentReading)(kComInI2);
                LogMeasurableMacro(CurrentReading)(kComOutI2);
                LogMeasurableMacro(TemperatureReading)(kComT1);
                LogMeasurableMacro(TemperatureReading)(kComT2);
            }

            if (kEpsBoardAvailable) {
                LogMeasurableMacro(CurrentReading)(kEpsBoostInI1);
                LogMeasurableMacro(CurrentReading)(kEpsLoadI1);
                LogMeasurableMacro(CurrentReading)(kEpsBoostInI2);
                LogMeasurableMacro(CurrentReading)(kEpsLoadI2);
                LogMeasurableMacro(TemperatureReading)(kEpsT1);
                LogMeasurableMacro(TemperatureReading)(kEpsT2);
                LogMeasurableMacro(TemperatureReading)(kEpsBmsDieT1);
                LogMeasurableMacro(TemperatureReading)(kEpsBmsDieT2);
                LogMeasurableMacro(TemperatureReading)(kEpsBmsBatT1);
                LogMeasurableMacro(TemperatureReading)(kEpsBmsBatT2);
                LogMeasurableMacro(VoltageReading)(kEpsAdcBatV1);
                LogMeasurableMacro(VoltageReading)(kEpsBoostOutV1);
                LogMeasurableMacro(VoltageReading)(kEpsAdcBatV2);
                LogMeasurableMacro(VoltageReading)(kEpsBoostOutV2);
                LogMeasurableMacro(VoltageReading)(kEpsRail1);
                LogMeasurableMacro(VoltageReading)(kEpsRail2);
                LogMeasurableMacro(BmsReadings)(kEpsBmsReadings1);
                LogMeasurableMacro(BmsReadings)(kEpsBmsReadings2);
            }

            if (kXPosAvailable) {
                LogMeasurableMacro(VoltageReading)(kXPosV);
                LogMeasurableMacro(VoltageReading)(kXPosSolarV);
                LogMeasurableMacro(CurrentReading)(kXPosI);
                LogMeasurableMacro(CurrentReading)(kXPosSolarI);
                LogMeasurableMacro(TemperatureReading)(kXPosT1);
                LogMeasurableMacro(TemperatureReading)(kXPosT2);
            }

            if (kYPosAvailable) {
                LogMeasurableMacro(VoltageReading)(kYPosV);
                LogMeasurableMacro(VoltageReading)(kYPosSolarV);
                LogMeasurableMacro(CurrentReading)(kYPosI);
                LogMeasurableMacro(CurrentReading)(kYPosSolarI);
                LogMeasurableMacro(TemperatureReading)(kYPosT1);
                LogMeasurableMacro(TemperatureReading)(kYPosT2);
            }

            if (kZPosAvailable) {
                LogMeasurableMacro(VoltageReading)(kEpsTopPanelV);
                LogMeasurableMacro(VoltageReading)(kEpsTopSolarV);
                LogMeasurableMacro(CurrentReading)(kEpsTopPanelI);
                LogMeasurableMacro(CurrentReading)(kEpsTopSolarI);
                LogMeasurableMacro(TemperatureReading)(kZPosT);
            }

            if (kXNegAvailable) {
                LogMeasurableMacro(VoltageReading)(kXNegV);
                LogMeasurableMacro(VoltageReading)(kXNegSolarV);
                LogMeasurableMacro(CurrentReading)(kXNegI);
                LogMeasurableMacro(CurrentReading)(kXNegSolarI);
                LogMeasurableMacro(TemperatureReading)(kXNegT1);
                LogMeasurableMacro(TemperatureReading)(kXNegT2);
            }

            if (kYNegAvailable) {
                LogMeasurableMacro(VoltageReading)(kYNegV);
                LogMeasurableMacro(VoltageReading)(kYNegSolarV);
                LogMeasurableMacro(CurrentReading)(kYNegI);
                LogMeasurableMacro(CurrentReading)(kYNegSolarI);
                LogMeasurableMacro(TemperatureReading)(kYNegT1);
                LogMeasurableMacro(TemperatureReading)(kYNegT2);
            }

            if (kZNegAvailable) {
                LogMeasurableMacro(VoltageReading)(kZNegV);
                LogMeasurableMacro(VoltageReading)(kZNegSolarV);
                LogMeasurableMacro(CurrentReading)(kZNegI);
                LogMeasurableMacro(CurrentReading)(kZNegSolarI);
                LogMeasurableMacro(TemperatureReading)(kZNegT1);
                LogMeasurableMacro(TemperatureReading)(kZNegT2);
            }

            if (kFsBoardAvailable) {
                LogMeasurableMacro(CurrentReading)(kFsTorquerXI);
                LogMeasurableMacro(CurrentReading)(kFsTorquerTotalI);
                LogMeasurableMacro(CurrentReading)(kFsTorquerYI);
                LogMeasurableMacro(CurrentReading)(kFsTorquerZI);
                LogMeasurableMacro(VoltageReading)(kFsTorquerXAV);
                LogMeasurableMacro(VoltageReading)(kFsTorquerXBV);
                LogMeasurableMacro(VoltageReading)(kFsTorquerYAV);
                LogMeasurableMacro(VoltageReading)(kFsTorquerYBV);
                LogMeasurableMacro(VoltageReading)(kFsTorquerZAV);
                LogMeasurableMacro(VoltageReading)(kFsTorquerZBV);

                LogMeasurableMacro(TemperatureReading)(kFsHbXT);
                LogMeasurableMacro(TemperatureReading)(kFsHbYT);
                LogMeasurableMacro(TemperatureReading)(kFsHbZT);
                LogMeasurableMacro(GyroscopeReading)(kFsImuGyro1);
                LogMeasurableMacro(AccelerometerReading)(kFsImuAccel1);
                LogMeasurableMacro(TemperatureReading)(kFsImuT1);
                LogMeasurableMacro(MagnetometerReading)(kFsImuMagno1);
                LogMeasurableMacro(GyroscopeReading)(kFsImuGyro2);
                LogMeasurableMacro(AccelerometerReading)(kFsImuAccel2);
                LogMeasurableMacro(TemperatureReading)(kFsImuT2);
                LogMeasurableMacro(MagnetometerReading)(kFsImuMagno2);
            }

            if (kCdhBoardAvailable) {
                LogMeasurableMacro(TemperatureReading)(kCdhT);
            }

            if (kUtilitiesBoardAvailable) {
                LogMeasurableMacro(TemperatureReading)(kUtilT);
                LogMeasurableMacro(VoltageReading)(kUtilHeatV);
            }
        }
        UartFlush();
        SystemWatchdog::ResetTimer();
        TaskUtils::SleepMilli(kHealthCheckPeriodMillis);
    }
}

void UartPutch(Char ch) { RingBuf_put(&ring_buffer, ch); }

void UartFlush() {
    // Send 100 bytes at a time, or less
    byte encoded_message[100];

    while (RingBuf_getCount(&ring_buffer)) {
        int bytes_available = RingBuf_getCount(&ring_buffer);
        if (bytes_available == -1) {
            continue;
        }

        uint32_t byte_counter = 0;
        while (byte_counter < 100 && byte_counter < bytes_available) {
            RingBuf_get(&ring_buffer, encoded_message + byte_counter);
            byte_counter++;
        }

        RunnableSystemHealthCheck::WriteToDataLogger(
            kConsoleMessage, encoded_message, byte_counter);
    }
}

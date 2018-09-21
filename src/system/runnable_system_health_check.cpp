#include <external/etl/exception.h>
#include <src/board/uart/uart.h>
#include <src/messages/AccelerometerReading.pb.h>
#include <src/messages/CurrentReading.pb.h>
#include <src/messages/GyroscopeReading.pb.h>
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

void RunnableSystemHealthCheck::SystemHealthCheck() {
    while (1) {
        if (datalogger_enabled) {
            // Telecomms

            LogMeasurableMacro(double, VoltageReading)(
                kComInI1, &NanopbUtils::NanopbVoltage);

            LogMeasurableMacro(double, VoltageReading)(
                kComOutV1, &NanopbUtils::NanopbVoltage);
            LogMeasurableMacro(double, VoltageReading)(
                kComInV2, &NanopbUtils::NanopbVoltage);
            LogMeasurableMacro(double, VoltageReading)(
                kComOutV2, &NanopbUtils::NanopbVoltage);
            LogMeasurableMacro(double, CurrentReading)(
                kComInI1, &NanopbUtils::NanopbCurrent);
            LogMeasurableMacro(double, CurrentReading)(
                kComOutI1, &NanopbUtils::NanopbCurrent);
            LogMeasurableMacro(double, CurrentReading)(
                kComInI2, &NanopbUtils::NanopbCurrent);
            LogMeasurableMacro(double, CurrentReading)(
                kComOutI2, &NanopbUtils::NanopbCurrent);
            LogMeasurableMacro(double, TemperatureReading)(
                kComT1, &NanopbUtils::NanopbTemperature);

            // EPS
            LogMeasurableMacro(double, CurrentReading)(
                kEpsBoostInI1, &NanopbUtils::NanopbCurrent);
            LogMeasurableMacro(double, CurrentReading)(
                kEpsLoadI1, &NanopbUtils::NanopbCurrent);
            LogMeasurableMacro(double, CurrentReading)(
                kEpsBoostInI2, &NanopbUtils::NanopbCurrent);
            LogMeasurableMacro(double, CurrentReading)(
                kEpsLoadI2, &NanopbUtils::NanopbCurrent);
            LogMeasurableMacro(double, TemperatureReading)(
                kEpsT1, &NanopbUtils::NanopbTemperature);
            LogMeasurableMacro(double, TemperatureReading)(
                kEpsT2, &NanopbUtils::NanopbTemperature);
            LogMeasurableMacro(double, TemperatureReading)(
                kEpsBmsDieT1, &NanopbUtils::NanopbTemperature);
            LogMeasurableMacro(double, TemperatureReading)(
                kEpsBmsDieT2, &NanopbUtils::NanopbTemperature);
            LogMeasurableMacro(double, TemperatureReading)(
                kEpsBmsBatT1, &NanopbUtils::NanopbTemperature);
            LogMeasurableMacro(double, TemperatureReading)(
                kEpsBmsBatT2, &NanopbUtils::NanopbTemperature);
            LogMeasurableMacro(double, VoltageReading)(
                kEpsAdcBatV1, &NanopbUtils::NanopbVoltage);
            LogMeasurableMacro(double, VoltageReading)(
                kEpsBoostOutV1, &NanopbUtils::NanopbVoltage);

            LogMeasurableMacro(double, TemperatureReading)(
                kXPosT1, &NanopbUtils::NanopbTemperature);
            LogMeasurableMacro(double, TemperatureReading)(
                kXPosT2, &NanopbUtils::NanopbTemperature);
            LogMeasurableMacro(double, TemperatureReading)(
                kYPosT1, &NanopbUtils::NanopbTemperature);
            LogMeasurableMacro(double, TemperatureReading)(
                kYPosT2, &NanopbUtils::NanopbTemperature);
            LogMeasurableMacro(double, TemperatureReading)(
                kXNegT1, &NanopbUtils::NanopbTemperature);
            LogMeasurableMacro(double, TemperatureReading)(
                kXNegT2, &NanopbUtils::NanopbTemperature);
            LogMeasurableMacro(double, TemperatureReading)(
                kYNegT1, &NanopbUtils::NanopbTemperature);
            LogMeasurableMacro(double, TemperatureReading)(
                kYNegT2, &NanopbUtils::NanopbTemperature);
            LogMeasurableMacro(double, TemperatureReading)(
                kZNegT1, &NanopbUtils::NanopbTemperature);
            LogMeasurableMacro(double, TemperatureReading)(
                kZNegT2, &NanopbUtils::NanopbTemperature);
            LogMeasurableMacro(double, TemperatureReading)(
                kZPosT, &NanopbUtils::NanopbTemperature);

            LogMeasurableMacro(double, VoltageReading)(
                kXPosV, &NanopbUtils::NanopbVoltage);
            LogMeasurableMacro(double, VoltageReading)(
                kXPosSolarV, &NanopbUtils::NanopbVoltage);
            LogMeasurableMacro(double, VoltageReading)(
                kYPosV, &NanopbUtils::NanopbVoltage);
            LogMeasurableMacro(double, VoltageReading)(
                kYPosSolarV, &NanopbUtils::NanopbVoltage);
            LogMeasurableMacro(double, VoltageReading)(
                kXNegV, &NanopbUtils::NanopbVoltage);
            LogMeasurableMacro(double, VoltageReading)(
                kXNegSolarV, &NanopbUtils::NanopbVoltage);
            LogMeasurableMacro(double, VoltageReading)(
                kYNegV, &NanopbUtils::NanopbVoltage);
            LogMeasurableMacro(double, VoltageReading)(
                kYNegSolarV, &NanopbUtils::NanopbVoltage);
            LogMeasurableMacro(double, VoltageReading)(
                kZNegV, &NanopbUtils::NanopbVoltage);
            LogMeasurableMacro(double, VoltageReading)(
                kZNegSolarV, &NanopbUtils::NanopbVoltage);
            LogMeasurableMacro(double, VoltageReading)(
                kEpsTopPanelV, &NanopbUtils::NanopbVoltage);
            LogMeasurableMacro(double, VoltageReading)(
                kEpsTopSolarV, &NanopbUtils::NanopbVoltage);

            LogMeasurableMacro(double, CurrentReading)(
                kXPosI, &NanopbUtils::NanopbCurrent);
            LogMeasurableMacro(double, CurrentReading)(
                kXPosSolarI, &NanopbUtils::NanopbCurrent);
            LogMeasurableMacro(double, CurrentReading)(
                kYPosI, &NanopbUtils::NanopbCurrent);
            LogMeasurableMacro(double, CurrentReading)(
                kYPosSolarI, &NanopbUtils::NanopbCurrent);
            LogMeasurableMacro(double, CurrentReading)(
                kXNegI, &NanopbUtils::NanopbCurrent);
            LogMeasurableMacro(double, CurrentReading)(
                kXNegSolarI, &NanopbUtils::NanopbCurrent);
            LogMeasurableMacro(double, CurrentReading)(
                kYNegI, &NanopbUtils::NanopbCurrent);
            LogMeasurableMacro(double, CurrentReading)(
                kYNegSolarI, &NanopbUtils::NanopbCurrent);
            LogMeasurableMacro(double, CurrentReading)(
                kZNegI, &NanopbUtils::NanopbCurrent);
            LogMeasurableMacro(double, CurrentReading)(
                kZNegSolarI, &NanopbUtils::NanopbCurrent);
            LogMeasurableMacro(double, CurrentReading)(
                kEpsTopPanelI, &NanopbUtils::NanopbCurrent);
            LogMeasurableMacro(double, CurrentReading)(
                kEpsTopSolarI, &NanopbUtils::NanopbCurrent);

            // TODO(akremor): Keeping these as they represent uninitialised
            // measurables
            // LogToUart(double, CurrentReading)(kEpsBoostInI1,
            //                                         &NanopbUtils::NanopbCurrent);
            // LogToUart(double, CurrentReading)(kEpsLoadI1,
            //                                         &NanopbUtils::NanopbCurrent);
            LogMeasurableMacro(double, VoltageReading)(
                kEpsAdcBatV2, &NanopbUtils::NanopbVoltage);
            LogMeasurableMacro(double, VoltageReading)(
                kEpsBoostOutV2, &NanopbUtils::NanopbVoltage);
            // LogToUart(double, CurrentReading)(kEpsBoostInI2,
            //                                        &NanopbUtils::NanopbCurrent);
            // LogToUart(double, CurrentReading)(kEpsLoadI2,
            //                                         &NanopbUtils::NanopbCurrent);
            LogMeasurableMacro(double, VoltageReading)(
                kEpsRail1, &NanopbUtils::NanopbVoltage);
            LogMeasurableMacro(double, VoltageReading)(
                kEpsRail2, &NanopbUtils::NanopbVoltage);
            LogMeasurableMacro(BmsReadings, BmsReadings)(
                kEpsBmsReadings1, &NanopbUtils::NanopbBms);
            LogMeasurableMacro(BmsReadings, BmsReadings)(
                kEpsBmsReadings2, &NanopbUtils::NanopbBms);

            // Flight Systems
            LogMeasurableMacro(double, CurrentReading)(
                kFsTorquerXI, &NanopbUtils::NanopbCurrent);
            LogMeasurableMacro(double, CurrentReading)(
                kFsTorquerTotalI, &NanopbUtils::NanopbCurrent);
            LogMeasurableMacro(double, CurrentReading)(
                kFsTorquerYI, &NanopbUtils::NanopbCurrent);
            LogMeasurableMacro(double, CurrentReading)(
                kFsTorquerZI, &NanopbUtils::NanopbCurrent);
            LogMeasurableMacro(double, VoltageReading)(
                kFsTorquerXAV, &NanopbUtils::NanopbVoltage);
            LogMeasurableMacro(double, VoltageReading)(
                kFsTorquerXBV, &NanopbUtils::NanopbVoltage);
            LogMeasurableMacro(double, VoltageReading)(
                kFsTorquerYAV, &NanopbUtils::NanopbVoltage);
            LogMeasurableMacro(double, VoltageReading)(
                kFsTorquerYBV, &NanopbUtils::NanopbVoltage);
            LogMeasurableMacro(double, VoltageReading)(
                kFsTorquerZAV, &NanopbUtils::NanopbVoltage);
            LogMeasurableMacro(double, VoltageReading)(
                kFsTorquerZBV, &NanopbUtils::NanopbVoltage);

            LogMeasurableMacro(double, TemperatureReading)(
                kFsHbXT, &NanopbUtils::NanopbTemperature);
            LogMeasurableMacro(double, TemperatureReading)(
                kFsHbYT, &NanopbUtils::NanopbTemperature);
            LogMeasurableMacro(double, TemperatureReading)(
                kFsHbZT, &NanopbUtils::NanopbTemperature);
            LogMeasurableMacro(GyroscopeReading, GyroscopeReading)(
                kFsImuGyro1, &NanopbUtils::NanopbGyroscope);
            LogMeasurableMacro(AccelerometerReading, AccelerometerReading)(
                kFsImuAccel1, &NanopbUtils::NanopbAccelerometer);
            LogMeasurableMacro(double, TemperatureReading)(
                kFsImuT1, &NanopbUtils::NanopbTemperature);
            LogMeasurableMacro(MagnetometerReading, MagnetometerReading)(
                kFsImuMagno1, &NanopbUtils::NanopbMagnetometer);
            LogMeasurableMacro(GyroscopeReading, GyroscopeReading)(
                kFsImuGyro2, &NanopbUtils::NanopbGyroscope);
            LogMeasurableMacro(AccelerometerReading, AccelerometerReading)(
                kFsImuAccel2, &NanopbUtils::NanopbAccelerometer);
            LogMeasurableMacro(double, TemperatureReading)(
                kFsImuT2, &NanopbUtils::NanopbTemperature);
            LogMeasurableMacro(MagnetometerReading, MagnetometerReading)(
                kFsImuMagno2, &NanopbUtils::NanopbMagnetometer);

            // CDH
            LogMeasurableMacro(double, TemperatureReading)(
                kCdhT, &NanopbUtils::NanopbTemperature);

            // Utilities
            LogMeasurableMacro(double, TemperatureReading)(
                kUtilT, &NanopbUtils::NanopbTemperature);
            LogMeasurableMacro(double, VoltageReading)(
                kUtilHeatV, &NanopbUtils::NanopbVoltage);
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

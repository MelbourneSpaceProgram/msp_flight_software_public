#include <src/board/uart/uart.h>
#include <src/messages/AccelerometerReading.pb.h>
#include <src/messages/CurrentReading.pb.h>
#include <src/messages/GyroscopeReading.pb.h>
#include <src/messages/TemperatureReading.pb.h>
#include <src/messages/VoltageReading.pb.h>
#include <src/sensors/i2c_sensors/measurables/temperature_measurable.h>
#include <src/sensors/measurable_id.h>
#include <src/system/runnable_system_health_check.h>
#include <src/util/satellite_time_source.h>
#include <src/util/system_watchdog.h>
#include <src/util/task_utils.h>

Uart RunnableSystemHealthCheck::debug_uart(UMBILICAL_CONSOLE);

RunnableSystemHealthCheck::RunnableSystemHealthCheck() {
    debug_uart.SetBaudRate(Uart::kBaud115200)
        ->SetReadTimeout(TaskUtils::MilliToCycles(1000))
        ->SetWriteTimeout(TaskUtils::MilliToCycles(1000))
        ->Open();
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
    packet[2] =
        message_size + 3;  // Length of packet (this, ID, checksum, payload)
    packet[3] = measurable_id;
    packet[4] = 0x00;  // Checksum

    debug_uart.PerformWriteTransaction(packet, 5);
    debug_uart.PerformWriteTransaction(encoded_message, message_size);
}

void RunnableSystemHealthCheck::SystemHealthCheck() {
    while (1) {
        // Telecomms

        LogToUart(double, VoltageReading)(kComInI1,
                                          &NanopbUtils::NanopbVoltage);

        LogToUart(double, VoltageReading)(kComOutV1,
                                          &NanopbUtils::NanopbVoltage);
        LogToUart(double, VoltageReading)(kComInV2,
                                          &NanopbUtils::NanopbVoltage);
        LogToUart(double, VoltageReading)(kComOutV2,
                                          &NanopbUtils::NanopbVoltage);
        LogToUart(double, CurrentReading)(kComInI1,
                                          &NanopbUtils::NanopbCurrent);
        LogToUart(double, CurrentReading)(kComOutI1,
                                          &NanopbUtils::NanopbCurrent);
        LogToUart(double, CurrentReading)(kComInI2,
                                          &NanopbUtils::NanopbCurrent);
        LogToUart(double, CurrentReading)(kComOutI2,
                                          &NanopbUtils::NanopbCurrent);
        LogToUart(double, TemperatureReading)(kComT1,
                                              &NanopbUtils::NanopbTemperature);

        // EPS
        LogToUart(double, CurrentReading)(kEpsBoostInI1,
                                          &NanopbUtils::NanopbCurrent);
        LogToUart(double, CurrentReading)(kEpsLoadI1,
                                          &NanopbUtils::NanopbCurrent);
        LogToUart(double, CurrentReading)(kEpsBoostInI2,
                                          &NanopbUtils::NanopbCurrent);
        LogToUart(double, CurrentReading)(kEpsLoadI2,
                                          &NanopbUtils::NanopbCurrent);
        LogToUart(double, TemperatureReading)(kEpsT1,
                                              &NanopbUtils::NanopbTemperature);
        LogToUart(double, TemperatureReading)(kEpsT2,
                                              &NanopbUtils::NanopbTemperature);
        LogToUart(double, TemperatureReading)(kEpsBmsDieT1,
                                              &NanopbUtils::NanopbTemperature);
        LogToUart(double, TemperatureReading)(kEpsBmsDieT2,
                                              &NanopbUtils::NanopbTemperature);
        LogToUart(double, TemperatureReading)(kEpsBmsBatT1,
                                              &NanopbUtils::NanopbTemperature);
        LogToUart(double, TemperatureReading)(kEpsBmsBatT2,
                                              &NanopbUtils::NanopbTemperature);
        LogToUart(double, VoltageReading)(kEpsAdcBatV1,
                                          &NanopbUtils::NanopbVoltage);
        LogToUart(double, VoltageReading)(kEpsBoostOutV1,
                                          &NanopbUtils::NanopbVoltage);

        LogToUart(double, TemperatureReading)(kXPosT1,
                                              &NanopbUtils::NanopbTemperature);
        LogToUart(double, TemperatureReading)(kXPosT2,
                                              &NanopbUtils::NanopbTemperature);
        LogToUart(double, TemperatureReading)(kYPosT1,
                                              &NanopbUtils::NanopbTemperature);
        LogToUart(double, TemperatureReading)(kYPosT2,
                                              &NanopbUtils::NanopbTemperature);
        LogToUart(double, TemperatureReading)(kXNegT1,
                                              &NanopbUtils::NanopbTemperature);
        LogToUart(double, TemperatureReading)(kXNegT2,
                                              &NanopbUtils::NanopbTemperature);
        LogToUart(double, TemperatureReading)(kYNegT1,
                                              &NanopbUtils::NanopbTemperature);
        LogToUart(double, TemperatureReading)(kYNegT2,
                                              &NanopbUtils::NanopbTemperature);
        LogToUart(double, TemperatureReading)(kZNegT1,
                                              &NanopbUtils::NanopbTemperature);
        LogToUart(double, TemperatureReading)(kZNegT2,
                                              &NanopbUtils::NanopbTemperature);
        LogToUart(double, TemperatureReading)(kZPosT,
                                              &NanopbUtils::NanopbTemperature);

        LogToUart(double, VoltageReading)(kXPosV,
                                          &NanopbUtils::NanopbVoltage);
        LogToUart(double, VoltageReading)(kXPosSolarV,
                                          &NanopbUtils::NanopbVoltage);
        LogToUart(double, VoltageReading)(kYPosV,
                                          &NanopbUtils::NanopbVoltage);
        LogToUart(double, VoltageReading)(kYPosSolarV,
                                          &NanopbUtils::NanopbVoltage);
        LogToUart(double, VoltageReading)(kXNegV,
                                          &NanopbUtils::NanopbVoltage);
        LogToUart(double, VoltageReading)(kXNegSolarV,
                                          &NanopbUtils::NanopbVoltage);
        LogToUart(double, VoltageReading)(kYNegV,
                                          &NanopbUtils::NanopbVoltage);
        LogToUart(double, VoltageReading)(kYNegSolarV,
                                          &NanopbUtils::NanopbVoltage);
        LogToUart(double, VoltageReading)(kZNegV,
                                          &NanopbUtils::NanopbVoltage);
        LogToUart(double, VoltageReading)(kZNegSolarV,
                                          &NanopbUtils::NanopbVoltage);
        LogToUart(double, VoltageReading)(kEpsTopPanelV,
                                          &NanopbUtils::NanopbVoltage);
        LogToUart(double, VoltageReading)(kEpsTopSolarV,
                                          &NanopbUtils::NanopbVoltage);

        LogToUart(double, CurrentReading)(kXPosI,
                                          &NanopbUtils::NanopbCurrent);
        LogToUart(double, CurrentReading)(kXPosSolarI,
                                          &NanopbUtils::NanopbCurrent);
        LogToUart(double, CurrentReading)(kYPosI,
                                          &NanopbUtils::NanopbCurrent);
        LogToUart(double, CurrentReading)(kYPosSolarI,
                                          &NanopbUtils::NanopbCurrent);
        LogToUart(double, CurrentReading)(kXNegI,
                                          &NanopbUtils::NanopbCurrent);
        LogToUart(double, CurrentReading)(kXNegSolarI,
                                          &NanopbUtils::NanopbCurrent);
        LogToUart(double, CurrentReading)(kYNegI,
                                          &NanopbUtils::NanopbCurrent);
        LogToUart(double, CurrentReading)(kYNegSolarI,
                                          &NanopbUtils::NanopbCurrent);
        LogToUart(double, CurrentReading)(kZNegI,
                                          &NanopbUtils::NanopbCurrent);
        LogToUart(double, CurrentReading)(kZNegSolarI,
                                          &NanopbUtils::NanopbCurrent);
        LogToUart(double, CurrentReading)(kEpsTopPanelI,
                                          &NanopbUtils::NanopbCurrent);
        LogToUart(double, CurrentReading)(kEpsTopSolarI,
                                          &NanopbUtils::NanopbCurrent);

        // TODO(akremor): Keeping these as they represent uninitialised
        // measurables
        // LogToUart(double, CurrentReading)(kEpsBoostInI1,
        //                                         &NanopbUtils::NanopbCurrent);
        // LogToUart(double, CurrentReading)(kEpsLoadI1,
        //                                         &NanopbUtils::NanopbCurrent);
        LogToUart(double, VoltageReading)(kEpsAdcBatV2,
                                          &NanopbUtils::NanopbVoltage);
        LogToUart(double, VoltageReading)(kEpsBoostOutV2,
                                          &NanopbUtils::NanopbVoltage);
        // LogToUart(double, CurrentReading)(kEpsBoostInI2,
        //                                        &NanopbUtils::NanopbCurrent);
        // LogToUart(double, CurrentReading)(kEpsLoadI2,
        //                                         &NanopbUtils::NanopbCurrent);
        LogToUart(double, VoltageReading)(kEpsRail1,
                                          &NanopbUtils::NanopbVoltage);
        LogToUart(double, VoltageReading)(kEpsRail2,
                                          &NanopbUtils::NanopbVoltage);
        LogToUart(BmsReadings, BmsReadings)(kEpsBmsReadings1,
                                            &NanopbUtils::NanopbBms);
        LogToUart(BmsReadings, BmsReadings)(kEpsBmsReadings2,
                                            &NanopbUtils::NanopbBms);

        // Flight Systems
        LogToUart(double, CurrentReading)(kFsTorquerXI,
                                          &NanopbUtils::NanopbCurrent);
        LogToUart(double, CurrentReading)(kFsTorquerTotalI,
                                          &NanopbUtils::NanopbCurrent);
        LogToUart(double, CurrentReading)(kFsTorquerYI,
                                          &NanopbUtils::NanopbCurrent);
        LogToUart(double, CurrentReading)(kFsTorquerZI,
                                          &NanopbUtils::NanopbCurrent);
        LogToUart(double, VoltageReading)(kFsTorquerXAV,
                                          &NanopbUtils::NanopbVoltage);
        LogToUart(double, VoltageReading)(kFsTorquerXBV,
                                          &NanopbUtils::NanopbVoltage);
        LogToUart(double, VoltageReading)(kFsTorquerYAV,
                                          &NanopbUtils::NanopbVoltage);
        LogToUart(double, VoltageReading)(kFsTorquerYBV,
                                          &NanopbUtils::NanopbVoltage);
        LogToUart(double, VoltageReading)(kFsTorquerZAV,
                                          &NanopbUtils::NanopbVoltage);
        LogToUart(double, VoltageReading)(kFsTorquerZBV,
                                          &NanopbUtils::NanopbVoltage);

        LogToUart(double, TemperatureReading)(kFsHbXT,
                                              &NanopbUtils::NanopbTemperature);
        LogToUart(double, TemperatureReading)(kFsHbYT,
                                              &NanopbUtils::NanopbTemperature);
        LogToUart(double, TemperatureReading)(kFsHbZT,
                                              &NanopbUtils::NanopbTemperature);
        LogToUart(GyroscopeReading, GyroscopeReading)(
            kFsImuGyro1, &NanopbUtils::NanopbGyroscope);
        LogToUart(AccelerometerReading, AccelerometerReading)(
            kFsImuAccel1, &NanopbUtils::NanopbAccelerometer);
        LogToUart(double, TemperatureReading)(kFsImuT1,
                                              &NanopbUtils::NanopbTemperature);
        LogToUart(MagnetometerReading, MagnetometerReading)(
            kFsImuMagno1, &NanopbUtils::NanopbMagnetometer);
        LogToUart(GyroscopeReading, GyroscopeReading)(
            kFsImuGyro2, &NanopbUtils::NanopbGyroscope);
        LogToUart(AccelerometerReading, AccelerometerReading)(
            kFsImuAccel2, &NanopbUtils::NanopbAccelerometer);
        LogToUart(double, TemperatureReading)(kFsImuT2,
                                              &NanopbUtils::NanopbTemperature);
        LogToUart(MagnetometerReading, MagnetometerReading)(
            kFsImuMagno2, &NanopbUtils::NanopbMagnetometer);

        // CDH
        LogToUart(double, TemperatureReading)(kCdhT,
                                              &NanopbUtils::NanopbTemperature);

        // TODO(dingbenjamin): Yet to add:
        //        kUtilAdc2,
        //        kUtilT,

        SystemWatchdog::ResetTimer();
        TaskUtils::SleepMilli(kHealthCheckPeriodMillis);
    }
}

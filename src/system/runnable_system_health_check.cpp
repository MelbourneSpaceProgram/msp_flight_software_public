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

        LogToUart(double, VoltageReading)(kCommsRegulator1InputVoltage,
                                          &NanopbUtils::NanopbVoltage);

        LogToUart(double, VoltageReading)(kCommsRegulator1OutputVoltage,
                                          &NanopbUtils::NanopbVoltage);
        LogToUart(double, VoltageReading)(kCommsRegulator2InputVoltage,
                                          &NanopbUtils::NanopbVoltage);
        LogToUart(double, VoltageReading)(kCommsRegulator2OutputVoltage,
                                          &NanopbUtils::NanopbVoltage);
        LogToUart(double, CurrentReading)(kCommsRegulator1InputCurrent,
                                          &NanopbUtils::NanopbCurrent);
        LogToUart(double, CurrentReading)(kCommsRegulator1OutputCurrent,
                                          &NanopbUtils::NanopbCurrent);
        LogToUart(double, CurrentReading)(kCommsRegulator2InputCurrent,
                                          &NanopbUtils::NanopbCurrent);
        LogToUart(double, CurrentReading)(kCommsRegulator2OutputCurrent,
                                          &NanopbUtils::NanopbCurrent);
        LogToUart(double, TemperatureReading)(kCommsTemp1,
                                              &NanopbUtils::NanopbTemperature);

        // EPS
        LogToUart(double, CurrentReading)(kPowerBoostInCurrent1,
                                          &NanopbUtils::NanopbCurrent);
        LogToUart(double, CurrentReading)(kPowerLoadCurrent1,
                                          &NanopbUtils::NanopbCurrent);
        LogToUart(double, CurrentReading)(kPowerLoadCurrent2,
                                          &NanopbUtils::NanopbCurrent);
        LogToUart(double, CurrentReading)(kPowerBoostInCurrent2,
                                          &NanopbUtils::NanopbCurrent);
        LogToUart(double, TemperatureReading)(kPowerTemp1,
                                              &NanopbUtils::NanopbTemperature);
        LogToUart(double, TemperatureReading)(kPowerTemp2,
                                              &NanopbUtils::NanopbTemperature);
        LogToUart(double, TemperatureReading)(kPowerBmsDieTemp1,
                                              &NanopbUtils::NanopbTemperature);
        LogToUart(double, TemperatureReading)(kPowerBmsDieTemp2,
                                              &NanopbUtils::NanopbTemperature);
        LogToUart(double, TemperatureReading)(kPowerBmsBatteryTemp1,
                                              &NanopbUtils::NanopbTemperature);
        LogToUart(double, TemperatureReading)(kPowerBmsBatteryTemp2,
                                              &NanopbUtils::NanopbTemperature);
        LogToUart(double, VoltageReading)(kEpsBatVoltage1,
                                          &NanopbUtils::NanopbVoltage);
        LogToUart(double, VoltageReading)(kEpsBoostOutVoltage1,
                                          &NanopbUtils::NanopbVoltage);
        // TODO(akremor): Keeping these as they represent uninitialised
        // measurables
        // LogToUart(double, CurrentReading)(kEpsBoostInCurrent1,
        //                                         &NanopbUtils::NanopbCurrent);
        // LogToUart(double, CurrentReading)(kEpsLoadCurrent1,
        //                                         &NanopbUtils::NanopbCurrent);
        LogToUart(double, VoltageReading)(kEpsBatVoltage2,
                                          &NanopbUtils::NanopbVoltage);
        LogToUart(double, VoltageReading)(kEpsBoostOutVoltage2,
                                          &NanopbUtils::NanopbVoltage);
        // LogToUart(double, CurrentReading)(kEpsBoostInCurrent2,
        //                                        &NanopbUtils::NanopbCurrent);
        // LogToUart(double, CurrentReading)(kEpsLoadCurrent2,
        //                                         &NanopbUtils::NanopbCurrent);
        LogToUart(double, VoltageReading)(kEps5VRail1,
                                          &NanopbUtils::NanopbVoltage);
        LogToUart(double, VoltageReading)(kEps5VRail2,
                                          &NanopbUtils::NanopbVoltage);
        LogToUart(BmsReadings, BmsReadings)(kPowerBmsReadings1,
                                            &NanopbUtils::NanopbBms);
        LogToUart(BmsReadings, BmsReadings)(kPowerBmsReadings2,
                                            &NanopbUtils::NanopbBms);

        // Flight Systems
        LogToUart(double, CurrentReading)(kFsTorquerCurrentX,
                                          &NanopbUtils::NanopbCurrent);
        LogToUart(double, CurrentReading)(kFsTorquerCurrentTotal,
                                          &NanopbUtils::NanopbCurrent);
        LogToUart(double, CurrentReading)(kFsTorquerCurrentY,
                                          &NanopbUtils::NanopbCurrent);
        LogToUart(double, CurrentReading)(kFsTorquerCurrentZ,
                                          &NanopbUtils::NanopbCurrent);
        LogToUart(double, VoltageReading)(kFsMagTorqAX,
                                          &NanopbUtils::NanopbVoltage);
        LogToUart(double, VoltageReading)(kFsMagTorqBX,
                                          &NanopbUtils::NanopbVoltage);
        LogToUart(double, VoltageReading)(kFsMagTorqAY,
                                          &NanopbUtils::NanopbVoltage);
        LogToUart(double, VoltageReading)(kFsMagTorqBY,
                                          &NanopbUtils::NanopbVoltage);
        LogToUart(double, VoltageReading)(kFsMagTorqAZ,
                                          &NanopbUtils::NanopbVoltage);
        LogToUart(double, VoltageReading)(kFsMagTorqBZ,
                                          &NanopbUtils::NanopbVoltage);

        LogToUart(double, TemperatureReading)(kFsTempHbX,
                                              &NanopbUtils::NanopbTemperature);
        LogToUart(double, TemperatureReading)(kFsTempHbY,
                                              &NanopbUtils::NanopbTemperature);
        LogToUart(double, TemperatureReading)(kFsTempHbZ,
                                              &NanopbUtils::NanopbTemperature);
        LogToUart(GyroscopeReading, GyroscopeReading)(
            kFsImuGyro1, &NanopbUtils::NanopbGyroscope);
        LogToUart(AccelerometerReading, AccelerometerReading)(
            kFsImuAccelerometer1, &NanopbUtils::NanopbAccelerometer);
        LogToUart(double, TemperatureReading)(kFsImuTemperature1,
                                              &NanopbUtils::NanopbTemperature);
        LogToUart(MagnetometerReading, MagnetometerReading)(
            kFsImuMagnetometer1, &NanopbUtils::NanopbMagnetometer);
        LogToUart(GyroscopeReading, GyroscopeReading)(
            kFsImuGyro2, &NanopbUtils::NanopbGyroscope);
        LogToUart(AccelerometerReading, AccelerometerReading)(
            kFsImuAccelerometer2, &NanopbUtils::NanopbAccelerometer);
        LogToUart(double, TemperatureReading)(kFsImuTemperature2,
                                              &NanopbUtils::NanopbTemperature);
        LogToUart(MagnetometerReading, MagnetometerReading)(
            kFsImuMagnetometer2, &NanopbUtils::NanopbMagnetometer);

        // CDH
        LogToUart(double, TemperatureReading)(kCdhSysTemp,
                                              &NanopbUtils::NanopbTemperature);

        // TODO(dingbenjamin): Yet to add:
        //        kUtilAdc2,
        //        kUtilTemp1,

        SystemWatchdog::ResetTimer();
        TaskUtils::SleepMilli(kHealthCheckPeriodMillis);
    }
}

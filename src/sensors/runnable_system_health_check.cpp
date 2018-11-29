#include <src/board/uart/uart.h>
#include <src/config/unit_tests.h>
#include <src/messages/AccelerometerReading.pb.h>
#include <src/messages/BmsChargingInfoReading.pb.h>
#include <src/messages/BmsCurrentsReading.pb.h>
#include <src/messages/BmsOperationValuesReading.pb.h>
#include <src/messages/BmsSettingsReading.pb.h>
#include <src/messages/BmsTemperatureReading.pb.h>
#include <src/messages/BmsVoltagesReading.pb.h>
#include <src/messages/CurrentReading.pb.h>
#include <src/messages/GyroscopeReading.pb.h>
#include <src/messages/MagnetometerReading.pb.h>
#include <src/messages/TemperatureReading.pb.h>
#include <src/messages/VoltageReading.pb.h>
#include <src/sensors/i2c_sensors/measurables/temperature_measurable.h>
#include <src/sensors/measurable_id.h>
#include <src/sensors/runnable_system_health_check.h>
#include <src/telecomms/lithium.h>
#include <src/util/msp_exception.h>
#include <src/util/satellite_time_source.h>
#include <src/util/system_watchdog.h>
#include <src/util/tirtos_utils.h>
#include <ti/drivers/GPIO.h>
#include <ti/drivers/utils/RingBuf.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/utils/Load.h>
#include <xdc/runtime/System.h>

Uart* RunnableSystemHealthCheck::debug_uart = NULL;
bool RunnableSystemHealthCheck::datalogger_enabled = true;

RunnableSystemHealthCheck::RunnableSystemHealthCheck() {}

fnptr RunnableSystemHealthCheck::GetRunnablePointer() {
    return &RunnableSystemHealthCheck::SystemHealthCheck;
}

bool RunnableSystemHealthCheck::IsEnabled() { return datalogger_enabled; }

void RunnableSystemHealthCheck::EnableDatalogger(bool enable_logger) {
    datalogger_enabled = enable_logger;
}

void RunnableSystemHealthCheck::SystemHealthCheck() {
    while (1) {
        try {
            Load_Stat stat;
            Task_Handle idlTskHandle = Task_getIdleTaskHandle(0);
            Load_getTaskLoad(idlTskHandle, &stat);
            uint32_t idle_load = Load_calculateLoad(&stat);
            uint32_t cpu_load = Load_getCPULoad();

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
                    LogMeasurableMacro(BmsChargingInfoReading)(
                        kEpsBmsChargingInfoReading1);
                    LogMeasurableMacro(BmsCurrentsReading)(
                        kEpsBmsCurrentsReading1);
                    LogMeasurableMacro(BmsOperationValuesReading)(
                        kEpsBmsOperationValuesReading1);
                    LogMeasurableMacro(BmsSettingsReading)(
                        kEpsBmsSettingsReading1);
                    LogMeasurableMacro(BmsTemperatureReading)(
                        kEpsBmsTemperatureReading1);
                    LogMeasurableMacro(BmsVoltagesReading)(
                        kEpsBmsVoltagesReading1);
                    LogMeasurableMacro(BmsChargingInfoReading)(
                        kEpsBmsChargingInfoReading2);
                    LogMeasurableMacro(BmsCurrentsReading)(
                        kEpsBmsCurrentsReading2);
                    LogMeasurableMacro(BmsOperationValuesReading)(
                        kEpsBmsOperationValuesReading2);
                    LogMeasurableMacro(BmsSettingsReading)(
                        kEpsBmsSettingsReading2);
                    LogMeasurableMacro(BmsTemperatureReading)(
                        kEpsBmsTemperatureReading2);
                    LogMeasurableMacro(BmsVoltagesReading)(
                        kEpsBmsVoltagesReading2);
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
                    LogMeasurableMacro(MagnetometerReading)(kFsImuMagnoA);
                    LogMeasurableMacro(GyroscopeReading)(kFsImuGyro2);
                    LogMeasurableMacro(AccelerometerReading)(kFsImuAccel2);
                    LogMeasurableMacro(TemperatureReading)(kFsImuT2);
                    LogMeasurableMacro(MagnetometerReading)(kFsImuMagnoB);
                }

                if (kCdhBoardAvailable) {
                    LogMeasurableMacro(TemperatureReading)(kCdhT);
                }

                if (kUtilitiesBoardAvailable) {
                    LogMeasurableMacro(TemperatureReading)(kUtilT);
                    LogMeasurableMacro(VoltageReading)(kUtilHeatV);
                }
            }
            SystemWatchdog::ResetTimer();
            GPIO_toggle(SYS_LED);
            TirtosUtils::SleepMilli(kHealthCheckPeriodMs);
        } catch (MspException& e) {
            MspException::LogTopLevelException(e,
                                               kRunnableSystemHealthCheckCatch);
        }
    }
}

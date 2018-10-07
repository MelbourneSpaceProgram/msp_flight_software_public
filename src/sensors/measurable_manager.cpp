#include <src/board/i2c/bms/bms.h>
#include <src/config/satellite.h>
#include <src/sensors/i2c_sensors/mcp9808.h>
#include <src/sensors/i2c_sensors/measurables/bms_battery_temperature_measurable.h>
#include <src/sensors/i2c_sensors/measurables/bms_charging_info_measurable.h>
#include <src/sensors/i2c_sensors/measurables/bms_currents_measurable.h>
#include <src/sensors/i2c_sensors/measurables/bms_die_temperature_measurable.h>
#include <src/sensors/i2c_sensors/measurables/bms_operation_values_measurable.h>
#include <src/sensors/i2c_sensors/measurables/bms_settings_measurable.h>
#include <src/sensors/i2c_sensors/measurables/bms_temperature_measurable.h>
#include <src/sensors/i2c_sensors/measurables/bms_voltages_measurable.h>
#include <src/sensors/i2c_sensors/measurables/current_measurable.h>
#include <src/sensors/i2c_sensors/measurables/imu_accelerometer_measurable.h>
#include <src/sensors/i2c_sensors/measurables/imu_gyroscope_measurable.h>
#include <src/sensors/i2c_sensors/measurables/imu_magnetometer_measurable.h>
#include <src/sensors/i2c_sensors/measurables/imu_temperature_measurable.h>
#include <src/sensors/i2c_sensors/measurables/temperature_measurable.h>
#include <src/sensors/i2c_sensors/measurables/voltage_measurable.h>
#include <src/sensors/i2c_sensors/mpu9250_motion_tracker.h>
#include <src/sensors/magnetometer_calibration.h>
#include <src/sensors/measurable_id.h>
#include <src/sensors/measurable_manager.h>
#include <src/system/sensor_state_machines/battery_temp_state_machine.h>
#include <src/system/state_manager.h>

MeasurableManager *MeasurableManager::instance = NULL;

MeasurableManager *MeasurableManager::GetInstance() {
    if (instance == NULL) {
        instance = new MeasurableManager();
    }
    return instance;
}

MeasurableManager::MeasurableManager() { measurables.fill(NULL); }

void MeasurableManager::Init(const I2c *bus_a, const I2c *bus_b,
                             const I2c *bus_c, const I2c *bus_d) {
    this->bus_a = bus_a;
    this->bus_b = bus_b;
    this->bus_c = bus_c;
    this->bus_d = bus_d;

    I2cMultiplexer *mux_a = new I2cMultiplexer(bus_a, 0x76);
    I2cMultiplexer *mux_c = new I2cMultiplexer(bus_c, 0x71);

    InitTelecomms(mux_a);
    InitPower(mux_a);
    InitFlightSystems(mux_a);
    InitCdh(mux_a);
    InitUtilities(mux_c);
    InitSolarPanels(mux_c);
}

void MeasurableManager::InitTelecomms(const I2cMultiplexer *mux_a) {
    Adc *comms_adc_1 =
        new Adc(bus_a, 0x48, mux_a, I2cMultiplexer::kMuxChannel3);
    comms_adc_1->SetGainAmplifierLevel(kAdc4v096);
    Adc *comms_adc_2 =
        new Adc(bus_a, 0x49, mux_a, I2cMultiplexer::kMuxChannel3);
    comms_adc_2->SetGainAmplifierLevel(kAdc4v096);

    AddVoltage(kComInV1, comms_adc_1, kAdcP2NGnd, 2.0);
    AddVoltage(kComOutV1, comms_adc_1, kAdcP3NGnd, 3.0);
    AddCurrent(kComInI1, comms_adc_1, kAdcP0NGnd, 1, 0);
    AddCurrent(kComOutI1, comms_adc_1, kAdcP1NGnd, 1, 0);

    AddVoltage(kComInV2, comms_adc_2, kAdcP2NGnd, 2.0);
    AddVoltage(kComOutV2, comms_adc_2, kAdcP3NGnd, 3.0);
    AddCurrent(kComInI2, comms_adc_2, kAdcP0NGnd, 1, 0);
    AddCurrent(kComOutI2, comms_adc_2, kAdcP1NGnd, 1, 0);

    Mcp9808 *comms_temp_1 =
        new Mcp9808(bus_a, 0x18, mux_a, I2cMultiplexer::kMuxChannel3);
    Mcp9808 *comms_temp_2 =
        new Mcp9808(bus_a, 0x19, mux_a, I2cMultiplexer::kMuxChannel3);

    AddTemperature(kComT1, comms_temp_1);
    AddTemperature(kComT2, comms_temp_2);
}

void MeasurableManager::InitPower(const I2cMultiplexer *mux_a) {
    Adc *power_adc_1 =
        new Adc(bus_a, 0x48, mux_a, I2cMultiplexer::kMuxChannel2);
    power_adc_1->SetGainAmplifierLevel(kAdc2v048);
    Adc *power_adc_2 =
        new Adc(bus_a, 0x49, mux_a, I2cMultiplexer::kMuxChannel2);
    power_adc_2->SetGainAmplifierLevel(kAdc2v048);
    Adc *power_adc_3 =
        new Adc(bus_a, 0x4A, mux_a, I2cMultiplexer::kMuxChannel2);
    power_adc_3->SetGainAmplifierLevel(kAdc2v048);

    AddVoltage(kEpsAdcBatV1, power_adc_1, kAdcP0NGnd, 3.0);
    AddVoltage(kEpsBoostOutV1, power_adc_1, kAdcP1NGnd, 3.0);
    AddCurrent(kEpsBoostInI1, power_adc_1, kAdcP2NGnd, 5.0 / 3, 0);
    AddCurrent(kEpsLoadI1, power_adc_1, kAdcP3NGnd, 5.0 / 3, 0);

    AddVoltage(kEpsAdcBatV2, power_adc_2, kAdcP0NGnd, 3.0);
    AddVoltage(kEpsBoostOutV2, power_adc_2, kAdcP1NGnd, 3.0);
    AddCurrent(kEpsLoadI2, power_adc_2, kAdcP2NGnd, 5.0 / 3, 0);
    AddCurrent(kEpsBoostInI2, power_adc_2, kAdcP3NGnd, 5.0 / 3, 0);

    AddVoltage(kEpsRail1, power_adc_3, kAdcP0NGnd, 3.0);
    AddVoltage(kEpsRail2, power_adc_3, kAdcP1NGnd, 3.0);

    Mcp9808 *power_temp_1 =
        new Mcp9808(bus_a, 0x18, mux_a, I2cMultiplexer::kMuxChannel2);
    Mcp9808 *power_temp_2 =
        new Mcp9808(bus_a, 0x19, mux_a, I2cMultiplexer::kMuxChannel2);

    Bms *bms_bus_d = new Bms(bus_d, 0x68, NULL, I2cMultiplexer::kMuxNoChannel);
    Bms *bms_bus_c = new Bms(bus_c, 0x68, NULL, I2cMultiplexer::kMuxNoChannel);

    StateManager *state_manager = StateManager::GetStateManager();
    BatteryTempStateMachine *battery_temp_state_machine =
        static_cast<BatteryTempStateMachine *>(
            state_manager->GetStateMachine(kBatteryTempStateMachine));

    battery_temp_state_machine->RegisterWithSensor(
        AddBmsBatteryTempMeasurable(kEpsBmsBatT1, bms_bus_d));
    battery_temp_state_machine->RegisterWithSensor(
        AddBmsBatteryTempMeasurable(kEpsBmsBatT2, bms_bus_c));

    // TODO(hugorilla): Remove redundant BMS measurables here
    AddTemperature(kEpsT1, power_temp_1);
    AddTemperature(kEpsT2, power_temp_2);
    AddBmsDieTempMeasurable(kEpsBmsDieT1, bms_bus_d);
    AddBmsDieTempMeasurable(kEpsBmsDieT2, bms_bus_c);
    AddBmsSettingsMeasurable(kEpsBmsSettingsReading1, bms_bus_d);
    AddBmsSettingsMeasurable(kEpsBmsSettingsReading2, bms_bus_c);
    AddBmsChargingInfoMeasurable(kEpsBmsChargingInfoReading1, bms_bus_d);
    AddBmsChargingInfoMeasurable(kEpsBmsChargingInfoReading2, bms_bus_c);
    AddBmsTemperatureMeasurable(kEpsBmsTemperatureReading1, bms_bus_d);
    AddBmsTemperatureMeasurable(kEpsBmsTemperatureReading2, bms_bus_c);
    AddBmsVoltagesMeasurable(kEpsBmsVoltagesReading1, bms_bus_d);
    AddBmsVoltagesMeasurable(kEpsBmsVoltagesReading2, bms_bus_c);
    AddBmsCurrentsMeasurable(kEpsBmsCurrentsReading1, bms_bus_d);
    AddBmsCurrentsMeasurable(kEpsBmsCurrentsReading2, bms_bus_c);
    AddBmsOperationValuesMeasurable(kEpsBmsOperationValuesReading1, bms_bus_d);
    AddBmsOperationValuesMeasurable(kEpsBmsOperationValuesReading2, bms_bus_c);
}

void MeasurableManager::InitFlightSystems(const I2cMultiplexer *mux_a) {
    Adc *fs_adc_x = new Adc(bus_a, 0x48, mux_a, I2cMultiplexer::kMuxChannel1);
    fs_adc_x->SetGainAmplifierLevel(kAdc4v096);
    Adc *fs_adc_y = new Adc(bus_a, 0x49, mux_a, I2cMultiplexer::kMuxChannel1);
    fs_adc_y->SetGainAmplifierLevel(kAdc4v096);
    Adc *fs_adc_z = new Adc(bus_a, 0x4B, mux_a, I2cMultiplexer::kMuxChannel1);
    fs_adc_z->SetGainAmplifierLevel(kAdc4v096);

    AddVoltage(kFsTorquerXAV, fs_adc_x, kAdcP1NGnd, 2);
    AddVoltage(kFsTorquerXBV, fs_adc_x, kAdcP2NGnd, 2);
    AddVoltage(kFsTorquerYAV, fs_adc_y, kAdcP1NGnd, 2);
    AddVoltage(kFsTorquerYBV, fs_adc_y, kAdcP2NGnd, 2);
    AddVoltage(kFsTorquerZAV, fs_adc_z, kAdcP1NGnd, 2);
    AddVoltage(kFsTorquerZBV, fs_adc_z, kAdcP2NGnd, 2);

    AddCurrent(kFsTorquerXI, fs_adc_x, kAdcP0NGnd, 0.1, 1.65);
    AddCurrent(kFsTorquerTotalI, fs_adc_x, kAdcP3NGnd, 1.0 / 7.5, 0);
    AddCurrent(kFsTorquerYI, fs_adc_y, kAdcP0NGnd, 0.1, 1.65);
    AddCurrent(kFsTorquerZI, fs_adc_z, kAdcP0NGnd, 0.1, 1.65);

    Mcp9808 *fs_hb_xt =
        new Mcp9808(bus_a, 0x18, mux_a, I2cMultiplexer::kMuxChannel1);
    Mcp9808 *fs_hb_yt =
        new Mcp9808(bus_a, 0x19, mux_a, I2cMultiplexer::kMuxChannel1);
    Mcp9808 *fs_hb_zt =
        new Mcp9808(bus_a, 0x1A, mux_a, I2cMultiplexer::kMuxChannel1);

    AddTemperature(kFsHbXT, fs_hb_xt);
    AddTemperature(kFsHbYT, fs_hb_yt);
    AddTemperature(kFsHbZT, fs_hb_zt);

    MPU9250MotionTracker *fs_imu_1 = new MPU9250MotionTracker(
        bus_a, 0x68, mux_a, I2cMultiplexer::kMuxChannel1);

    AddImuGyrometerMeasurable(kFsImuGyro1, fs_imu_1);
    AddImuAcceleromterMeasurable(kFsImuAccel1, fs_imu_1);
    AddImuTemperatureMeasurable(kFsImuT1, fs_imu_1);
    double initial_biases_bus_a_data[3][1];
    const Matrix initial_biases_bus_a(
        kPreFlightMagnetometerCalibrationBiasesImuBusA,
        initial_biases_bus_a_data);
    double initial_scale_factors_bus_a_data[3][3];
    const Matrix initial_scale_factors_bus_a(
        kPreFlightMagnetometerCalibrationScaleFactorsImuBusA,
        initial_scale_factors_bus_a_data);

    AddImuMagnetometerMeasurable(
        kFsImuMagno1, fs_imu_1, kImuAToBodyFrameTransform, initial_biases_bus_a,
        initial_scale_factors_bus_a, MagnetometerCalibration::kBufferFilenameA);

    MPU9250MotionTracker *fs_imu_2 = new MPU9250MotionTracker(bus_b, 0x68);

    AddImuGyrometerMeasurable(kFsImuGyro2, fs_imu_2);
    AddImuAcceleromterMeasurable(kFsImuAccel2, fs_imu_2);
    AddImuTemperatureMeasurable(kFsImuT2, fs_imu_2);
    double initial_biases_bus_b_data[3][1];
    const Matrix initial_biases_bus_b(
        kPreFlightMagnetometerCalibrationBiasesImuBusB,
        initial_biases_bus_b_data);
    double initial_scale_factors_bus_b_data[3][3];
    const Matrix initial_scale_factors_bus_b(
        kPreFlightMagnetometerCalibrationScaleFactorsImuBusB,
        initial_scale_factors_bus_b_data);
    AddImuMagnetometerMeasurable(
        kFsImuMagno2, fs_imu_2, kImuBToBodyFrameTransform, initial_biases_bus_b,
        initial_scale_factors_bus_b, MagnetometerCalibration::kBufferFilenameA);
}

void MeasurableManager::InitUtilities(const I2cMultiplexer *mux_c) {
    Adc *util_adc_1 = new Adc(bus_c, 0x49, mux_c, I2cMultiplexer::kMuxChannel1);
    AddVoltage(kUtilHeatV, util_adc_1, kAdcP3NGnd, 1);

    Mcp9808 *util_temp =
        new Mcp9808(bus_c, 0x1C, mux_c, I2cMultiplexer::kMuxChannel1);
    AddTemperature(kUtilT, util_temp);
}

void MeasurableManager::InitCdh(const I2cMultiplexer *mux_a) {
    Mcp9808 *cdh_temp_1 =
        new Mcp9808(bus_a, 0x1A, mux_a, I2cMultiplexer::kMuxChannel0);
    AddTemperature(kCdhT, cdh_temp_1);
}

void MeasurableManager::InitSolarPanels(const I2cMultiplexer *mux_c) {
    Mcp9808 *x_pos_temp_1 =
        new Mcp9808(bus_c, 0x19, mux_c, I2cMultiplexer::kMuxChannel4);
    Mcp9808 *x_pos_temp_2 =
        new Mcp9808(bus_c, 0x1A, mux_c, I2cMultiplexer::kMuxChannel4);

    AddTemperature(kXPosT1, x_pos_temp_1);
    AddTemperature(kXPosT2, x_pos_temp_2);

    Mcp9808 *y_pos_temp_1 =
        new Mcp9808(bus_c, 0x19, mux_c, I2cMultiplexer::kMuxChannel5);
    Mcp9808 *y_pos_temp_2 =
        new Mcp9808(bus_c, 0x1A, mux_c, I2cMultiplexer::kMuxChannel5);

    AddTemperature(kYPosT1, y_pos_temp_1);
    AddTemperature(kYPosT2, y_pos_temp_2);

    Mcp9808 *x_neg_temp_1 =
        new Mcp9808(bus_c, 0x19, mux_c, I2cMultiplexer::kMuxChannel6);
    Mcp9808 *x_neg_temp_2 =
        new Mcp9808(bus_c, 0x1A, mux_c, I2cMultiplexer::kMuxChannel6);

    AddTemperature(kXNegT1, x_neg_temp_1);
    AddTemperature(kXNegT2, x_neg_temp_2);

    Mcp9808 *y_neg_temp_1 =
        new Mcp9808(bus_c, 0x19, mux_c, I2cMultiplexer::kMuxChannel7);
    Mcp9808 *y_neg_temp_2 =
        new Mcp9808(bus_c, 0x1A, mux_c, I2cMultiplexer::kMuxChannel7);

    AddTemperature(kYNegT1, y_neg_temp_1);
    AddTemperature(kYNegT2, y_neg_temp_2);

    Mcp9808 *z_neg_temp_1 =
        new Mcp9808(bus_c, 0x19, mux_c, I2cMultiplexer::kMuxChannel3);
    Mcp9808 *z_neg_temp_2 =
        new Mcp9808(bus_c, 0x1A, mux_c, I2cMultiplexer::kMuxChannel3);

    AddTemperature(kZNegT1, z_neg_temp_1);
    AddTemperature(kZNegT2, z_neg_temp_2);

    Mcp9808 *z_pos_temp_1 =
        new Mcp9808(bus_c, 0x19, mux_c, I2cMultiplexer::kMuxChannel2);

    AddTemperature(kZPosT, z_pos_temp_1);

    Adc *z_pos_adc = new Adc(bus_c, 0x4B, mux_c, I2cMultiplexer::kMuxChannel2);
    z_pos_adc->SetGainAmplifierLevel(kAdc4v096);

    AddVoltage(kEpsTopPanelV, z_pos_adc, kAdcP0NGnd, 3.0);
    AddVoltage(kEpsTopSolarV, z_pos_adc, kAdcP1NGnd, 3.0);
    AddCurrent(kEpsTopSolarI, z_pos_adc, kAdcP2NGnd, 5.0 / 3, 0);
    AddCurrent(kEpsTopPanelI, z_pos_adc, kAdcP3NGnd, 5.0 / 3, 0);

    Adc *x_pos_adc = new Adc(bus_c, 0x48, mux_c, I2cMultiplexer::kMuxChannel4);
    x_pos_adc->SetGainAmplifierLevel(kAdc4v096);

    AddVoltage(kXPosV, x_pos_adc, kAdcP0NGnd, 2.0);
    AddVoltage(kXPosSolarV, x_pos_adc, kAdcP2NGnd, 2.0);
    AddCurrent(kXPosI, x_pos_adc, kAdcP1NGnd, 0.285714, 0);
    AddCurrent(kXPosSolarI, x_pos_adc, kAdcP3NGnd, 0.285714, 0);

    Adc *y_pos_adc = new Adc(bus_c, 0x48, mux_c, I2cMultiplexer::kMuxChannel5);
    y_pos_adc->SetGainAmplifierLevel(kAdc4v096);

    AddVoltage(kYPosV, y_pos_adc, kAdcP0NGnd, 2.0);
    AddVoltage(kYPosSolarV, y_pos_adc, kAdcP2NGnd, 2.0);
    AddCurrent(kYPosI, y_pos_adc, kAdcP1NGnd, 0.285714, 0);
    AddCurrent(kYPosSolarI, y_pos_adc, kAdcP3NGnd, 0.285714, 0);

    Adc *x_neg_adc = new Adc(bus_c, 0x48, mux_c, I2cMultiplexer::kMuxChannel6);
    x_neg_adc->SetGainAmplifierLevel(kAdc4v096);

    AddVoltage(kXNegV, x_neg_adc, kAdcP0NGnd, 2.0);
    AddVoltage(kXNegSolarV, x_neg_adc, kAdcP2NGnd, 2.0);
    AddCurrent(kXNegI, x_neg_adc, kAdcP1NGnd, 0.285714, 0);
    AddCurrent(kXNegSolarI, x_neg_adc, kAdcP3NGnd, 0.285714, 0);

    Adc *y_neg_adc = new Adc(bus_c, 0x48, mux_c, I2cMultiplexer::kMuxChannel7);
    y_neg_adc->SetGainAmplifierLevel(kAdc4v096);

    AddVoltage(kYNegV, y_neg_adc, kAdcP0NGnd, 2.0);
    AddVoltage(kYNegSolarV, y_neg_adc, kAdcP2NGnd, 2.0);
    AddCurrent(kYNegI, y_neg_adc, kAdcP1NGnd, 0.285714, 0);
    AddCurrent(kYNegSolarI, y_neg_adc, kAdcP3NGnd, 0.285714, 0);

    Adc *z_neg_adc = new Adc(bus_c, 0x48, mux_c, I2cMultiplexer::kMuxChannel3);
    z_neg_adc->SetGainAmplifierLevel(kAdc4v096);

    AddVoltage(kZNegV, z_neg_adc, kAdcP0NGnd, 2.0);
    AddVoltage(kZNegSolarV, z_neg_adc, kAdcP2NGnd, 2.0);
    AddCurrent(kZNegI, z_neg_adc, kAdcP1NGnd, 0.285714, 0);
    AddCurrent(kZNegSolarI, z_neg_adc, kAdcP3NGnd, 0.285714, 0);
}

void MeasurableManager::AddVoltage(MeasurableId id, Adc *adc, AdcMuxMode line,
                                   float scaling_factor) {
    CheckValidId(id);
    VoltageMeasurable *voltage =
        new VoltageMeasurable(adc, line, scaling_factor);
    measurables[id] = voltage;
}

void MeasurableManager::AddCurrent(MeasurableId id, Adc *adc, AdcMuxMode line,
                                   float scaling_factor,
                                   float zero_bias_point) {
    CheckValidId(id);
    CurrentMeasurable *current =
        new CurrentMeasurable(adc, line, scaling_factor, zero_bias_point);
    measurables[id] = current;
}

void MeasurableManager::AddTemperature(MeasurableId id, Mcp9808 *temp_sensor) {
    CheckValidId(id);
    TemperatureMeasurable *temp = new TemperatureMeasurable(temp_sensor);
    measurables[id] = temp;
}

void MeasurableManager::AddBmsDieTempMeasurable(MeasurableId id, Bms *bms) {
    CheckValidId(id);
    BmsDieTemperatureMeasurable *temp = new BmsDieTemperatureMeasurable(bms);
    measurables[id] = temp;
}

void MeasurableManager::AddBmsSettingsMeasurable(MeasurableId id, Bms *bms) {
    CheckValidId(id);
    BmsSettingsMeasurable *bms_settings = new BmsSettingsMeasurable(bms);
    measurables[id] = bms_settings;
}

void MeasurableManager::AddBmsChargingInfoMeasurable(MeasurableId id,
                                                     Bms *bms) {
    CheckValidId(id);
    BmsChargingInfoMeasurable *bms_charging_info =
        new BmsChargingInfoMeasurable(bms);
    measurables[id] = bms_charging_info;
}

void I2cMeasurableManager::AddBmsOperationValuesMeasurable(MeasurableId id,
                                                           Bms *bms) {
    CheckValidId(id);
    BmsOperationValuesMeasurable *bms_operation_values =
        new BmsOperationValuesMeasurable(bms);
    measurables[id] = bms_operation_values;
}

void I2cMeasurableManager::AddBmsTemperatureMeasurable(MeasurableId id,
                                                       Bms *bms) {
    CheckValidId(id);
    BmsTemperatureMeasurable *bms_temperatures =
        new BmsTemperatureMeasurable(bms);
    measurables[id] = bms_temperatures;
}

void MeasurableManager::AddBmsVoltagesMeasurable(MeasurableId id, Bms *bms) {
    CheckValidId(id);
    BmsVoltagesMeasurable *bms_voltages = new BmsVoltagesMeasurable(bms);
    measurables[id] = bms_voltages;
}

void MeasurableManager::AddBmsCurrentsMeasurable(MeasurableId id, Bms *bms) {
    CheckValidId(id);
    BmsCurrentsMeasurable *bms_currents = new BmsCurrentsMeasurable(bms);
    measurables[id] = bms_currents;
}

void MeasurableManager::AddImuGyrometerMeasurable(
    MeasurableId id, MPU9250MotionTracker *imu_sensor) {
    CheckValidId(id);
    ImuGyroscopeMeasurable *gyro = new ImuGyroscopeMeasurable(imu_sensor);
    measurables[id] = gyro;
}

void MeasurableManager::AddImuAcceleromterMeasurable(
    MeasurableId id, MPU9250MotionTracker *imu_sensor) {
    CheckValidId(id);
    ImuAccelerometerMeasurable *accelerometer =
        new ImuAccelerometerMeasurable(imu_sensor);
    measurables[id] = accelerometer;
}

void MeasurableManager::AddImuTemperatureMeasurable(
    MeasurableId id, MPU9250MotionTracker *imu_sensor) {
    CheckValidId(id);
    ImuTemperatureMeasurable *temp = new ImuTemperatureMeasurable(imu_sensor);
    measurables[id] = temp;
}

void MeasurableManager::AddImuMagnetometerMeasurable(
    MeasurableId id, MPU9250MotionTracker *imu_sensor,
    const Matrix &frame_mapping, const Matrix &initial_biases,
    const Matrix &initial_scale_factors,
    const char *calibration_readings_buffer_filename) {
    CheckValidId(id);
    ImuMagnetometerMeasurable *magnetometer = new ImuMagnetometerMeasurable(
        imu_sensor, frame_mapping, initial_biases, initial_scale_factors,
        calibration_readings_buffer_filename);
    measurables[id] = magnetometer;
}

BmsBatteryTemperatureMeasurable *MeasurableManager::AddBmsBatteryTempMeasurable(
    MeasurableId id, Bms *bms) {
    CheckValidId(id);
    BmsBatteryTemperatureMeasurable *temp =
        new BmsBatteryTemperatureMeasurable(bms);
    measurables[id] = temp;
    return temp;
}

void MeasurableManager::CheckValidId(MeasurableId id) {
    if (id >= kMeasurableIdEnd) {
        etl::exception e("Measurable ID outside max size", __FILE__, __LINE__);
        throw e;
    } else if (measurables.at(id) != NULL) {
        etl::exception e("ID already used", __FILE__, __LINE__);
        throw e;
    }
}

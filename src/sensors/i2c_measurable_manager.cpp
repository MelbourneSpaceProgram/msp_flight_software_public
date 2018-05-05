#include <src/board/i2c/bms/bms.h>
#include <src/config/satellite.h>
#include <src/sensors/i2c_measurable_manager.h>
#include <src/sensors/i2c_sensors/adc.h>
#include <src/sensors/i2c_sensors/mcp9808.h>
#include <src/sensors/i2c_sensors/measurables/bms_battery_temperature_measurable.h>
#include <src/sensors/i2c_sensors/measurables/bms_die_temperature_measurable.h>
#include <src/sensors/i2c_sensors/measurables/bms_readings_measurable.h>
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
#include <src/system/sensor_state_machines/battery_temp_state_machine.h>
#include <src/system/state_manager.h>

I2cMeasurableManager *I2cMeasurableManager::instance = NULL;

I2cMeasurableManager *I2cMeasurableManager::GetInstance() {
    if (instance == NULL) {
        instance = new I2cMeasurableManager();
    }
    return instance;
}

I2cMeasurableManager::I2cMeasurableManager() { measurables.fill(NULL); }

void I2cMeasurableManager::Init(const I2c *bus_a, const I2c *bus_b,
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

void I2cMeasurableManager::InitTelecomms(const I2cMultiplexer *mux_a) {
    Adc *comms_adc_1 =
        new Adc(bus_a, 0x48, mux_a, I2cMultiplexer::kMuxChannel3);
    comms_adc_1->SetGainAmplifierLevel(kAdc4v096);
    Adc *comms_adc_2 =
        new Adc(bus_a, 0x49, mux_a, I2cMultiplexer::kMuxChannel3);
    comms_adc_2->SetGainAmplifierLevel(kAdc4v096);

    AddVoltage(kCommsRegulator1InputVoltage, comms_adc_1, kAdcP2NGnd, 2.0);
    AddVoltage(kCommsRegulator1OutputVoltage, comms_adc_1, kAdcP3NGnd, 3.0);
    AddCurrent(kCommsRegulator1InputCurrent, comms_adc_1, kAdcP0NGnd, 1, 0);
    AddCurrent(kCommsRegulator1OutputCurrent, comms_adc_1, kAdcP1NGnd, 1, 0);

    AddVoltage(kCommsRegulator2InputVoltage, comms_adc_2, kAdcP2NGnd, 2.0);
    AddVoltage(kCommsRegulator2OutputVoltage, comms_adc_2, kAdcP3NGnd, 3.0);
    AddCurrent(kCommsRegulator2InputCurrent, comms_adc_2, kAdcP0NGnd, 1, 0);
    AddCurrent(kCommsRegulator2OutputCurrent, comms_adc_2, kAdcP1NGnd, 1, 0);

    MCP9808 *comms_temp_1 =
        new MCP9808(bus_a, 0x18, mux_a, I2cMultiplexer::kMuxChannel3);
    MCP9808 *comms_temp_2 =
        new MCP9808(bus_a, 0x19, mux_a, I2cMultiplexer::kMuxChannel3);

    AddTemperature(kCommsTemp1, comms_temp_1);
    AddTemperature(kCommsTemp2, comms_temp_2);
}

void I2cMeasurableManager::InitPower(const I2cMultiplexer *mux_a) {
    Adc *power_adc_1 =
        new Adc(bus_a, 0x48, mux_a, I2cMultiplexer::kMuxChannel2);
    power_adc_1->SetGainAmplifierLevel(kAdc2v048);
    Adc *power_adc_2 =
        new Adc(bus_a, 0x49, mux_a, I2cMultiplexer::kMuxChannel2);
    power_adc_2->SetGainAmplifierLevel(kAdc2v048);
    Adc *power_adc_3 =
        new Adc(bus_a, 0x4A, mux_a, I2cMultiplexer::kMuxChannel2);
    power_adc_3->SetGainAmplifierLevel(kAdc2v048);

    AddVoltage(kEpsBatVoltage1, power_adc_1, kAdcP0NGnd, 3.0);
    AddVoltage(kEpsBoostOutVoltage1, power_adc_1, kAdcP1NGnd, 3.0);
    AddCurrent(kPowerBoostInCurrent1, power_adc_1, kAdcP2NGnd, 5.0 / 3, 0);
    AddCurrent(kPowerLoadCurrent1, power_adc_1, kAdcP3NGnd, 5.0 / 3, 0);

    AddVoltage(kEpsBatVoltage2, power_adc_2, kAdcP0NGnd, 3.0);
    AddVoltage(kEpsBoostOutVoltage2, power_adc_2, kAdcP1NGnd, 3.0);
    AddCurrent(kPowerBoostInCurrent2, power_adc_2, kAdcP2NGnd, 5.0 / 3, 0);
    AddCurrent(kPowerLoadCurrent2, power_adc_2, kAdcP3NGnd, 5.0 / 3, 0);

    AddVoltage(kEps5VRail1, power_adc_3, kAdcP0NGnd, 3.0);
    AddVoltage(kEps5VRail2, power_adc_3, kAdcP1NGnd, 3.0);

    MCP9808 *power_temp_1 =
        new MCP9808(bus_a, 0x18, mux_a, I2cMultiplexer::kMuxChannel2);
    MCP9808 *power_temp_2 =
        new MCP9808(bus_a, 0x19, mux_a, I2cMultiplexer::kMuxChannel2);

    Bms *bms_bus_d = new Bms(bus_d, 0x68, NULL, I2cMultiplexer::kMuxNoChannel);
    Bms *bms_bus_c = new Bms(bus_c, 0x68, NULL, I2cMultiplexer::kMuxNoChannel);

    StateManager *state_manager = StateManager::GetStateManager();
    BatteryTempStateMachine *battery_temp_state_machine =
        static_cast<BatteryTempStateMachine *>(
            state_manager->GetStateMachine(kBatteryTempStateMachine));

    battery_temp_state_machine->RegisterWithSensor(
        AddBmsBatteryTempMeasurable(kPowerBmsBatteryTemp1, bms_bus_d));
    battery_temp_state_machine->RegisterWithSensor(
        AddBmsBatteryTempMeasurable(kPowerBmsBatteryTemp2, bms_bus_c));

    AddTemperature(kPowerTemp1, power_temp_1);
    AddTemperature(kPowerTemp2, power_temp_2);
    AddBmsDieTempMeasurable(kPowerBmsDieTemp1, bms_bus_d);
    AddBmsDieTempMeasurable(kPowerBmsDieTemp2, bms_bus_c);
    AddBmsReadingsMeasurable(kPowerBmsReadings1, bms_bus_d);
    AddBmsReadingsMeasurable(kPowerBmsReadings2, bms_bus_c);
}

void I2cMeasurableManager::InitFlightSystems(const I2cMultiplexer *mux_a) {
    Adc *fs_adc_x = new Adc(bus_a, 0x48, mux_a, I2cMultiplexer::kMuxChannel1);
    fs_adc_x->SetGainAmplifierLevel(kAdc4v096);
    Adc *fs_adc_y = new Adc(bus_a, 0x49, mux_a, I2cMultiplexer::kMuxChannel1);
    fs_adc_y->SetGainAmplifierLevel(kAdc4v096);
    Adc *fs_adc_z = new Adc(bus_a, 0x4B, mux_a, I2cMultiplexer::kMuxChannel1);
    fs_adc_z->SetGainAmplifierLevel(kAdc4v096);

    AddVoltage(kFsMagTorqAX, fs_adc_x, kAdcP1NGnd, 2);
    AddVoltage(kFsMagTorqBX, fs_adc_x, kAdcP2NGnd, 2);
    AddVoltage(kFsMagTorqAY, fs_adc_y, kAdcP1NGnd, 2);
    AddVoltage(kFsMagTorqBY, fs_adc_y, kAdcP2NGnd, 2);
    AddVoltage(kFsMagTorqAZ, fs_adc_z, kAdcP1NGnd, 2);
    AddVoltage(kFsMagTorqBZ, fs_adc_z, kAdcP2NGnd, 2);

    AddCurrent(kFsTorquerCurrentX, fs_adc_x, kAdcP0NGnd, 0.1, 1.65);
    AddCurrent(kFsTorquerCurrentTotal, fs_adc_x, kAdcP3NGnd, 1.0 / 7.5, 0);
    AddCurrent(kFsTorquerCurrentY, fs_adc_y, kAdcP0NGnd, 0.1, 1.65);
    AddCurrent(kFsTorquerCurrentZ, fs_adc_z, kAdcP0NGnd, 0.1, 1.65);

    MCP9808 *fs_temp_hb_x =
        new MCP9808(bus_a, 0x18, mux_a, I2cMultiplexer::kMuxChannel1);
    MCP9808 *fs_temp_hb_y =
        new MCP9808(bus_a, 0x19, mux_a, I2cMultiplexer::kMuxChannel1);
    MCP9808 *fs_temp_hb_z =
        new MCP9808(bus_a, 0x1A, mux_a, I2cMultiplexer::kMuxChannel1);

    AddTemperature(kFsTempHbX, fs_temp_hb_x);
    AddTemperature(kFsTempHbY, fs_temp_hb_y);
    AddTemperature(kFsTempHbZ, fs_temp_hb_z);

    MPU9250MotionTracker *fs_imu_1 = new MPU9250MotionTracker(
        bus_a, 0x68, mux_a, I2cMultiplexer::kMuxChannel1);

    AddImuGyrometerMeasurable(kFsImuGyro1, fs_imu_1);
    AddImuAcceleromterMeasurable(kFsImuAccelerometer1, fs_imu_1);
    AddImuTemperatureMeasurable(kFsImuTemperature1, fs_imu_1);
    double initial_biases_bus_a_data[3][1];
    const Matrix initial_biases_bus_a(
        kPreFlightMagnetometerCalibrationBiasesImuBusA,
        initial_biases_bus_a_data);
    double initial_scale_factors_bus_a_data[3][1];
    const Matrix initial_scale_factors_bus_a(
        kPreFlightMagnetometerCalibrationScaleFactorsImuBusA,
        initial_scale_factors_bus_a_data);
    AddImuMagnetometerMeasurable(
        kFsImuMagnetometer1, fs_imu_1, kImuAToBodyFrameTransform,
        initial_biases_bus_a, initial_scale_factors_bus_a);

    MPU9250MotionTracker *fs_imu_2 = new MPU9250MotionTracker(bus_b, 0x68);

    AddImuGyrometerMeasurable(kFsImuGyro2, fs_imu_2);
    AddImuAcceleromterMeasurable(kFsImuAccelerometer2, fs_imu_2);
    AddImuTemperatureMeasurable(kFsImuTemperature2, fs_imu_2);
    double initial_biases_bus_b_data[3][1];
    const Matrix initial_biases_bus_b(
        kPreFlightMagnetometerCalibrationBiasesImuBusB,
        initial_biases_bus_b_data);
    double initial_scale_factors_bus_b_data[3][1];
    const Matrix initial_scale_factors_bus_b(
        kPreFlightMagnetometerCalibrationScaleFactorsImuBusB,
        initial_scale_factors_bus_b_data);
    AddImuMagnetometerMeasurable(
        kFsImuMagnetometer2, fs_imu_2, kImuBToBodyFrameTransform,
        initial_biases_bus_b, initial_scale_factors_bus_b);
}

void I2cMeasurableManager::InitUtilities(const I2cMultiplexer *mux_c) {
    Adc *util_adc_1 = new Adc(bus_c, 0x49, mux_c, I2cMultiplexer::kMuxChannel1);
}

void I2cMeasurableManager::InitCdh(const I2cMultiplexer *mux_a) {
    MCP9808 *cdh_temp_1 =
        new MCP9808(bus_a, 0x1A, mux_a, I2cMultiplexer::kMuxChannel0);
    AddTemperature(kCdhSysTemp, cdh_temp_1);
}

void I2cMeasurableManager::InitSolarPanels(const I2cMultiplexer *mux_c) {
    MCP9808 *solar_panel_1_temp_1 =
        new MCP9808(bus_c, 0x19, mux_c, I2cMultiplexer::kMuxChannel4);
    MCP9808 *solar_panel_1_temp_2 =
        new MCP9808(bus_c, 0x1A, mux_c, I2cMultiplexer::kMuxChannel4);

    AddTemperature(kPowerPanel1Temp1, solar_panel_1_temp_1);
    AddTemperature(kPowerPanel1Temp2, solar_panel_1_temp_2);

    MCP9808 *solar_panel_2_temp_1 =
        new MCP9808(bus_c, 0x19, mux_c, I2cMultiplexer::kMuxChannel5);
    MCP9808 *solar_panel_2_temp_2 =
        new MCP9808(bus_c, 0x1A, mux_c, I2cMultiplexer::kMuxChannel5);

    AddTemperature(kPowerPanel2Temp1, solar_panel_2_temp_1);
    AddTemperature(kPowerPanel2Temp2, solar_panel_2_temp_2);

    MCP9808 *solar_panel_3_temp_1 =
        new MCP9808(bus_c, 0x19, mux_c, I2cMultiplexer::kMuxChannel6);
    MCP9808 *solar_panel_3_temp_2 =
        new MCP9808(bus_c, 0x1A, mux_c, I2cMultiplexer::kMuxChannel6);

    AddTemperature(kPowerPanel3Temp1, solar_panel_3_temp_1);
    AddTemperature(kPowerPanel3Temp2, solar_panel_3_temp_2);

    MCP9808 *solar_panel_4_temp_1 =
        new MCP9808(bus_c, 0x19, mux_c, I2cMultiplexer::kMuxChannel7);
    MCP9808 *solar_panel_4_temp_2 =
        new MCP9808(bus_c, 0x1A, mux_c, I2cMultiplexer::kMuxChannel7);

    AddTemperature(kPowerPanel4Temp1, solar_panel_4_temp_1);
    AddTemperature(kPowerPanel4Temp2, solar_panel_4_temp_2);

    MCP9808 *solar_panel_5_temp_1 =
        new MCP9808(bus_c, 0x19, mux_c, I2cMultiplexer::kMuxChannel3);
    MCP9808 *solar_panel_5_temp_2 =
        new MCP9808(bus_c, 0x1A, mux_c, I2cMultiplexer::kMuxChannel3);

    AddTemperature(kPowerPanel5Temp1, solar_panel_5_temp_1);
    AddTemperature(kPowerPanel5Temp2, solar_panel_5_temp_2);

    MCP9808 *solar_panel_6_temp_1 =
        new MCP9808(bus_c, 0x1C, mux_c, I2cMultiplexer::kMuxChannel2);

    AddTemperature(kPowerPanel6Temp1, solar_panel_6_temp_1);

    Adc *solar_adc_6 =
        new Adc(bus_c, 0x4B, mux_c, I2cMultiplexer::kMuxChannel2);
    solar_adc_6->SetGainAmplifierLevel(kAdc4v096);

    AddVoltage(kPowerTopPanelVoltage, solar_adc_6, kAdcP0NGnd, 3.0);
    AddVoltage(kPowerTopSolarVoltage, solar_adc_6, kAdcP1NGnd, 3.0);
    AddCurrent(kPowerTopPanelCurrent, solar_adc_6, kAdcP1NGnd, 5.0 / 3, 0);
    AddCurrent(kPowerTopSolarCurrent, solar_adc_6, kAdcP3NGnd, 5.0 / 3, 0);

    Adc *solar_adc_1 =
        new Adc(bus_c, 0x48, mux_c, I2cMultiplexer::kMuxChannel4);
    solar_adc_1->SetGainAmplifierLevel(kAdc4v096);

    AddVoltage(kPowerPanelVoltage1, solar_adc_1, kAdcP0NGnd, 3.0);
    AddVoltage(kPowerSolarVoltage1, solar_adc_1, kAdcP2NGnd, 3.0);
    AddCurrent(kPowerPanelCurrent1, solar_adc_1, kAdcP1NGnd, 0.285714, 0);
    AddCurrent(kPowerSolarCurrent1, solar_adc_1, kAdcP3NGnd, 0.285714, 0);

    Adc *solar_adc_2 =
        new Adc(bus_c, 0x48, mux_c, I2cMultiplexer::kMuxChannel5);
    solar_adc_2->SetGainAmplifierLevel(kAdc4v096);

    AddVoltage(kPowerPanelVoltage2, solar_adc_2, kAdcP0NGnd, 3.0);
    AddVoltage(kPowerSolarVoltage2, solar_adc_2, kAdcP2NGnd, 3.0);
    AddCurrent(kPowerPanelCurrent2, solar_adc_2, kAdcP1NGnd, 0.285714, 0);
    AddCurrent(kPowerSolarCurrent2, solar_adc_2, kAdcP3NGnd, 0.285714, 0);

    Adc *solar_adc_3 =
        new Adc(bus_c, 0x48, mux_c, I2cMultiplexer::kMuxChannel6);
    solar_adc_3->SetGainAmplifierLevel(kAdc4v096);

    AddVoltage(kPowerPanelVoltage3, solar_adc_3, kAdcP0NGnd, 3.0);
    AddVoltage(kPowerSolarVoltage3, solar_adc_3, kAdcP2NGnd, 3.0);
    AddCurrent(kPowerPanelCurrent3, solar_adc_3, kAdcP1NGnd, 0.285714, 0);
    AddCurrent(kPowerSolarCurrent3, solar_adc_3, kAdcP3NGnd, 0.285714, 0);

    Adc *solar_adc_4 =
        new Adc(bus_c, 0x48, mux_c, I2cMultiplexer::kMuxChannel7);
    solar_adc_4->SetGainAmplifierLevel(kAdc4v096);

    AddVoltage(kPowerPanelVoltage4, solar_adc_4, kAdcP0NGnd, 3.0);
    AddVoltage(kPowerSolarVoltage4, solar_adc_4, kAdcP2NGnd, 3.0);
    AddCurrent(kPowerPanelCurrent4, solar_adc_4, kAdcP1NGnd, 0.285714, 0);
    AddCurrent(kPowerSolarCurrent4, solar_adc_4, kAdcP3NGnd, 0.285714, 0);

    Adc *solar_adc_5 =
        new Adc(bus_c, 0x48, mux_c, I2cMultiplexer::kMuxChannel3);
    solar_adc_5->SetGainAmplifierLevel(kAdc4v096);

    AddVoltage(kPowerPanelVoltage5, solar_adc_5, kAdcP0NGnd, 3.0);
    AddVoltage(kPowerSolarVoltage5, solar_adc_5, kAdcP2NGnd, 3.0);
    AddCurrent(kPowerPanelCurrent5, solar_adc_5, kAdcP1NGnd, 0.285714, 0);
    AddCurrent(kPowerSolarCurrent5, solar_adc_5, kAdcP3NGnd, 0.285714, 0);
}

void I2cMeasurableManager::AddVoltage(MeasurableId id, Adc *adc,
                                      AdcMuxMode line, float scaling_factor) {
    CheckValidId(id);
    VoltageMeasurable *voltage =
        new VoltageMeasurable(adc, line, scaling_factor);
    measurables[id] = voltage;
}

void I2cMeasurableManager::AddCurrent(MeasurableId id, Adc *adc,
                                      AdcMuxMode line, float scaling_factor,
                                      float zero_bias_point) {
    CheckValidId(id);
    CurrentMeasurable *current =
        new CurrentMeasurable(adc, line, scaling_factor, zero_bias_point);
    measurables[id] = current;
}

void I2cMeasurableManager::AddTemperature(MeasurableId id,
                                          MCP9808 *temp_sensor) {
    CheckValidId(id);
    TemperatureMeasurable *temp = new TemperatureMeasurable(temp_sensor);
    measurables[id] = temp;
}

void I2cMeasurableManager::AddBmsDieTempMeasurable(MeasurableId id, Bms *bms) {
    CheckValidId(id);
    BmsDieTemperatureMeasurable *temp = new BmsDieTemperatureMeasurable(bms);
    measurables[id] = temp;
}

void I2cMeasurableManager::AddBmsReadingsMeasurable(MeasurableId id, Bms *bms) {
    CheckValidId(id);
    BmsReadingsMeasurable *temp = new BmsReadingsMeasurable(bms);
    measurables[id] = temp;
}

void I2cMeasurableManager::AddImuGyrometerMeasurable(
    MeasurableId id, MPU9250MotionTracker *imu_sensor) {
    CheckValidId(id);
    ImuGyroscopeMeasurable *gyro = new ImuGyroscopeMeasurable(imu_sensor);
    measurables[id] = gyro;
}

void I2cMeasurableManager::AddImuAcceleromterMeasurable(
    MeasurableId id, MPU9250MotionTracker *imu_sensor) {
    CheckValidId(id);
    ImuAccelerometerMeasurable *accelerometer =
        new ImuAccelerometerMeasurable(imu_sensor);
    measurables[id] = accelerometer;
}
void I2cMeasurableManager::AddImuTemperatureMeasurable(
    MeasurableId id, MPU9250MotionTracker *imu_sensor) {
    CheckValidId(id);
    ImuTemperatureMeasurable *temp = new ImuTemperatureMeasurable(imu_sensor);
    measurables[id] = temp;
}
void I2cMeasurableManager::AddImuMagnetometerMeasurable(
    MeasurableId id, MPU9250MotionTracker *imu_sensor,
    const Matrix &frame_mapping, const Matrix &initial_biases,
    const Matrix &initial_scale_factors) {
    CheckValidId(id);
    ImuMagnetometerMeasurable *magnetometer = new ImuMagnetometerMeasurable(
        imu_sensor, frame_mapping, initial_biases, initial_scale_factors);
    measurables[id] = magnetometer;
}
BmsBatteryTemperatureMeasurable *
I2cMeasurableManager::AddBmsBatteryTempMeasurable(MeasurableId id, Bms *bms) {
    CheckValidId(id);
    BmsBatteryTemperatureMeasurable *temp =
        new BmsBatteryTemperatureMeasurable(bms);
    measurables[id] = temp;
    return temp;
}

void I2cMeasurableManager::CheckValidId(MeasurableId id) {
    if (id > kMaxMeasurables - 1) {
        etl::exception e("Measurable ID outside max size", __FILE__, __LINE__);
        throw e;
    } else if (measurables.at(id) != NULL) {
        etl::exception e("ID already used", __FILE__, __LINE__);
        throw e;
    }
}

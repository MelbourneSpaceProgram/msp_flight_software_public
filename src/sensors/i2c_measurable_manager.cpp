#include <src/i2c/bms/bms.h>
#include <src/sensors/i2c_measurable_manager.h>
#include <src/sensors/i2c_sensors/adc.h>
#include <src/sensors/i2c_sensors/measurables/bms_battery_temperature_measurable.h>
#include <src/sensors/i2c_sensors/measurables/bms_die_temperature_measurable.h>
#include <src/sensors/i2c_sensors/measurables/bms_battery_temperature_measurable.h>
#include <src/sensors/i2c_sensors/measurables/rtime_measurable.h>
#include <src/sensors/i2c_sensors/measurables/temperature_measurable.h>
#include <src/sensors/i2c_sensors/measurables/voltage_measurable.h>
#include <src/sensors/measurable_id.h>
#include <src/sensors/i2c_sensors/mcp9808.hpp>

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
    // TODO(dingbenjamin): Implement
    // SCP D0x26
    // Current Sensors

    Adc *comms_adc_1 =
        new Adc(bus_a, 0x48, mux_a, I2cMultiplexer::kMuxChannel3);
    Adc *comms_adc_2 =
        new Adc(bus_a, 0x49, mux_a, I2cMultiplexer::kMuxChannel3);
    Adc *comms_adc_3 =
        new Adc(bus_a, 0x4A, mux_a, I2cMultiplexer::kMuxChannel3);

    MCP9808 *comms_temp_1 =
        new MCP9808(bus_a, 0x18, mux_a, I2cMultiplexer::kMuxChannel3);
    MCP9808 *comms_temp_2 =
        new MCP9808(bus_a, 0x19, mux_a, I2cMultiplexer::kMuxChannel3);

    AddTemperature(kCommsTemp1, comms_temp_1);
    AddTemperature(kCommsTemp2, comms_temp_2);
}

void I2cMeasurableManager::InitPower(const I2cMultiplexer *mux_a) {
    // SCP D0x20
    // BMS 1 D0x68
    // BMS 2 C0x68
    // Current Sensors

    Adc *power_adc_1 =
        new Adc(bus_a, 0x48, mux_a, I2cMultiplexer::kMuxChannel2);
    Adc *power_adc_2 =
        new Adc(bus_a, 0x49, mux_a, I2cMultiplexer::kMuxChannel2);
    Adc *power_adc_3 =
        new Adc(bus_a, 0x4A, mux_a, I2cMultiplexer::kMuxChannel2);

    MCP9808 *power_temp_1 =
        new MCP9808(bus_a, 0x18, mux_a, I2cMultiplexer::kMuxChannel2);
    MCP9808 *power_temp_2 =
        new MCP9808(bus_a, 0x19, mux_a, I2cMultiplexer::kMuxChannel2);

    Bms *power_bms_die_temp_1 =
        new Bms(bus_d, 0x68, NULL, I2cMultiplexer::kMuxNoChannel);
    Bms *power_bms_die_temp_2 =
        new Bms(bus_c, 0x68, NULL, I2cMultiplexer::kMuxNoChannel);

    Bms *power_bms_battery_temp_1 =
        new Bms(bus_d, 0x68, NULL, I2cMultiplexer::kMuxNoChannel);
    Bms *power_bms_battery_temp_2 =
        new Bms(bus_c, 0x68, NULL, I2cMultiplexer::kMuxNoChannel);

    AddTemperature(kPowerTemp1, power_temp_1);
    AddTemperature(kPowerTemp2, power_temp_2);
    AddBmsDieTempMeasurable(kPowerBmsDieTemp1, power_bms_die_temp_1);
    AddBmsDieTempMeasurable(kPowerBmsDieTemp2, power_bms_die_temp_2);
    AddBmsBatteryTempMeasurable(kPowerBmsBatteryTemp1, power_bms_battery_temp_1);
    AddBmsBatteryTempMeasurable(kPowerBmsBatteryTemp2, power_bms_battery_temp_2);
}

void I2cMeasurableManager::InitFlightSystems(const I2cMultiplexer *mux_a) {
    // TODO(dingbenjamin): Implement
    // IMU A0x68
    // SCP D0x21
    // SCP B0x21
    // IMU B0x68
    // Rad B0x20
    // Current Sensors

    Adc *fs_adc_1 = new Adc(bus_a, 0x48, mux_a, I2cMultiplexer::kMuxChannel1);
    Adc *fs_adc_2 = new Adc(bus_a, 0x49, mux_a, I2cMultiplexer::kMuxChannel1);
    Adc *fs_adc_3 = new Adc(bus_a, 0x4A, mux_a, I2cMultiplexer::kMuxChannel1);
    Adc *fs_adc_4 = new Adc(bus_a, 0x4B, mux_a, I2cMultiplexer::kMuxChannel1);

    MCP9808 *fs_temp_1 =
        new MCP9808(bus_a, 0x18, mux_a, I2cMultiplexer::kMuxChannel1);
    MCP9808 *fs_temp_2 =
        new MCP9808(bus_a, 0x19, mux_a, I2cMultiplexer::kMuxChannel1);
    MCP9808 *fs_temp_3 =
        new MCP9808(bus_a, 0x1A, mux_a, I2cMultiplexer::kMuxChannel1);

    AddTemperature(kFsTemp1, fs_temp_1);
    AddTemperature(kFsTemp2, fs_temp_2);
    AddTemperature(kFsTemp3, fs_temp_3);
}

void I2cMeasurableManager::InitUtilities(const I2cMultiplexer *mux_c) {
    // TODO(dingbenjamin): Implement
    // Temp 5 C0x1C
    // Current Sensors

    // TODO(dingbenjamin): Confirm mux line
    Adc *util_adc_2 = new Adc(bus_c, 0x49, mux_c, I2cMultiplexer::kMuxChannel1);

    MCP9808 *util_temp_1 =
        new MCP9808(bus_a, 0x1C, mux_c, I2cMultiplexer::kMuxChannel1);
    AddTemperature(kUtilTemp1, util_temp_1);
}

void I2cMeasurableManager::InitCdh(const I2cMultiplexer *mux_a) {
    // TODO(dingbenjamin): Implement
    // RTC A0x69
    // SCP D0x22
    // Current Sensors

    Adc *cdh_adc_1 = new Adc(bus_a, 0x48, mux_a, I2cMultiplexer::kMuxChannel0);
    Adc *cdh_adc_2 = new Adc(bus_a, 0x49, mux_a, I2cMultiplexer::kMuxChannel0);

    Rtc *rtc = new Rtc(bus_a, 0x69, mux_a, I2cMultiplexer::kMuxChannel0);
    RTimeMeasurable *rtime_measurable = new RTimeMeasurable(rtc);
    measurables[kCdhRtc] = rtime_measurable;

    MCP9808 *cdh_temp_1 =
        new MCP9808(bus_a, 0x1A, mux_a, I2cMultiplexer::kMuxChannel0);
    AddTemperature(kCdhTemp1, cdh_temp_1);
}

void I2cMeasurableManager::InitSolarPanels(const I2cMultiplexer *mux_c) {
    // TODO(dingbenjamin): Implement
    // Temp 1 C0x19
    // Temp 2 C0x1A
    // IR 1 C0x5B
    // IR 2 C0x5C
    // Rad ??
    // Current Sensors

    // TODO(dingbenjamin): Confirm mux line
    Adc *solar_adc_1 =
        new Adc(bus_c, 0x48, mux_c, I2cMultiplexer::kMuxChannel7);

    MCP9808 *solar_temp_1 =
        new MCP9808(bus_a, 0x19, mux_c, I2cMultiplexer::kMuxChannel7);
    MCP9808 *solar_temp_2 =
        new MCP9808(bus_a, 0x1A, mux_c, I2cMultiplexer::kMuxChannel7);

    AddTemperature(kSolarTemp1, solar_temp_1);
    AddTemperature(kSolarTemp2, solar_temp_2);
}

void I2cMeasurableManager::AddVoltage(MeasurableId id, Adc *adc,
                                      AdcMuxMode line) {
    CheckValidId(id);
    VoltageMeasurable *voltage = new VoltageMeasurable(adc, line);
    measurables[id] = voltage;
}

void I2cMeasurableManager::AddTemperature(MeasurableId id,
                                          MCP9808 *temp_sensor) {
    CheckValidId(id);
    TemperatureMeasurable *temp = new TemperatureMeasurable(temp_sensor);
    measurables[id] = temp;
}

void I2cMeasurableManager::AddBmsDieTempMeasurable(MeasurableId id,
                                                   Bms *temp_sensor) {
    CheckValidId(id);
    BmsDieTemperatureMeasurable *temp =
        new BmsDieTemperatureMeasurable(temp_sensor);
    measurables[id] = temp;
}

void I2cMeasurableManager::AddBmsBatteryTempMeasurable(MeasurableId id,
                                                       Bms *temp_sensor) {
    CheckValidId(id);
    BmsBatteryTemperatureMeasurable *temp =
        new BmsBatteryTemperatureMeasurable(temp_sensor);
    measurables[id] = temp;
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

bool I2cMeasurableManager::CheckValidMeasurable(MeasurableId id) {
  return (id < kMaxMeasurables - 1) && (measurables.at(id) != NULL);
}

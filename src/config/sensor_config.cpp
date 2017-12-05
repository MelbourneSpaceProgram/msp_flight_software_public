#include <src/config/sensor_config.hpp>
#include <src/i2c/i2c_bus.hpp>
#include <src/i2c/sensors/mcp9808.hpp>
#include <src/observers/sample_temp_observer.hpp>
#include <Board.h>

SensorConfig::SensorConfig() {
  // Set built false initially.
  built = false;
}

bool SensorConfig::config_built() {
  return built;
}

SensorConfig* SensorConfig::instance() {
  // Create the instance.
  static SensorConfig instance;

  // Build config unless already built.
  if (!instance.config_built()) {
    instance.build_config();
  }

  return &instance;
}

void SensorConfig::build_config() {

  // Create global I2C configuration.
  I2CConfiguration i2c_config(I2CConfiguration::BLOCKING_MODE, I2CConfiguration::RATE_400_KHZ);

  // Create bus 1.
  I2CBus bus_1(&i2c_config, Board_I2C_TMP);

  // Open bus 1.
  bus_1.open();

  // Create h-bridge temperature sensor 1.
  MCP9808 hbridge_temp_1(&bus_1, 0x18, "HBRIDGE_TEMP_1");
  // Create and add observers.
  SampleTempObserver observer;
  hbridge_temp_1.add_observer(&observer);
  // Add sensor to the config.
  config["HBRIDGE_TEMP_1"] = &hbridge_temp_1;
}

I2CSensor* SensorConfig::by_code(string code) {
  return config[code];
}

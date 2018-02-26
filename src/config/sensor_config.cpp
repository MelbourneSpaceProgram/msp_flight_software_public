#include <src/config/sensor_config.hpp>
#include <Board.h>
#include <src/i2c/i2c.h>
#include <string>
#include <src/i2c/sensors/mcp9808.hpp>
#include <src/observers/specific_observers/sample_temp_observer.hpp>

SensorConfig::SensorConfig() {
    // Set built false initially.
    built = false;
}

bool SensorConfig::config_built() { return built; }

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
    // Create bus 1.
    I2c bus_1(Board_I2C_TMP);

    // Create h-bridge temperature sensor 1.
    MCP9808 hbridge_temp_1(&bus_1, 0x18, "HBRIDGE_TEMP_1");
    // Create and add observers.
    SampleTempObserver observer;
    hbridge_temp_1.AddObserver(&observer);
    // Add sensor to the config.
    config["HBRIDGE_TEMP_1"] = &hbridge_temp_1;
}

I2CSensor* SensorConfig::by_code(std::string code) { return config[code]; }

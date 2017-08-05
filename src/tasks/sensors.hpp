#ifndef _SENSORS_H_
#define _SENSORS_H_

#include <src/i2c/i2c_configuration.hpp>
#include <src/i2c/i2c_bus.hpp>
#include <src/i2c/sensors/mcp9808.hpp>

void InitMCP9808ReadTask(void);

#endif // _SENSORS_H_

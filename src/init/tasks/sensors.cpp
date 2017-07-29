#include <src/init/tasks/sensors.hpp>
#include <src/public_headers/init/init.hpp>
#include <Board.h>

void SensorsTask() {

  while(true) {
    I2CConfiguration i2c_config(I2CConfiguration::BLOCKING_MODE, I2CConfiguration::RATE_400_KHZ);

  I2CBus i2c_bus(&i2c_config, Board_I2C_TMP);

  i2c_bus.open();

  MCP9808 mcp9808(&i2c_bus, 0x18);

  double reading = mcp9808.get_reading();

  i2c_bus.close();
  }
  

}

void InitSensorsTask() {
  Task_Params taskParams;
  Task_Params_init(&taskParams);
  taskParams.stackSize = TASKSTACKSIZE;
  taskParams.instance->name = "sensors";
  taskParams.stack = &sensors_stack;
  taskParams.priority = 2;

  Task_construct(&sensors_task_struct, (Task_FuncPtr) SensorsTask,
                 &taskParams, NULL);
  Task_Handle task = Task_handle(&sensors_task_struct);
}

#include <src/tasks/sample_sensor_read_task.hpp>
#include <src/config/sensor_config.hpp>
#include <src/init/init.h>
#include <xdc/std.h>
#include <xdc/runtime/Log.h>

Task_Struct mcp9808_read_task_struct;
Char mcp9808_stack[TASKSTACKSIZE];

void SampleSensorReadTask() {
  
  // Get the sensor config object.
  SensorConfig* config = SensorConfig::instance();

  // Load the h-bridge sensor.
  I2CSensor* hbridge_temp_1 = config->by_code("HBRIDGE_TEMP_1");

  while(true) {
    // Read from the sensor.
    hbridge_temp_1->take_reading();

    // Perform non-blocking sleep for 1 second.
    Task_sleep((UInt) 1000);
  }
}

void InitSampleSensorReadTask() {
  Task_Params taskParams;
  Task_Params_init(&taskParams);
  taskParams.stackSize = TASKSTACKSIZE;
  taskParams.instance->name = "sensors";
  taskParams.stack = &mcp9808_stack;
  taskParams.priority = 2;

  Task_construct(&mcp9808_read_task_struct, (Task_FuncPtr) SampleSensorReadTask,
                 &taskParams, NULL);
  Task_Handle task = Task_handle(&mcp9808_read_task_struct);
}

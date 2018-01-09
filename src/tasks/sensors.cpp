#include <Board.h>
#include <src/init/init.h>
#include <src/i2c/i2c_bus.hpp>
#include <src/i2c/i2c_configuration.hpp>
#include <src/i2c/sensors/mcp9808.hpp>
#include <src/observers/sample_temp_observer.hpp>
#include <src/tasks/sensors.hpp>
#include <string>

Task_Struct mcp9808_read_task_struct;
Char mcp9808_stack[TASKSTACKSIZE];

void MCP9808ReadTask() {
    while (true) {
        I2CConfiguration i2c_config(I2CConfiguration::BLOCKING_MODE,
                                    I2CConfiguration::RATE_400_KHZ);

        I2CBus i2c_bus(&i2c_config, Board_I2C_TMP);

        i2c_bus.open();

        MCP9808 mcp9808(&i2c_bus, 0x18, "SENSOR123");

        SampleTempObserver sto;

        mcp9808.add_observer(&sto);

        mcp9808.take_reading();

        double reading = mcp9808.get_reading();

        string id = mcp9808.get_id();

        i2c_bus.close();

        Task_sleep((UInt)1000);
    }
}

void InitMCP9808ReadTask() {
    Task_Params taskParams;
    Task_Params_init(&taskParams);
    taskParams.stackSize = TASKSTACKSIZE;
    taskParams.instance->name = "sensors";
    taskParams.stack = &mcp9808_stack;
    taskParams.priority = 2;

    Task_construct(&mcp9808_read_task_struct, (Task_FuncPtr)MCP9808ReadTask,
                   &taskParams, NULL);
    Task_Handle task = Task_handle(&mcp9808_read_task_struct);
}

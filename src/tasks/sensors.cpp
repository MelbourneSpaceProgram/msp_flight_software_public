#include <src/tasks/sensors.hpp>
#include <Board.h>
#include <src/i2c/i2c.h>
#include <src/init/init.h>
#include <string>
#include <src/sensors/i2c_sensors/mcp9808.hpp>
#include <src/observers/specific_observers/sample_temp_observer.hpp>

Task_Struct mcp9808_read_task_struct;
Char mcp9808_stack[TASKSTACKSIZE];

void MCP9808ReadTask() {
    while (true) {
        I2c i2c_bus(Board_I2C_TMP);

        MCP9808 mcp9808(&i2c_bus, 0x18, "SENSOR123");

        SampleTempObserver sto;

        mcp9808.AddObserver(&sto);

        mcp9808.TakeReading();

        double reading = mcp9808.GetReading();

        std::string id = mcp9808.GetId();


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

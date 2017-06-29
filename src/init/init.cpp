#include <src/public_headers/init/init.hpp>

void init_core()
{
    Board_initGeneral();
    Board_initGPIO();
    Board_initI2C();
    Board_initSPI();
    Board_initUART();
}

void init_time()
{
    // Set the reference time to 2017/01/01 00:00 UTC
    SatelliteTime::set_reference_time(((uint64_t) 1483228800000));
}

void init_logger()
{

}

int _tmain();

void init_diagnostics()
{
    initHeartbeat();
}

void init_satellite()
{
    Task_Params taskParams;
    Task_Params_init(&taskParams);
    taskParams.stackSize = TASKSTACKSIZE;
    taskParams.instance->name = "allocator_request";
    taskParams.stack = &allocatorTaskStack;
    taskParams.priority = 2;
    taskParams.arg0 = 1;
    Task_construct(&allocatorTaskStruct, (Task_FuncPtr) _tmain,
                   &taskParams, NULL);
    Task_Handle task = Task_handle(&allocatorTaskStruct);
}

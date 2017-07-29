/**
 * @file
 */

#include <src/public_headers/init/init.hpp>
#include <src/init/tasks.hpp>

/**
 * Initialises the core MSP432 drivers provided by TI. Should be called once at system startup, and prior to the BIOS starting.
 */
void init_core()
{
    Board_initGeneral();
    Board_initGPIO();
    Board_initI2C();
    Board_initSPI();
    Board_initUART();
}

/**
 * Initialises the system time. The default epoch is 2017/01/01 00:00 UTC. The actual system time can be set via the SatelliteTiming module.
 */
void init_time()
{
    // Set the current time to 2017/01/01 00:00 UTC
    SatelliteTime::set_utc_time(((uint64_t) 1483228800000));
}

/**
 * Initialises the logging output of the satellite. Not yet written.
 */
void init_logger()
{

}

void test_time(){
    while(1){
        uint64_t curr = SatelliteTime::get_utc_time();
        //System_printf("%hu\n", curr);
        //System_flush();
        //Task_sleep(1);
    }
}
//int _tmain();

/**
 * Initialises the diagnostic output of the satellite. Currently this is just the heartbeat LED, although it may be expanded to include self-tests.
 */
void init_diagnostics()
{
    initHeartbeat();
}

/**
 * Starts the main process running on the satellite, the task scheduler loop. Should only be called once all other initialisation tasks have completed successfully.
 */
void init_satellite()
{
    Task_Params taskParams;
    Task_Params_init(&taskParams);
    taskParams.stackSize = TASKSTACKSIZE;
    taskParams.instance->name = "allocator_request";
    taskParams.stack = &allocatorTaskStack;
    taskParams.priority = 2;
    taskParams.arg0 = 1;


    Task_construct(&allocatorTaskStruct, (Task_FuncPtr) test_time,
                   &taskParams, NULL);
    Task_Handle task = Task_handle(&allocatorTaskStruct);

    InitTasks();
}

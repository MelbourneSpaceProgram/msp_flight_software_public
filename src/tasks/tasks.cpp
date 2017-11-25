#include <src/tasks/tasks.hpp>
#include <xdc/std.h>
#include <ti/sysbios/knl/Task.h>

#include <src/uart/uart_configuration.hpp>
#include <src/uart/uart.hpp>

#include <src/CDH/util/DebugStream.h>

#include <src/messages/TemperatureMessage.h>
#include "Board.h"
#define UARTA0 MSP_EXP432P401R_UARTA0
#define UARTA2 MSP_EXP432P401R_UARTA2

Task_Struct debugStream_task_struct;
Char debugStream_stack[2048];

void *debugStream(){
    DebugStream debugStream;


    while(1){
        // Generate some various messages
        TemperatureMessage msg(229, 33, 44);
        SerialisedMessage serialMsg = msg.serialise();
        debugStream.sendMessage(serialMsg);
        Task_sleep(50);
    }
}
void InitTasks() {

    Task_Params taskParams;
    Task_Params_init(&taskParams);
    taskParams.stackSize = 2048; // TODO
    taskParams.instance->name = "debugStream";
    taskParams.stack = &debugStream_stack;
    taskParams.priority = 6;

    Task_construct(&debugStream_task_struct, (Task_FuncPtr) debugStream, &taskParams, NULL);
    Task_Handle task = Task_handle(&debugStream_task_struct);

    InitMCP9808ReadTask();
}

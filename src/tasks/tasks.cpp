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

Task_Struct messageGenerator_task_struct;
Char messageGenerator_stack[2048];

void *debugStream(){
    DebugStream debugStream;

    while(1){
        MessageQueue::getInstance().dispatchMessages();
        Task_sleep(5000);
    }
}

void *messageGenerator(){
    // Now generate some fake debug messages
    TemperatureMessage msg(229, 33, 44);

    TemperatureMessage *ptMsg;
    ptMsg = &msg;

    while(true){
        MessageQueue::getInstance().pushMessage( new TemperatureMessage(229, 33, 44) );

        // TODO While this 'works', I do not believe it is valid.
        // Need to research using memory pools etc to allow an effective heap
        MessageQueue::getInstance().pushMessage( ptMsg );

        // Set our max refresh rate
        Task_sleep(10000);
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

    Task_Params messageGeneratorTaskParams;
    Task_Params_init(&messageGeneratorTaskParams);
    messageGeneratorTaskParams.stackSize = 2048; // TODO
    messageGeneratorTaskParams.instance->name = "messageGenerator";
    messageGeneratorTaskParams.stack = &messageGenerator_stack;
    messageGeneratorTaskParams.priority = 2;

    Task_construct(&messageGenerator_task_struct, (Task_FuncPtr) messageGenerator, &messageGeneratorTaskParams, NULL);
    Task_Handle msgTask = Task_handle(&messageGenerator_task_struct);


    InitMCP9808ReadTask();
}

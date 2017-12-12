#include <src/tasks/tasks.hpp>
#include <xdc/std.h>
#include <ti/sysbios/knl/Task.h>
#include <src/CDH/util/DebugStream.h>
#include <src/messages/SerialisedMessage.h>
#include <src/messages/TestMessage.h>
#include <src/CDH/util/SerialisedMessageBuilder.h>
#include "Board.h"
#define UARTA0 MSP_EXP432P401R_UARTA0
#define UARTA2 MSP_EXP432P401R_UARTA2

Task_Struct debugStream_task_struct;
Char debugStream_stack[2048];

void InitTasks() {

    Task_Params taskParams;
    Task_Params_init(&taskParams);
    taskParams.stackSize = 2048; // TODO
    taskParams.instance->name = "debugStream";
    taskParams.stack = &debugStream_stack;
    taskParams.priority = 6;

    Task_construct(&debugStream_task_struct, (Task_FuncPtr) DebugStream::InitDebugStream, &taskParams, NULL);
    Task_Handle task = Task_handle(&debugStream_task_struct);

    //InitMCP9808ReadTask();
}

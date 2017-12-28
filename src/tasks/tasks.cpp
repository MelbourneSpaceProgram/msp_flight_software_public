#include <src/CDH/util/debug_stream.h>
#include <src/messages/serialised_message.h>
#include <src/messages/serialised_message_builder.h>
#include <src/messages/test_message.h>
#include <ti/sysbios/knl/Task.h>
#include <xdc/std.h>
#include <src/tasks/tasks.hpp>
#include "Board.h"
#define UARTA0 MSP_EXP432P401R_UARTA0
#define UARTA2 MSP_EXP432P401R_UARTA2

Task_Struct debugStream_task_struct;
Char debugStream_stack[4096];

void InitTasks() {
    Task_Params taskParams;
    Task_Params_init(&taskParams);
    taskParams.stackSize = 4096;
    taskParams.instance->name = "debugStream";
    taskParams.stack = &debugStream_stack;
    taskParams.priority = 6;

    Task_construct(&debugStream_task_struct,
                   (Task_FuncPtr)DebugStream::InitTestDebugStream, &taskParams,
                   NULL);
    Task_Handle task = Task_handle(&debugStream_task_struct);

    // InitMCP9808ReadTask();
}

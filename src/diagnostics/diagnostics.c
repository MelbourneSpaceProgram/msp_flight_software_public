/*
 * diagnostics.c
 *
 *  Created on: 2Mar.,2017
 *      Author: Anthony
 */

#include "src/hal/hal.h"
#include "diagnostics.h"

Task_Struct heartbeatTaskStruct;
Char heartbeatTaskStack[TASKSTACKSIZE];

int initHeartbeat()
{
    // Blink a LED every half second, so we can see whether the device is functioning, and not stalling.

    /* Construct heartBeat Task  thread */
    Task_Params taskParams;
    Task_Params_init(&taskParams);
    taskParams.instance->name = "heartbeat"; // This doesn't appear to do what I expected
    taskParams.arg0 = HEARTBEATFREQ * 1000 / 2; // Concert cycles to milliseconds, and halve.
    taskParams.stackSize = TASKSTACKSIZE;
    taskParams.priority = 15;
    taskParams.stack = &heartbeatTaskStack;

    // Start the task
    Task_construct(&heartbeatTaskStruct, (Task_FuncPtr) heartbeatFxn, &taskParams, NULL);

    return 0;
}


Void heartbeatFxn(UArg arg0, UArg arg1)
{
    while (1) {
        msp_GPIO_toggle(HEARTBEATPIN);
        Task_sleep((UInt)arg0);
    }
}

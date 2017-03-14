/*
 * Copyright (c) 2015, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 *  ======== empty.c ========
 */
/* XDCtools Header files */
#include <xdc/std.h>
#include <xdc/runtime/System.h>
#include <xdc/runtime/Memory.h>

/* BIOS Header files */
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Semaphore.h>
#include <ti/sysbios/knl/Queue.h>
#include <ti/sysbios/knl/Mailbox.h>

/* TI-RTOS Header files */
#include <ti/drivers/GPIO.h>
#include <ti/drivers/I2C.h>
#include <ti/drivers/SDSPI.h>
#include <ti/drivers/SPI.h>
#include <ti/drivers/UART.h>
//#include <ti/drivers/Watchdog.h>

/* Board Header file */
#include "Board.h"

#include "src/diagnostics/diagnostics.h"
#include "src/hal/hal.h"


#define TASKSTACKSIZE   512

Task_Struct mbx0TaskStruct;
Char mbx0TaskStack[TASKSTACKSIZE];

Task_Struct mbx1TaskStruct;
Char mbx1TaskStack[TASKSTACKSIZE];

Task_Struct mbx2TaskStruct;
Char mbx2TaskStack[TASKSTACKSIZE];

// The mailbox
Mailbox_Handle mbx0;

Task_Handle reader1Handle, reader2Handle;

Void writer(UArg arg1, UArg arg2);
Void reader(UArg arg1, UArg arg2);


// Just MailBox things

#define NUMMSGS     8   /* number of messages */
#define TIMEOUT     10  /* timeout, in system ticks */

typedef struct MsgObj {
    Int id;             /* writer task id */
    Int val;            /* message value */
} MsgObj, *Msg;


/*
 *  ======== main ========
 */
int main(void)
{

    /* Call board init functions */
    Board_initGeneral();
    Board_initGPIO();
    Board_initI2C();
    Board_initSDSPI();
    Board_initSPI();
    Board_initUART();

    initHeartbeat();




    // Mailbox tests
    /* Construct heartBeat Task  thread */
    Task_Params taskParams;
    Task_Params_init(&taskParams);
    taskParams.instance->name = "heartbeat"; // This doesn't appear to do what I expected
    taskParams.arg0 = 1000; // Concert cycles to milliseconds, and halve.
    taskParams.stackSize = TASKSTACKSIZE;

    /* Turn on user LED */
    GPIO_write(Board_LED0, Board_LED_ON);
    GPIO_write(Board_LED1, Board_LED_ON);

    System_printf("Starting the example\nSystem provider is set to SysMin. "
                  "Halt the target to view any SysMin contents in ROV.\n");
    /* SysMin will only print to the console when you call flush or exit */
    System_flush();

    /* Start BIOS */
    BIOS_start();

    return (0);
}


// IPC tests

Void writer(UArg id, UArg arg2)
{
    MsgObj msg;
    Int i;

    for(i = 0; i < 10; i++){
        msg.id = id;
        msg.val = i;

        System_printf("Attempting to send message %d\n", msg.val);

        System_flush();
        Mailbox_post(mbx0, &msg, TIMEOUT);

        if(i == 5){
            Task_setPri(reader1Handle, -1);
            Task_setPri(reader2Handle, -1);
        }
    }
}

Void reader(UArg arg1, UArg arg2)
{
    MsgObj msg;
    Int i;

    System_printf("called reader\n");

    while(1){
        if (Mailbox_pend(mbx0, &msg, TIMEOUT) == 0){
            System_printf("Reader: Mailbox pending timeout\n");
            System_flush();
            break;
        }

        System_printf("Read from the mailbox %d \n", msg.val);
        System_flush();
    }
}

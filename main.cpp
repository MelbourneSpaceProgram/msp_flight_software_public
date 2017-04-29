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

/* XDCtools Header files */
#include <xdc/std.h>
#include <xdc/runtime/System.h>
#include <xdc/runtime/Memory.h>

/* BIOS Header files */
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Semaphore.h>


/* TI-RTOS Header files */
#include <ti/drivers/GPIO.h>
#include <ti/drivers/I2C.h>
#include <ti/drivers/SDSPI.h>
#include <ti/drivers/SPI.h>
#include <ti/drivers/UART.h>
#include <ti/drivers/Watchdog.h>
#include <ti/display/Display.h>

/* Board Header file */
#include "Board.h"

#include <src/public_headers/diagnostics.hpp>
#include <src/public_headers/hal.hpp>
#include <src/public_headers/systems.hpp>
#include <src/public_headers/allocator.hpp>

#include <xdc/runtime/Log.h>   // For Log_warning1("Warning number #%d", 4); things
#include <xdc/runtime/Diags.h> // For Log_print0(Diags_USER1, "hello"); things.
#define TASKSTACKSIZE   8096

Task_Struct allocatorTaskStruct;
Char allocatorTaskStack[TASKSTACKSIZE];


/*
 *  ======== main ========
 */


int main(void)
    {

    // Call board init functions
    Board_initGeneral();
    Board_initGPIO();
    Board_initI2C();
    Board_initSDSPI();
    Board_initSPI();
    Board_initUART();
    Display_init();

    initHeartbeat();

    Task_Params taskParams;
    Task_Params_init(&taskParams);
    taskParams.stackSize = TASKSTACKSIZE;
    taskParams.instance->name = "allocator_request";
    taskParams.stack = &allocatorTaskStack;
    taskParams.priority = 2;
    taskParams.arg0 = 1000;
    Task_construct(&allocatorTaskStruct, (Task_FuncPtr) handle_new_request, &taskParams, NULL);

    // Obtain instance handle
    Task_Handle task = Task_handle(&allocatorTaskStruct);

    // Run the init's

    Log_info0("Log in main");
    // Start BIOS
    BIOS_start();

    return (0);
}

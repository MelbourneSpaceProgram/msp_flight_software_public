//
// Created by Anthony on 23/03/2017.
//

#ifndef CDH_SOFTWARE_ALLOCATOR_H
#define CDH_SOFTWARE_ALLOCATOR_H

#include <xdc/std.h>
#include <xdc/runtime/System.h>
#include <xdc/runtime/Memory.h>

/* BIOS Header files */
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Semaphore.h>

#define MAX_EVENTS_REQUESTED 10
#define MAX_EVENTS_ALLOCATED 100
#define MAX_DEPENDENCIES 100
#define ALLOCATOR_TIMESTEP 60000 // 1 minute time step
#define ALLOCATOR_SIZE 10 // 90 minutes worth of power allocation
#define MAX_ALLOCATION_ATTEMPTS 10
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

typedef enum {REQUEST_ACCEPTED, REQUEST_DENIED} allocator_status_type;

typedef enum {SUCCESS, FAILURE} status_type;

void handle_new_request(UArg arg0, UArg arg1);

#endif //CDH_SOFTWARE_ALLOCATOR_H



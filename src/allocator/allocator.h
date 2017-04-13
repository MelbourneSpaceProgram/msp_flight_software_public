//
// Created by Anthony on 23/03/2017.
//

#ifndef CDH_SOFTWARE_ALLOCATOR_H
#define CDH_SOFTWARE_ALLOCATOR_H

#ifdef __cplusplus

#include "src/system.h"

#include <xdc/std.h>
#include <xdc/runtime/System.h>
#include <xdc/runtime/Memory.h>

/* BIOS Header files */
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Semaphore.h>

#define MAX_DEPENDENCIES 5
#define MAX_ALLOCATION_ATTEMPTS 10
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

//#include "power_allocator.c"
#include "../util/status.h"

typedef enum
{
	REQUEST_ACCEPTED, REQUEST_DENIED
} allocator_status_type;


typedef struct request
{
	system_type system_id;
	int subsystem_id; // The subsystem that is requesting this event
	int request_id; // A unique identifier. This is only unique in the context of a subsystem, so subsystem_id and request_id is required to globally identify.
	int start_time; // Some system definition of time. Might be number of microseconds since something?
	int end_time;
	int priority; // The priority of this task, from a range 0 - X.
	int dependencies[MAX_DEPENDENCIES]; // All the sub tasks that need to be created for this task. Need to work out the maximum size.
	void (*cancel_callback)(); // Function to call if this event is cancelled. Should we also call it if the event is not accepted?
	void (*success_callback)(); // Function to call if this event succeeds.
	void (*failure_callback)(); // Function to call if this event fails.
	int duration; // Duration of this event in milliseconds?
	int type; // The type of event. Maybe this shouldn't be type SUBSYSTEM?
	float power_peak; // Peak power draw expected.
	float power_average; // Average power draw expected.
} request_type;


class Schedule
{
    static const int MAX_SCHEDULE_SIZE = 10;
    request_type requests[MAX_SCHEDULE_SIZE];
    int size;
public:
    Schedule(void);
    int get_size(void);
    request_type get_request(int);
    request_type get_requests(void);
    status_type add(request_type);
    status_type remove(request_type);
    status_type clear(void);
    void print(void);
};



void handle_new_request(UArg arg0, UArg arg1);

allocator_status_type allocator(request_type request);
request_type find_lowest_priority_overlap(request_type request, Schedule schedule);
void handle_new_request(UArg arg0, UArg arg1);
void init_allocator();
void print_request(request_type request);
void print_schedule();
int requests_equal(request_type a, request_type b);
int requests_overlap(request_type a, request_type b);
status_type run_allocator();
void telecomms_cancel_callback();
void telecomms_failure_callback();
request_type telecomms_requests();
void telecomms_success_callback();

#endif
#endif //CDH_SOFTWARE_ALLOCATOR_H

#include <xdc/std.h>
#include <ti/sysbios/knl/Task.h>
#include <xdc/runtime/System.h>
#include <xdc/runtime/Memory.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include <src/public_headers/systems.hpp>

#include <src/public_headers/allocator.hpp>
#include <src/CDH/allocator/allocator_p.hpp>
#include <src/public_headers/schedule.hpp>

#include <src/public_headers/housekeeping.hpp>
#include <src/public_headers/logger.h>
//#include <src/CDH/allocator/schedule_p.hpp>

#define TASKSTACKSIZE 2048
Task_Struct taskStructs[5];
Char taskStacks[5][TASKSTACKSIZE];
int taskCounter = 0;




// Depending on how we can stretch function pointers, this might be all we need to implement the allocator.
// Seeing as typical pointers are okay, I think we could experiment with passing in a function pointer, and then a pointer to a struct that contains all of the data necessary to run?
void create_task(){

    // Request tasks from each subsystem.
    request_short request = CDH_requests();

    Task_Handle handle;
    // TODO magic number
    if (taskCounter < 5){
        Task_Params taskParams;
        Task_Params_init(&taskParams);
        taskParams.stackSize = TASKSTACKSIZE;
        taskParams.instance->name = "dynamic_task";
        taskParams.stack = &taskStacks[taskCounter];
        taskParams.priority = 4;
        taskParams.arg0 = request.period;
        taskParams.arg1 = request.args;
        handle = Task_create((Task_FuncPtr) request.func, &taskParams, NULL);
        CDH_task_callback(handle);
        taskCounter++;
    }
}

int requests_equal(request_long_type a, request_long_type b)
{
	int is_equal = 0;
	if (a.request_id == b.request_id)
	{
		is_equal = 1;
	}

	return is_equal;
}

int requests_overlap(request_long_type a, request_long_type b)
{
	return (a.start_time > b.start_time ? a.start_time : b.start_time)
			<= (a.end_time < b.end_time ? a.end_time : b.end_time);
}

request_long_type telecomms_requests()
{
	request_long_type request;
	request.system_id = TELECOM;
	request.subsystem_id = 3;
	request.request_id = rand() % 1000;
	request.duration = rand() % 120000;
	request.start_time = rand() % 599999; // 10 seconds from now. Should we use relative or absolute times?
	request.end_time = request.start_time + request.duration;
	request.priority = rand() % 16; // On a 1-16 scale, we are about an 8
	request.dependencies[0] = 1;
	request.dependencies[1] = 22;
	request.dependencies[2] = 44;
	// = {1, 2, 3}; // These refer to other subsystems
	request.type = 10;
	request.power_peak = 3;
	request.power_average = 1.2;

	return request;
}



request_long_type find_lowest_priority_overlap(request_long_type request, Schedule schedule)
{

	request_long_type lowest_request;
	int lowest_priority = 1000; // TODO The maximum priority
	for (int i = 0; i < schedule.get_size(); i++)
	{
		if (requests_overlap(request, schedule.get_request(i)))
		{
			if (schedule.get_request(i).priority < lowest_priority)
			{
				lowest_request = schedule.get_request(i);
				lowest_priority = schedule.get_request(i).priority;
			}
		}
	}

	return lowest_request;
}

void print_request(request_long_type request)
{

     char   uartLog_outBuf[300 + 4];
    System_snprintf(uartLog_outBuf, 290, "Request %d from system %d-%d, start %d end %d priority %d\n",request.request_id, request.system_id, request.subsystem_id, request.start_time, request.end_time, request.priority);
	//log_info(uartLog_outBuf);
    return;
}

// Main allocator task
allocator_status_type allocator(request_long_type request)
{

    static Schedule schedule; // Init schedule TODO This works?

    schedule.print(); // TODO Check this isn't empty

	allocator_status_type request_acceptance_status = ALLOCATOR_REQUEST_DENIED; // Assume the implicit state is deny.

	// To determine if we can allocate an event or not, we need to check for resource availability.
	// We need to iterate through every already scheduled event occurring in this duration, and calculate the average power consumption per time-step.

	print_request(request);

	// For the given event, we need to ensure it can satisfy all system resource requirements
	int can_be_allocated = 1; // We assume the event be allocated, unless we find out otherwise

	// ALEX module will go here, doesn't need to be called function allocator
	System_printf("\n power allocator being called: can_be_allocated = %d\n", can_be_allocated);
	System_flush();

	if(1){
	//if(power_allocator(request, schedule) == REQUEST_ACCEPTED){
		can_be_allocated = 1;
	}
	else{
		can_be_allocated = 0;
	}
	System_printf("\n power allocator called: can_be_allocated = %d \n", can_be_allocated);
	System_flush();
	can_be_allocated &= rand() % 2; // TODO This could be where the pointing resource allocator provides its output

	if (!can_be_allocated)
	{
		System_printf("Attempting to optimise the event allocation\n\n\n");


		request_long_type removed_events[10];
		int n_removed_events = 0;

		// We bound the number of allocation attempts we can make
		for (int attempt_counter = 0;
				attempt_counter < MAX_ALLOCATION_ATTEMPTS && !(can_be_allocated);
				attempt_counter++)
		{

			// Find the index of the event with the lowest priority, that overlaps with the event we need to schedule
			request_long_type lowest_request = find_lowest_priority_overlap(request, schedule);

			if (lowest_request.system_id == EMPTY)
			{
				System_printf("Improving situation is impossible\n");
				break;
			}

			// Store this event for now, in case we can add it again later.
			// We will also use this array to notify the subsystems that their event was cancelled.
			removed_events[n_removed_events] = lowest_request;
			n_removed_events++;

			// Remove from the allocation
			schedule.remove(lowest_request);

			// Attempt to schedule the requested event again, now that we have removed the least important event
			can_be_allocated &= 1;
		}
	}

	if (can_be_allocated)
	{


		if (schedule.add(request))
		{
			request_acceptance_status = ALLOCATOR_REQUEST_ACCEPTED;
		}
		else
		{
			request_acceptance_status = ALLOCATOR_REQUEST_DENIED;
		}
	}

	System_flush();
	return request_acceptance_status;
}

void init_allocator()
{
	System_printf("Allocator init complete\n");
	System_flush();
}

allocator_status_type run_allocator()
{

	srand(time(NULL));

	request_long_type request = telecomms_requests();
	allocator(request);

	System_printf("Printing allocated requests\n");
	System_flush();
	//print_schedule();

	return ALLOCATOR_REQUEST_ACCEPTED;
}

void handle_new_request(UArg arg0, UArg arg1)
{

	init_allocator();

	while (1)
	{
	    create_task();
	    //create_event((Task_FuncPtr) output);
		run_allocator();
		Task_sleep((UInt) arg0);
	}

}

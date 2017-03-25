#include "allocator.h"

int requests_equal(request_type a, request_type b)
{
	int is_equal = 0;
	if (a.request_id == b.request_id)
	{
		is_equal = 1;
	}

	return is_equal;
}

void telecomms_cancel_callback()
{
	printf("Received a cancellation notice\n");
}

void telecomms_success_callback()
{
	printf("Received a success notice\n");
}

void telecomms_failure_callback()
{
	printf("Received a failure notice\n");
}

request_type telecomms_requests()
{

	request_type request;
	request.system_id = TELECOM;
	request.subsystem_id = 3;
	request.request_id = 22;
	request.duration = rand() % 120000;
	request.start_time = rand() % 599999; // 10 seconds from now. Should we use relative or absolute times?
	request.end_time = request.start_time + request.duration;
	request.priority = rand() % 16; // On a 1-16 scale, we are about an 8
	request.dependencies[0] = 1;
	request.dependencies[1] = 22;
	request.dependencies[2] = 44;
	// = {1, 2, 3}; // These refer to other subsystems
	request.cancel_callback = telecomms_cancel_callback;
	request.success_callback = telecomms_success_callback;
	request.failure_callback = telecomms_failure_callback;
	request.type = 10;
	request.power_peak = 3;
	request.power_average = 1.2;

	return request;
}

void print_schedule()
{
	for (int i = 0; i < 80; i++)
	{
		printf("=");
	}

	printf("\n");

	schedule_type schedule = schedule_get();
	for (int i = 0; i < schedule_size(); i++)
	{
		printf("schedule[%3d] = {%2d, %10d, %10d, %10f}\n", i,
				schedule.requests[i].priority, schedule.requests[i].start_time,
				schedule.requests[i].end_time,
				schedule.requests[i].power_average);
	}

	for (int i = 0; i < 80; i++)
	{
		printf("=");
	}

	printf("\n");
	printf("\n");
}

/*
 int find_lowest_priority_event_index_in_duration(request_type request)
 {
 // TODO We could return the event instead, or a globally unique ID?
 // TODO Do we need to pass these in, instead of global access?
 request_type lowest_priority_request;
 int index = -1;

 for(int j = 0; j < n_allocated_events; j++)
 {
 request_type examining_request = allocated_events[j];
 for (int i = 0; i < ALLOCATOR_SIZE; i++)
 {
 power_allocation_element_type power_allocation_element = allocated_power[i];

 // TODO Move one of these checks into the outer loop
 // TODO These ternary's are hard to read
 // First check whether there is any overlap between the allocation we are examining
 if ((request.start_time > power_allocation_element.start_time ? request.start_time : power_allocation_element.start_time) <=
 (request.end_time < power_allocation_element.end_time ? request.end_time : power_allocation_element.end_time))
 {

 if ((examining_request.start_time > request.start_time ? examining_request.start_time : request.start_time) <=
 (examining_request.end_time < request.end_time ? examining_request.end_time : request.end_time))
 {
 if (lowest_priority_request.priority < request.priority)
 {
 index = j;
 lowest_priority_request = request;
 }
 }
 }
 }
 }

 return index;
 }*/


void print_request(request_type request)
{
	// TODO This should really return a string, that we can log, or print
	printf("Request from system: %d, subsystem: %d, request id: %d\n",
			request.system_id, request.subsystem_id, request.request_id);
	printf("    Start: %d, end: %d, duration: %d\n", request.start_time,
			request.end_time, request.end_time - request.start_time);
	printf("    Priority: %d\n", request.priority);
}

// Main allocator task
allocator_status_type allocator(request_type request)
{

	allocator_status_type request_acceptance_status = REQUEST_DENIED; // Assume the implicit state is deny.

	// To determine if we can allocate an event or not, we need to check for resource availability.
	// We need to iterate through every already scheduled event occurring in this duration, and calculate the average power consumption per time-step.

	print_request(request);

	// For the given event, we need to ensure it can satisfy all system resource requirements

	int can_be_allocated = 1; // We assume the event be allocated, unless we find out otherwise

	// ALEX module will go here, doesn't need to be called function allocator
	//can_be_allocated &= power_allocator(request, schedule_get());
	can_be_allocated &= 1; // TODO This could be where the pointing resource allocator provides its output

	/*
	 if (!can_be_allocated)
	 {
	 printf("Attempting to optimise the event allocation\n\n\n");
	 print_allocated_events();

	 request_type removed_events[10];
	 int n_removed_events = 0;

	 // We bound the number of allocation attempts we can make
	 for (int attempt_counter = 0; attempt_counter < MAX_ALLOCATION_ATTEMPTS && !(can_be_allocated); attempt_counter++)
	 {

	 // Find the index of the event with the lowest priority, that overlaps with the event we need to schedule
	 int index_of_lowest_priority_request = find_lowest_priority_event_index_in_duration(request);

	 if (index_of_lowest_priority_request == -1)
	 {
	 printf("Improving situation is impossible\n");
	 break;
	 }

	 request_type lowest_priority_request = allocated_events[index_of_lowest_priority_request];

	 printf("Found an event to remove with priority %d\n", lowest_priority_request.priority);

	 // TODO We need a proper removal method
	 // Rather than writing a method to handle the subtraction of power, we just add a negative power instead
	 lowest_priority_request.power_average *= -1;

	 // Remove power allocation
	 do_for_event_range(lowest_priority_request, &allocate_power);

	 // Store this event for now, in case we can add it again later.
	 // We will also use this array to notify the subsystems that their event was cancelled.
	 removed_events[n_removed_events] = lowest_priority_request;
	 n_removed_events++;

	 // Remove from the allocation
	 allocated_events[index_of_lowest_priority_request] = allocated_events[n_allocated_events - 1];
	 n_allocated_events--;

	 // Tell the subsystem we cancelled the event (although this should happen later, once we finalize the deletion
	 // TODO
	 lowest_priority_request.cancel_callback();

	 // Attempt to schedule the requested event again, now that we have removed the least important event
	 can_be_allocated = do_for_event_range(request, &within_power_allocation);
	 }
	 }
	 */

	if (can_be_allocated)
	{
		if (schedule_add(request))
		{
			request_acceptance_status = REQUEST_ACCEPTED;
		}
		else
		{
			request_acceptance_status = REQUEST_DENIED;
		}
	}

	return request_acceptance_status;
}

void init_allocator()
{
	printf("Allocator init complete\n");
}

status_type run_allocator()
{

	srand ( time(NULL) );


	request_type request = telecomms_requests();
	allocator(request);

	printf("Printing allocated requests\n");
	print_schedule();

	return SUCCESS;
}

void handle_new_request(UArg arg0, UArg arg1)
{

	init_allocator();

	while (1)
	{
		run_allocator();
		Task_sleep((UInt) arg0);
	}

}

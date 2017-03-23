#include "allocator.h"
#include "src/system.h"

typedef struct request
{
    system_type system_id;
    int subsystem_id; // The subsystem that is requesting this event
    int request_id; // A unique identifier. This is only unique in the context of a subsystem, so subsystem_id and request_id is required to globally identify.
    int start_time; // Some system definition of time. Might be number of microseconds since something?
    int end_time;
    int priority; // The priority of this task, from a range 0 - X.
    int dependencies[MAX_DEPENDENCIES]; // All the sub tasks that need to be created for this task. Need to work out the maximum size.
    void( * cancel_callback)(); // Function to call if this event is cancelled. Should we also call it if the event is not accepted?
    void( * success_callback)(); // Function to call if this event succeeds.
    void( * failure_callback)(); // Function to call if this event fails.
    int duration; // Duration of this event in milliseconds?
    int type; // The type of event. Maybe this shouldn't be type SUBSYSTEM?
    float power_peak; // Peak power draw expected.
    float power_average; // Average power draw expected.
}
request_type;


typedef struct power_allocation_element
{
    int start_time;
    int end_time;
    float power_allocated;
    float power_available;
}
power_allocation_element_type;

request_type allocated_events[MAX_EVENTS_ALLOCATED]; // Maximum queue length of 100
power_allocation_element_type allocated_power[ALLOCATOR_SIZE];

int n_allocated_events = 0;

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

float request_power_available()
{

    float maximum_power_available = 5;
    float power = ((float)rand()/(float)(RAND_MAX)) * maximum_power_available;
    float rounded_down_power = floorf(power * 10) / 10;
    return rounded_down_power;
}


int event_starts_in_block(power_allocation_element_type power_allocation_element,request_type request)
{
    if (request.start_time >= power_allocation_element.start_time &&
            request.start_time <= power_allocation_element.end_time)
    {
        // This event overlaps with the allocation block
        return 1;
    }

    return 0;
}


void print_allocated_events()
{
    for(int i = 0; i < 80; i++)
    {
        printf("=");
    }

    printf("\n");

    for (int i = 0; i < n_allocated_events; i++)
    {
        printf("allocated_event[%3d] = {%2d, %10d, %10d, %10f}\n", i, allocated_events[i].priority, allocated_events[i].start_time, allocated_events[i].end_time, allocated_events[i].power_average);
    }

    for(int i = 0; i < 80; i++)
    {
        printf("=");
    }

    printf("\n");
    printf("\n");
}


int event_ends_in_block(power_allocation_element_type element, request_type request)
{
    if (element.end_time <= element.end_time)
    {
        // This event overlaps with the allocation block
        return 1;
    }

    return 0;
}


status_type allocate_power(power_allocation_element_type *power_allocation_element, request_type request)
{
    printf("Allocated power for timestep %6d  -  %6d\n", power_allocation_element->start_time, power_allocation_element->end_time);
    power_allocation_element->power_allocated += request.power_average;

    return SUCCESS;
}


status_type within_power_allocation(power_allocation_element_type *power_allocation_element, request_type request)
{

    status_type status = FAILURE;
    if (power_allocation_element->power_allocated + request.power_average >
            power_allocation_element->power_available)
    {
        status = SUCCESS;
    }

    return status;
}

// Validates whether the power team can support a request or not
status_type power_request_validator(power_allocation_element_type *power_allocation_element, request_type request)
{

    status_type status = FAILURE;

    if (power_allocation_element->power_allocated + request.power_average >
            power_allocation_element->power_available)
    {
        status = SUCCESS;
    }

    return status;
}


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
}

// TODO This function name sucks
int do_for_event_range(request_type request, status_type (*action)(power_allocation_element_type *, request_type))
{
    int check_allocation_flag = 0; // TODO Rename variable

    int returnVal = 1;

    for (int i = 0; i < ALLOCATOR_SIZE; i++)
    {
        power_allocation_element_type power_allocation_element = allocated_power[i];
        if (event_starts_in_block(power_allocation_element, request))
        {
            // This event overlaps with the allocation block
            check_allocation_flag = 1;
        }

        if (check_allocation_flag)
        {
            returnVal = action(&allocated_power[i], request);

            if(!returnVal)
            {
                return returnVal;
            }
        }

        if (event_ends_in_block(power_allocation_element, request))
        {
            // This event overlaps with the allocation block
            return returnVal;
        }
    }

    return returnVal;
}


void print_request(request_type request)
{
    // TODO This should really return a string, that we can log, or print
    printf("Request from system: %d, subsystem: %d, request id: %d\n", request.system_id, request.subsystem_id, request.request_id);
    printf("    Start: %d, end: %d, duration: %d\n", request.start_time, request.end_time, request.end_time - request.start_time);
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

    can_be_allocated &= do_for_event_range(request, &within_power_allocation); // Do we have the power to allocate?
    can_be_allocated &= 1; // TODO This could be where the pointing resource allocator provides its output

    if (!can_be_allocated)
    {
        printf("Attempting to optimise the event allocation\n\n\n");
        print_allocated_events();

        request_type removed_events[10];
        int n_removed_events = 0;

        // We bound the number of allocation attempts we can make
        for (int attempt_counter = 0; attempt_counter < MAX_ALLOCATION_ATTEMPTS || !(can_be_allocated); attempt_counter++)
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


    if (can_be_allocated)
    {
        // Update the power allocation table to reflect that this event is now scheduled
        do_for_event_range(request, &allocate_power);

        // Store the event in the allocation table
        allocated_events[n_allocated_events] = request;
        n_allocated_events++;

        request_acceptance_status = REQUEST_ACCEPTED;
    }

    return request_acceptance_status;
}


void init_allocator()
{

    for (int i = 0; i < ALLOCATOR_SIZE; i++)
    {
        allocated_power[i].power_available = 0;
        allocated_power[i].power_allocated = 0;
        allocated_power[i].start_time = i * ALLOCATOR_TIMESTEP;
        allocated_power[i].end_time = (i + 1) * ALLOCATOR_TIMESTEP - 1;
    }
    printf("Allocator init complete\n");
}

void update_power_estimate()
{

    for (int i = 0; i < ALLOCATOR_SIZE; i++)
    {
        allocated_power[i].power_available = request_power_available();
    }

    printf("Power update completed\n");
}



void print_power_allocation()
{

    for(int i = 0; i < 80; i++)
    {
        printf("=");
    }

    printf("\n");

    for (int i = 0; i < ALLOCATOR_SIZE; i++)
    {
        printf("allocated_power[%3d] = {%10d, %10d, %10f, %10f}\n", i, allocated_power[i].start_time, allocated_power[i].end_time, allocated_power[i].power_allocated, allocated_power[i].power_available);
    }

    for(int i = 0; i < 80; i++)
    {
        printf("=");
    }

    printf("\n");
    printf("\n");
}


status_type run_allocator()
{

    srand ( time(NULL) );

    update_power_estimate();
    printf("Printing PRE power allocation\n");
    print_power_allocation();

    request_type request = telecomms_requests();
    allocator(request);

    printf("Printing POST power allocation\n");
    print_power_allocation();

    printf("Printing allocated requests\n");
    print_allocated_events();

    return SUCCESS;
}

void handle_new_request(UArg arg0, UArg arg1)
{

    init_allocator();

    while(1){
        run_allocator();
        System_flush();
        Task_sleep((UInt) arg0);
    }

}

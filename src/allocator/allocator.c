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

typedef struct event {
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
event;


typedef struct power_allocation {
    int start_time;
    int end_time;
    float power_allocated;
    float power_available;
}
power_allocation;

struct event allocated_events[MAX_EVENTS_ALLOCATED]; // Maximum queue length of 100
struct power_allocation allocated_power[ALLOCATOR_SIZE];

int n_allocated_events = 0;

void telecomms_cancel_callback() {
    printf("Received a cancellation notice\n");
}

void telecomms_success_callback() {
    printf("Received a success notice\n");
}

void telecomms_failure_callback() {
    printf("Received a failure notice\n");
}

void telecomms_requests(event requests[MAX_EVENTS_REQUESTED], int * n_requested) {
    // Make some events and stick them into the request queue

    int start_time[5] = {10000, 70000, 77000, 430000, 540010};
    int end_time[5] = {15000, 80000, 390000, 490000, 588888};

    for (int i = 0; i < 5; i++) {
        requests[i].start_time = start_time[i]; // 10 seconds from now. Should we use relative or absolute times?
        requests[i].end_time = end_time[i];
        requests[i].priority = 8 + i; // One a 1-16 scale, we are about an 8
        requests[i].dependencies[0] = 1;
        requests[i].dependencies[1] = 22;
        requests[i].dependencies[2] = 44;
        // = {1, 2, 3}; // These refer to other subsystems
        requests[i].cancel_callback = telecomms_cancel_callback;
        requests[i].success_callback = telecomms_success_callback;
        requests[i].failure_callback = telecomms_failure_callback;
        requests[i].duration = 1000;
        requests[i].type = 10;
        requests[i].power_peak = 3;
        requests[i].power_average = 1.2;
    }

    *n_requested = 5;
}

float request_power_available() {

    float maximum_power_available = 5;
    float power = ((float)rand()/(float)(RAND_MAX)) * maximum_power_available;
    float rounded_down_power = floorf(power * 10) / 10;
    return rounded_down_power;
}


int event_starts_in_block(struct power_allocation allocation_block, struct event requested_event) {
    if (requested_event.start_time >= allocation_block.start_time &&
            requested_event.start_time <= allocation_block.end_time) {
        // This event overlaps with the allocation block
        return 1;
    }

    return 0;
}


void print_allocated_events() {
    for(int i = 0; i < 80; i++) {
        printf("=");
    }

    printf("\n");

    for (int i = 0; i < n_allocated_events; i++) {
        printf("allocated_event[%3d] = {%2d, %10d, %10d, %10f}\n", i, allocated_events[i].priority, allocated_events[i].start_time, allocated_events[i].end_time, allocated_events[i].power_average);
    }

    for(int i = 0; i < 80; i++) {
        printf("=");
    }

    printf("\n");
    printf("\n");
}


int event_ends_in_block(struct power_allocation allocation_block, struct event requested_event) {
    if (requested_event.end_time <= allocation_block.end_time) {
        // This event overlaps with the allocation block
        return 1;
    }

    return 0;
}


int allocate_power(struct power_allocation* allocation_block, struct event requested_event) {
    printf("Allocated power for timestep %6d  -  %6d\n", allocation_block->start_time, allocation_block->end_time);
    allocation_block->power_allocated += requested_event.power_average;

    return 1;
}


int within_power_allocation(struct power_allocation* allocation_block, struct event requested_event) {
    if (allocation_block->power_allocated + requested_event.power_average >
            allocation_block->power_available) {
        return 0;
    }

    return 1;
}


int find_lowest_priority_event_index_in_duration(struct event requested_event) {

    struct event lowest_priority_event;
    int index = -1;

    for(int j = 0; j < n_allocated_events; j++) {
        struct event check_event = allocated_events[j];
        for (int i = 0; i < ALLOCATOR_SIZE; i++) {
            struct power_allocation allocation_block = allocated_power[i];

            // First check whether there is any overlap between the allocation we are examining
            if ((requested_event.start_time > allocation_block.start_time ? requested_event.start_time : allocation_block.start_time) <=
                    (requested_event.end_time < allocation_block.end_time ? requested_event.end_time :allocation_block.end_time)) {

                if ((check_event.start_time > requested_event.start_time ? check_event.start_time : requested_event.start_time) <=
                        (check_event.end_time < requested_event.end_time ? check_event.end_time : requested_event.end_time)) {
                    if (lowest_priority_event.priority < requested_event.priority) {
                        index = j;
                        lowest_priority_event = requested_event;
                    }
                }
            }
        }
    }

    return index;
}


int do_for_event_range(struct event requested_event, int (*action)(struct power_allocation *, struct event)) {
    int check_allocation_flag = 0; // TODO Rename variable

    int returnVal = 1;

    for (int i = 0; i < ALLOCATOR_SIZE; i++) {
        struct power_allocation allocation_block = allocated_power[i];
        if (event_starts_in_block(allocation_block, requested_event)) {
            // This event overlaps with the allocation block
            check_allocation_flag = 1;
        }

        if (check_allocation_flag) {
            returnVal = action(&allocated_power[i], requested_event);

            if(!returnVal) {
                return returnVal;
            }
        }

        if (event_ends_in_block(allocation_block, requested_event)) {
            // This event overlaps with the allocation block
            return returnVal;
        }
    }

    return returnVal;
}

// Main allocator task
void allocator() {

    printf("Entering the allocator\n");
    // Request events from other teams
    event requests[MAX_EVENTS_REQUESTED]; // Can request up to 10 events at a time
    int n_requested = 0;

    telecomms_requests(requests, &n_requested);

    // Now, check which tasks we can schedule
    for (int i = 0; i < n_requested; i++) {
        event requested_event = requests[i];

        // To determine if we can allocate an event or not, we need to check for resource availability.
        // We need to iterate through every already scheduled event occurring in this duration, and calculate the average power consumption per time-step.

        printf("Examining request for {%10d, %10d, %10f}\n", requested_event.start_time, requested_event.end_time, requested_event.power_average);

        // Review our power allocation to see how we are going
        int can_be_allocated = do_for_event_range(requested_event, &within_power_allocation);

        int maximum_depth = 10;
        int depth_counter = 0;

        if (!can_be_allocated) {

            printf("Attempting to optimise the event allocation\n\n\n");
            print_allocated_events();

            struct event removed_events[10];
            int n_removed_events = 0;

            for (int attempt_counter = 0; attempt_counter < MAX_ALLOCATION_ATTEMPTS; attempt_counter++) {
                // We bound the number of allocation attempts we can make

                // Remove event
                int index = find_lowest_priority_event_index_in_duration(requested_event);

                if (index == -1) {
                    printf("Improving situation is impossible\n");
                    // We have no hope of improving this situation
                    break;
                }

                struct event lowest_priority_event = allocated_events[index];

                printf("Found an event to remove with priority %d\n", lowest_priority_event.priority);

                lowest_priority_event.power_average *= -1;
                // Remove power allocation
                do_for_event_range(lowest_priority_event, &allocate_power);
                removed_events[n_removed_events] = lowest_priority_event;
                n_removed_events++;

                // Remove from the allocation
                allocated_events[index] = allocated_events[n_allocated_events - 1];
                n_allocated_events--;
                // TODO Do something in here to reshuffle the events, based on priority
                // Maybe we go and cancel another event first, then attempt allocation again.
                can_be_allocated = do_for_event_range(requested_event, &within_power_allocation);

                if (can_be_allocated) {
                    break;
                }
            }
        }


        if (can_be_allocated) {
            do_for_event_range(requested_event, &allocate_power);
            // Store the event in the allocation table
            allocated_events[n_allocated_events] = requested_event;
            n_allocated_events++;
        }
    }

    printf("Exiting allocator function\n");
}





void init_allocator() {

    for (int i = 0; i < ALLOCATOR_SIZE; i++) {
        allocated_power[i].power_available = 0;
        allocated_power[i].power_allocated = 0;
        allocated_power[i].start_time = i * ALLOCATOR_TIMESTEP;
        allocated_power[i].end_time = (i + 1) * ALLOCATOR_TIMESTEP - 1;
    }
    printf("Allocator init complete\n");
}

void update_power_estimate() {

    for (int i = 0; i < ALLOCATOR_SIZE; i++) {
        allocated_power[i].power_available = request_power_available();
    }

    printf("Power update completed\n");
}



void print_power_allocation() {

    for(int i = 0; i < 80; i++) {
        printf("=");
    }

    printf("\n");

    for (int i = 0; i < ALLOCATOR_SIZE; i++) {
        printf("allocated_power[%3d] = {%10d, %10d, %10f, %10f}\n", i, allocated_power[i].start_time, allocated_power[i].end_time, allocated_power[i].power_allocated, allocated_power[i].power_available);
    }

    for(int i = 0; i < 80; i++) {
        printf("=");
    }

    printf("\n");
    printf("\n");
}
int main() {

    srand ( time(NULL) );

    printf("Entering main\n");

    init_allocator();
    update_power_estimate();

    print_power_allocation();

    printf("\n\nCalling allocator the first time\n");
    allocator();
    print_power_allocation();
    print_allocated_events();

    printf("\n\nCalling allocator the second time\n");
    allocator();
    print_power_allocation();
    print_allocated_events();
}
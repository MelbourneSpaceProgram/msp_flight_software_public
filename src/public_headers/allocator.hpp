// Public allocator interface

#ifndef MSP_ALLOCATOR_HPP
#define MSP_ALLOCATOR_HPP

#define MAX_DEPENDENCIES 5
#define MAX_ALLOCATION_ATTEMPTS 10

typedef enum
{
    ALLOCATOR_REQUEST_ACCEPTED, ALLOCATOR_REQUEST_DENIED
} allocator_status_type;


typedef struct request_long
{
    system_type system_id;
    int subsystem_id; // The subsystem that is requesting this event
    int request_id; // A unique identifier. This is only unique in the context of a subsystem, so subsystem_id and request_id is required to globally identify.
    int start_time; // Some system definition of time. Might be number of microseconds since something?
    int end_time;
    int priority; // The priority of this task, from a range 0 - X.
    int dependencies[MAX_DEPENDENCIES]; // All the sub tasks that need to be created for this task. Need to work out the maximum size.
    int duration; // Duration of this event in milliseconds?
    int type; // The type of event. Maybe this shouldn't be type SUBSYSTEM?
    float power_peak; // Peak power draw expected.
    float power_average; // Average power draw expected.

} request_long_type;

typedef Task_FuncPtr;

typedef struct request_short
{
    system_type system_id;
    int subsystem_id; // The subsystem that is requesting this event
    int request_id; // A unique identifier. This is only unique in the context of a subsystem, so subsystem_id and request_id is required to globally identify.
    int priority; // The priority of this task, from a range 0 - X.
    int period; // In ms
    int n_executions; // Number of executions
    char name[30];
    Task_FuncPtr func;
    UArg args;
} request_short_type;

int requests_equal(request_long_type a, request_long_type b);
void handle_new_request(UArg arg0, UArg arg1);

#endif //MSP_ALLOCATOR_HPP

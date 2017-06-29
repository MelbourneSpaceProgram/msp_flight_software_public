#ifndef MSP_ALLOCATOR_HPP
#define MSP_ALLOCATOR_HPP

#include <src/public_headers/arch/System.h>
#include <src/public_headers/arch/Task.h>

#define MAX_ALLOCATION_ATTEMPTS 10

typedef enum
{
    ALLOCATOR_REQUEST_ACCEPTED, ALLOCATOR_REQUEST_DENIED
} allocator_status_type;

void handle_new_request(UArg arg0, UArg arg1);

#endif //MSP_ALLOCATOR_HPP

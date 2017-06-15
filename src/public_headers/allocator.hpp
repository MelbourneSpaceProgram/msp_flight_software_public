// Public allocator interface

//#include <xdc/std.h>
//#include <ti/sysbios/knl/Task.h>
//#include <xdc/runtime/System.h>
#include <src/public_headers/arch/System.h>
#include <src/public_headers/arch/Task.h>
//#include <xdc/runtime/Memory.h>

#ifndef MSP_ALLOCATOR_HPP
#define MSP_ALLOCATOR_HPP


#define MAX_ALLOCATION_ATTEMPTS 10

typedef enum
{
    ALLOCATOR_REQUEST_ACCEPTED, ALLOCATOR_REQUEST_DENIED
} allocator_status_type;



void handle_new_request(UArg arg0, UArg arg1);


#endif //MSP_ALLOCATOR_HPP

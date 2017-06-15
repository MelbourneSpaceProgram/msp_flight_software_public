#include <src/public_headers/arch/Task.h>
#include <src/public_headers/systems.hpp>
#include <src/public_headers/request.hpp>

int requests_equal(request_long_type a, request_long_type b)
{

    // TODO Is this good enough for equality?
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

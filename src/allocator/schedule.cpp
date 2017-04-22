/*
 * new_schedule.cpp
 *
 *  Created on: 2Apr.,2017
 *      Author: Anthony
 */

#include <src/allocator/allocator.h>

Schedule::Schedule(void)
{
    size = 0;
}
int Schedule::get_size()
{
    return size;
}

request_type Schedule::get_request(int i){

    return requests[i];
}

status_type Schedule::clear()
{
    size = 0;
    return SUCCESS;
}

status_type Schedule::remove(request_type request)
{

    status_type status = FAILURE;

    // Iterate through to find the struct with the same id
    for (int i = 0; i < size; i++)
    {
        if (requests_equal(requests[i], request))
        {
            // We have found the request
            requests[i] = requests[size - 1];
            size--;
            status = SUCCESS;
            break;
        }
    }

    return status;
}

status_type Schedule::add(request_type request)
{

    status_type status = FAILURE;

    // Check the size of the schedule
    if (size < MAX_SCHEDULE_SIZE)
    {
        requests[size] = request;
        size++;
        status = SUCCESS;
    }

    return status;
}

void Schedule::print()
{
    for (int i = 0; i < 80; i++)
    {
        System_printf("=");
    }

    System_printf("\n");
    System_flush();

    for (int i = 0; i < get_size(); i++)
    {
        printf("schedule[%3d] = {%2d, %10d, %10d, %10f}\n", i,
               requests[i].priority, requests[i].start_time,
               requests[i].end_time, requests[i].power_average);
        System_flush();
    }

    for (int i = 0; i < 80; i++)
    {
        System_printf("=");
    }

    System_printf("\n");
    System_printf("\n");
    System_flush();

    return;
}

/*
 * schedule.c
 *
 *  Created on: 25Mar.,2017
 *      Author: Anthony
 */


#include "allocator.h"

// This is private to this file
static schedule_type schedule;

status_type schedule_add(request_type request)
{
    status_type status = FAILURE;

    // Check the size of the schedule
    if (schedule.size < MAX_SCHEDULE_SIZE)
    {
        schedule.requests[schedule.size] = request;
        schedule.size++;
        status = SUCCESS;
    }

    return status;
}

status_type schedule_delete(request_type request)
{

    status_type status = FAILURE;

    // Iterate through to find the struct with the same id
    for(int i = 0; i < schedule.size; i++)
    {
        if (requests_equal(schedule.requests[i], request))
        {
            // We have found the request
            schedule.requests[i] = schedule.requests[schedule.size - 1];
            schedule.size--;
            status = SUCCESS;
            break;
        }
    }

    return status;
}


schedule_type schedule_get()
{
    return schedule;
}
status_type schedule_clear()
{
    // Seeing as there are no security concerns, do we need to worry about actively zeroing the structs?

    schedule.size = 0;

    return SUCCESS;
}

int schedule_size()
{
    return schedule.size;
}


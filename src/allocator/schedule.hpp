/*
 * schedule.h
 *
 *  Created on: 2Apr.,2017
 *      Author: Anthony
 */

#ifndef SCHEDULE_HPP_
#define SCHEDULE_HPP_


#include "allocator.hpp"

class Schedule
{
    const int MAX_SCHEDULE_SIZE = 100;
    request_type requests[MAX_SCHEDULE_SIZE];
    int size;
public:
    Schedule(void);
    int get_size(void);
    request_type get_request(int);
    request_type get_requests(void);
    status_type add(request_type);
    status_type remove(request_type);
    status_type clear(void);
    void print(void);
};


#endif /* SCHEDULE_HPP_ */

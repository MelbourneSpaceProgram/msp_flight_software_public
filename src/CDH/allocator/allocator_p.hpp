#ifndef MSP_ALLOCATOR_P_HPP
#define MSP_ALLOCATOR_P_HPP

void handle_new_request(UArg arg0, UArg arg1);

class Schedule;

allocator_status_type allocator(request_long_type request);
request_long_type find_lowest_priority_overlap(request_long_type request, Schedule schedule);

void init_allocator();
void print_request(request_long_type request);
void print_schedule();

int requests_overlap(request_long_type a, request_long_type b);
allocator_status_type run_allocator();
request_long_type telecomms_requests();

#endif //MSP_ALLOCATOR_P_HPP

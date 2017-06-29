#ifndef MSP_SCHEDULE_HPP
#define MSP_SCHEDULE_HPP

typedef enum schedule_status
{
    SCHEDULE_SUCCESS, SCHEDULE_FAILURE, SCHEDULE_DENY
} schedule_status_type;

class Schedule
{
    static const int MAX_SCHEDULE_SIZE = 10;
    request_long_type requests[MAX_SCHEDULE_SIZE];
    int size;
public:
    Schedule(void);
    int get_size(void);
    request_long_type get_request(int);
    request_long_type get_requests(void);
    schedule_status_type add(request_long_type);
    schedule_status_type remove(request_long_type);
    schedule_status_type clear(void);
    void print(void);
};

#endif // MSP_SCHEDULE_HPP

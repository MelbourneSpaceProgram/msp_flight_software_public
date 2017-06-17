// Public time interface

#ifndef MSP_TIME_HPP
#define MSP_TIME_HPP

#include <src/public_headers/arch/Clock.h>
#include <stdint.h>

class Time
{
public:
    static uint64_t get_utc_time();
    static uint64_t get_boot_time();
    static void set_reference_time(uint64_t);
    static uint64_t get_time_since_boot();
private:
    static uint64_t reference_time; // In milliseconds
    static bool reference_time_valid; // True if the ref time has been set, false if not
    static uint64_t reference_tick_count; // Ticks
    static uint32_t ticks_per_millisecond; // TODO Ensure this matches the value in the config
};

#endif //MSP_TIME_HPP

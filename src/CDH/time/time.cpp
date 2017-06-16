#include <src/public_headers/time.hpp>

uint64_t Time::reference_time = 0;
uint64_t Time::reference_tick_count = 0;
uint32_t Time::ticks_per_millisecond = 1;
bool Time::reference_time_valid = false;


uint64_t Time::get_utc_time(){

    // Return 0 if we do not have the reference time
    // TODO Feel pretty bad doing this... could be worth bringing in exceptions

    uint64_t utc_time = 0;

    if (reference_time_valid){
        // Return UTC time
        // TODO Do we need to be careful about tick count overflow?
        uint32_t ticks_since_boot = Clock_getTicks();

        uint32_t time_since_reference = (ticks_since_boot - reference_tick_count) / ticks_per_millisecond;

        // Now add in the stored offset time
        utc_time = time_since_reference + reference_time;
    }

    return utc_time;
}

uint64_t Time::get_boot_time(){
    // Returns the time the system booted, NOT how long it took.
    // TODO Misleading name?

    uint64_t utc_time = get_utc_time();
    // Calculate the time of boot
    uint32_t ticks_since_boot = Clock_getTicks();
    uint64_t boot_time = utc_time - (ticks_since_boot / ticks_per_millisecond);

    return boot_time;
}

uint64_t Time::get_time_since_boot(){

    uint32_t ticks_since_boot = Clock_getTicks();
    uint64_t time_since_boot = ticks_since_boot / ticks_per_millisecond;

    return time_since_boot;
}

void Time::set_reference_time(uint64_t ref_time){
    // The passed in value needs to be correct as it is passed in.
    // We can't hold it for a few minutes and then decide to set the reference time.

    // TODO Do we need to do any time smearing here?
    // What if we have time issues? Does everyone expect monotonically increasing time?

    reference_time = ref_time;
    reference_tick_count = Clock_getTicks();
    reference_time_valid = true;
}

#include <src/public_headers/time.hpp>
#include <time.h>

// TODO This has not been ported to a non TI platform. WILL NOT WORK.

uint64_t SatelliteTime::reference_time = 0;
uint64_t SatelliteTime::reference_tick_count = 0;
uint32_t SatelliteTime::ticks_per_millisecond = 1;
bool SatelliteTime::reference_time_valid = false;

/**
 * Calculates the current UTC time of the satellite based on the reference time initially set.
 *
 * @return milliseconds since epoch, or 0 if the reference time is undefined
 */
uint64_t SatelliteTime::get_utc_time()
{

    // Return 0 if we do not have the reference time
    // TODO Feel pretty bad doing this... could be worth bringing in exceptions

    uint64_t utc_time = 0;

    if (reference_time_valid)
    {
        // Return UTC time
        // TODO Do we need to be careful about tick count overflow?
        uint32_t ticks_since_boot = Clock_getTicks();

        uint32_t time_since_reference =
                (ticks_since_boot - reference_tick_count)
                        / ticks_per_millisecond;

        // Now add in the stored offset time
        utc_time = time_since_reference + reference_time;
    }

    return utc_time;
}

/**
 * Returns the time the satellite was last turned on.
 * Returns 0 if the reference time of the satellite is not set.
 * @return time of system boot in milliseconds since epoch
 */
uint64_t SatelliteTime::get_boot_time()
{

    uint64_t boot_time = 0;
    uint64_t utc_time = get_utc_time();

    if (utc_time == 0)
    {
        // Calculate the time of boot
        uint32_t ticks_since_boot = Clock_getTicks();
        uint64_t boot_time = utc_time
                - (ticks_since_boot / ticks_per_millisecond);
    }

    return boot_time;
}

/**
 * Returns the time elapsed since the satellite turned on.
 * @return milliseconds elapsed since system boot
 */
uint64_t SatelliteTime::get_time_since_boot()
{

    uint32_t ticks_since_boot = clock();
    uint64_t time_since_boot = ticks_since_boot / ticks_per_millisecond;

    return time_since_boot;
}

/**
 * Sets the reference time and tick count of the satellite.
 * Must be called before using any of the other absolute timing functions.
 *
 * @param ref_time to use as the local epoch, in milliseconds.
 */
void SatelliteTime::set_reference_time(uint64_t ref_time)
{
    // The passed in value needs to be correct as it is passed in.
    // We can't hold it for a few minutes and then decide to set the reference time.

    // TODO Do we need to do any time smearing here?
    // What if we have time issues? Does everyone expect monotonically increasing time?

    reference_time = ref_time;
    reference_tick_count = Clock_getTicks();
    reference_time_valid = true;
}

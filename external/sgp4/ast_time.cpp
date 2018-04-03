/*     ----------------------------------------------------------------
*
*                                asttime.cpp
*
*   This file contains fundamental Astrodynamic procedures and functions
*   relating to the time functions. These routines are discussed in Ch 3
*   and Ch 5.
*
*                          Companion code for
*             Fundamentals of Astrodynamics and Applications
*                                  2013
*                            by David Vallado
*
*       (w) 719-573-2600, email dvallado@agi.com, davallado@gmail.com
*

*       ----------------------------------------------------------------      */

#include <external/sgp4/ast_time.h>
#define pi 3.14159265358979323846
namespace astTime {

/* -----------------------------------------------------------------------------
 *
 *                           function gstime
 *
 *  this function finds the greenwich sidereal time (iau-82).
 *
 *  author        : david vallado                  719-573-2600    1 mar 2001
 *
 *  inputs          description                    range / units
 *    jdut1       - julian date in ut1             days from 4713 bc
 *
 *  outputs       :
 *    gstime      - greenwich sidereal time        0 to 2pi rad
 *
 *  locals        :
 *    temp        - temporary variable for doubles   rad
 *    tut1        - julian centuries from the
 *                  jan 1, 2000 12 h epoch (ut1)
 *
 *  coupling      :
 *    none
 *
 *  references    :
 *    vallado       2013, 187, eq 3-45
 * ---------------------------------------------------------------------------
 */

double gstime(double jdut1) {
    const double twopi = 2.0 * pi;
    const double deg2rad = pi / 180.0;
    double temp, tut1;

    // original code
    tut1 = (jdut1 - 2451545.0) / 36525.0;
    // tut1 = (jdut1 - 2451545.0) / 36525.0;
    temp = -6.2e-6 * tut1 * tut1 * tut1 + 0.093104 * tut1 * tut1 +
           (876600.0 * 3600 + 8640184.812866) * tut1 + 67310.54841;  // sec
    temp = fmod(temp * deg2rad / 240.0,
                twopi);  // 360/86400 = 1/240, to deg, to rad

    // ------------------------ check quadrants ---------------------
    if (temp < 0.0) temp += twopi;

    return temp;
}

}  // end of namespace astTime

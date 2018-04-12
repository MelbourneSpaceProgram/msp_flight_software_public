#include <external/sgp4/sgp4.h>
#include <external/sgp4/sgp4_utils.h>
#include <math.h>

/*
teme_to_pef

This function transforms a vector from a true equator mean equinox (TEME)
frame to a psuedo-earth-fixed frame.

Author: David Vallado, 2007
Ported to C++ by Grady Hillhouse with some modifications, July 2015.

INPUTS          DESCRIPTION                     RANGE/UNITS
rteme           Position vector (TEME)          km
vteme           Velocity vector (TEME)          km/s
jdut1           Julian date                     days

OUTPUTS         DESCRIPTION                     RANGE/UNITS
rpef           Position vector (PEF)          km
vpef           Velocity vector (PEF)          km/s

Edited by MSP
*/

void Sgp4Utils::TemeToPef(double rteme[3], double vteme[3], double jdut1,
                          double rpef[3], double vpef[3]) {
    double gmst;
    double st[3][3];
    double omegaearth[3];

    // Get Greenwich mean sidereal time
    gmst = GreenwichMeanSiderealTime(jdut1);

    // st is the pef - tod matrix
    st[0][0] = cos(gmst);
    st[0][1] = -sin(gmst);
    st[0][2] = 0.0;
    st[1][0] = sin(gmst);
    st[1][1] = cos(gmst);
    st[1][2] = 0.0;
    st[2][0] = 0.0;
    st[2][1] = 0.0;
    st[2][2] = 1.0;

    // Get pseudo earth fixed position vector by multiplying the inverse pef-tod
    // matrix by rteme
    rpef[0] = st[0][0] * rteme[0] + st[1][0] * rteme[1] + st[2][0] * rteme[2];
    rpef[1] = st[0][1] * rteme[0] + st[1][1] * rteme[1] + st[2][1] * rteme[2];
    rpef[2] = st[0][2] * rteme[0] + st[1][2] * rteme[1] + st[2][2] * rteme[2];

    // Earth's angular rotation vector (omega)
    // Note: I don't have a good source for LOD. Historically it has been on the
    // order of 2 ms so I'm just using that as a constant. The effect is very
    // small.
    omegaearth[0] = 0.0;
    omegaearth[1] = 0.0;
    omegaearth[2] = 7.29211514670698e-05 * (1.0 - 0.002 / 86400.0);

    // Pseudo Earth Fixed velocity vector is st'*vteme - omegaearth X rpef
    vpef[0] = st[0][0] * vteme[0] + st[1][0] * vteme[1] + st[2][0] * vteme[2] -
              (omegaearth[1] * rpef[2] - omegaearth[2] * rpef[1]);
    vpef[1] = st[0][1] * vteme[0] + st[1][1] * vteme[1] + st[2][1] * vteme[2] -
              (omegaearth[2] * rpef[0] - omegaearth[0] * rpef[2]);
    vpef[2] = st[0][2] * vteme[0] + st[1][2] * vteme[1] + st[2][2] * vteme[2] -
              (omegaearth[0] * rpef[1] - omegaearth[1] * rpef[0]);
}

/* -----------------------------------------------------------------------------
 *
 *                 function GreenwichMeanSiderealTime
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
 *    jcent_from_y2k - julian *centuries* from the
 *                  jan 1, 2000 12 h epoch (ut1)
 *
 *  coupling      :
 *    none
 *
 *  references    :
 *    vallado       2013, 187, eq 3-45
 * ---------------------------------------------------------------------------
 *
 * Updates made by MSP
 */

double Sgp4Utils::GreenwichMeanSiderealTime(double jdut1) {
    double temp, jcent_from_y2k;

    jcent_from_y2k = (jdut1 - 2451545.0) / 36525.0;

    temp = -6.2e-6 * jcent_from_y2k * jcent_from_y2k * jcent_from_y2k +
           0.093104 * jcent_from_y2k * jcent_from_y2k +
           (876600.0 * 3600 + 8640184.812866) * jcent_from_y2k +
           67310.54841;  // sec

    temp = fmod(temp * kDegreesToRadians / 240.0,
                2 * kPi);  // 360/86400 = 1/240, to deg, to rad

    // ------------------------ check quadrants ---------------------
    if (temp < 0.0) temp += 2 * kPi;

    return temp;
}

double Sgp4Utils::Sign(double x) {
    if (x < 0.0) {
        return -1.0;
    } else {
        return 1.0;
    }
}

/* -----------------------------------------------------------------------------
 *
 *                           function EcefXyzToLatLong
 *
 *  these subroutines convert a geocentric equatorial position vector into
 *    latitude and longitude.  geodetic and geocentric latitude are found. the
 *    inputs must be ecef.
 *
 *  author        : david vallado                  719-573-2600    6 dec 2005
 *
 *  revisions
 *
 *  inputs          description                    range / units
 *    recef       - ecef position vector           km
 *
 *  outputs       :
 *    latgc       - geocentric latitude            -pi to pi rad
 *    latgd       - geodetic latitude              -pi to pi rad
 *    lon         - longitude (west -)             -2pi to 2pi rad
 *    hellp       - height above the ellipsoid     km
 *
 *  locals        :
 *    temp        - diff between geocentric/
 *                  geodetic lat                   rad
 *    sintemp     - sine of temp                   rad
 *    olddelta    - previous value of deltalat     rad
 *    rtasc       - right ascension                rad
 *    decl        - declination                    rad
 *    i           - index
 *
 *  coupling      :
 *    mag         - magnitude of a vector
 *    gstime      - greenwich sidereal time
 *    gcgd        - converts between geocentric and geodetic latitude
 *
 *  references    :
 *    vallado       2013, 173, alg 12 and alg 13, ex 3-3
 * ---------------------------------------------------------------------------
 *
 * Edited by MSP
 */

void Sgp4Utils::EcefXyzToLatLong(double recef[3], double& latgc, double& latgd,
                                 double& lon, double& hellp) {
    const double small = 0.00000001;  // small value for tolerances
    const double re = 6378.137;
    const double eesqrd = 0.006694385000;  // eccentricity of earth sqrd
    double magr, decl, rtasc, olddelta, temp, c = 0.0;

    // ---------------------------  implementation   -----------------------
    magr =
        sqrt(recef[0] * recef[0] + recef[1] * recef[1] + recef[2] * recef[2]);

    // ---------------------- find longitude value  ------------------------

    // magnitude of distance on x-y plane
    temp = sqrt(recef[0] * recef[0] + recef[1] * recef[1]);
    if (fabs(temp) < small)
        rtasc = Sign(recef[2]) * kPi * 0.5;
    else
        // arctan of x,y in position in rad => right ascension
        rtasc = atan2(recef[1], recef[0]);

    lon = rtasc;

    // orginal code
    // not sure it's scale down yet, take the absolute value
    if (fabs(lon) >= kPi)  // mod it ?
    {
        // lon < 0.0 means West
        if (lon < 0.0)
            lon = 2 * kPi + lon;
        else
            lon = lon - 2 * kPi;
    }

    decl = asin(recef[2] / magr);
    latgd = decl;

    // ----------------- iterate to find geodetic latitude -----------------
    olddelta = latgd + 10.0;

    for (int i = 1; (fabs(olddelta - latgd) >= small) && (i < 10); i++) {
        olddelta = latgd;
        double sintemp = sin(latgd);
        c = re / (sqrt(1.0 - eesqrd * sintemp * sintemp));
        latgd = atan((recef[2] + c * eesqrd * sintemp) / temp);
    }

    if ((kPi * 0.5 - fabs(latgd)) > kPi / 180.0)  // 1 deg
        hellp = (temp / cos(latgd)) - c;
    else {
        double s = c * (1.0 - eesqrd);
        hellp = recef[2] / sin(latgd) - s;
    }

    latgc = GeodeticToGeocentric(latgd);
}

/* -----------------------------------------------------------------------------
 *
 *                           function GeodeticToGeocentric
 *
 *  this function converts from geodetic to geocentric latitude for positions
 *    on the surface of the earth.  notice that (1-f) squared = 1-esqrd.
 *
 *  author        : david vallado                  719-573-2600    6 dec 2005
 *
 *  revisions
 *
 *  inputs          description                    range / units
 *    latgd       - geodetic latitude              -pi to pi rad
 *
 *  outputs       :
 *    latgc       - geocentric latitude            -pi to pi rad
 *
 *  locals        :
 *    none.
 *
 *  coupling      :
 *    none.
 *
 *  references    :
 *    vallado       2013, 140, eq 3-11
 * ---------------------------------------------------------------------------
 */

double Sgp4Utils::GeodeticToGeocentric(double latgd) {
    return atan((1.0 - kEccEarthSquared) * tan(latgd));
}

/* -----------------------------------------------------------------------------
 *
 *                   procedure DaysToMonthDayHourMinuteSecond
 *
 *  this procedure converts the day of the year, days, to the equivalent month
 *    day, hour, minute and second.
 *
 *  algorithm     : set up array for the number of days per month
 *                  find leap year - use 1900 because 2000 is a leap year
 *                  loop through a temp value while the value is < the days
 *                  perform int conversions to the correct day and month
 *                  convert remainder into h m s using type conversions
 *
 *  author        : david vallado                  719-573-2600    1 mar 2001
 *
 *  inputs          description                    range / units
 *    year        - year                           1900 .. 2100
 *    days        - julian day of the year         1.0  .. 366.0
 *
 *  outputs       :
 *    mon         - month                          1 .. 12
 *    day         - day                            1 .. 28,29,30,31
 *    hour        - hour                           0 .. 23
 *    min         - minute                         0 .. 59
 *    sec         - second                         0.0 .. 59.999
 *
 *  locals        :
 *    day_of_year - day of year
 *    months[13]  - int array containing the number of days per month
 *
 *  coupling      :
 *    none.
 * ---------------------------------------------------------------------------
 */

void Sgp4Utils::DaysToMonthDayHourMinuteSecond(int year, double days,
                                               int& month, int& day, int& hour,
                                               int& minute, double& sec) {
    int day_of_year, months_days;
    double hour_double, minutes_double;
    int months[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    day_of_year = (int)floor(days);
    /* ----------------- find month and day of month ---------------- */
    if ((year % 4) == 0) months[2] = 29;

    months_days = 0;
    for (int i = 1; i <= 12; i++) {
        if (day_of_year > months_days + months[i] || i == 12) {
            month = i;
            break;
        }
        months_days += months[i];
    }
    day = day_of_year - months_days;

    /* ----------------- find hours minutes and seconds ------------- */
    hour_double = (days - day_of_year) * 24.0;
    hour = (int)floor(hour_double);
    minutes_double = (hour_double - hour) * 60.0;
    minute = (int)floor(minutes_double);
    sec = (minutes_double - minute) * 60.0;
}  // days2mdhms

/* -----------------------------------------------------------------------------
 *
 *                           procedure JulianDay
 *
 *  this procedure finds the julian date given the year, month, day, and time.
 *    the julian date is defined by each elapsed day since noon, jan 1, 4713 bc.
 *
 *  algorithm     : calculate the answer in one step for efficiency
 *
 *  author        : david vallado                  719-573-2600    1 mar 2001
 *
 *  inputs          description                    range / units
 *    year        - year                           1900 .. 2100
 *    mon         - month                          1 .. 12
 *    day         - day                            1 .. 28,29,30,31
 *    hr          - universal time hour            0 .. 23
 *    min         - universal time min             0 .. 59
 *    sec         - universal time sec             0.0 .. 59.999
 *
 *  outputs       :
 *    jd          - julian date                    days from 4713 bc
 *
 *  locals        :
 *    none.
 *
 *  coupling      :
 *    none.
 *
 *  references    :
 *    vallado       2013, 183, alg 14, ex 3-4
 * ---------------------------------------------------------------------------
 */

void Sgp4Utils::JulianDay(int year, int mon, int day, int hr, int minute,
                          double sec, double& jd, double& jdFrac) {
    jd = 367.0 * year - floor((7 * (year + floor((mon + 9) / 12.0))) * 0.25) +
         floor(275 * mon / 9.0) + day +
         1721013.5;  // use - 678987.0 to go to mjd directly
    jdFrac = (sec + minute * 60.0 + hr * 3600.0) / 86400.0;

    // check that the day and fractional day are correct
    if (fabs(jdFrac) > 1.0) {
        double dtt = floor(jdFrac);
        jd = jd + dtt;
        jdFrac = jdFrac - dtt;
    }
}

/* -----------------------------------------------------------------------------
    *
    *                           function GetGravConstants
    *
    *  this function gets constants for the propagator. note that mu is
   identified to
    *    facilitiate comparisons with newer models. the common useage is wgs72.
    *
    *  author        : david vallado                  719-573-2600   21 jul 2006
    *
    *  inputs        :
    *    whichconst  - which set of constants to use  wgs72old, wgs72, wgs84
    *
    *  outputs       :
    *    tumin       - minutes in one time unit
    *    mu          - earth gravitational parameter
    *    radiusearthkm - radius of the earth in km
    *    xke         - reciprocal of tumin
    *    j2, j3, j4  - un-normalized zonal harmonic values
    *    j3oj2       - j3 divided by j2
    *
    *  locals        :
    *
    *  coupling      :
    *    none
    *
    *  references    :
    *    norad spacetrack report #3
    *    vallado, crawford, hujsak, kelso  2006
    ---------------------------------------------------------------------------
 */

void Sgp4Utils::GetGravConstants(gravconsttype whichconst, double& tumin,
                                 double& mu, double& radiusearthkm, double& xke,
                                 double& j2, double& j3, double& j4,
                                 double& j3oj2) {
    switch (whichconst) {
            // -- wgs-72 low precision str#3 constants --
        case wgs72old:
            mu = 398600.79964;         // in km3 / s2
            radiusearthkm = 6378.135;  // km
            xke = 0.0743669161;        // reciprocal of tumin
            tumin = 1.0 / xke;
            j2 = 0.001082616;
            j3 = -0.00000253881;
            j4 = -0.00000165597;
            j3oj2 = j3 / j2;
            break;
            // ------------ wgs-72 constants ------------
        case wgs72:
            mu = 398600.8;             // in km3 / s2
            radiusearthkm = 6378.135;  // km
            xke =
                60.0 / sqrt(radiusearthkm * radiusearthkm * radiusearthkm / mu);
            tumin = 1.0 / xke;
            j2 = 0.001082616;
            j3 = -0.00000253881;
            j4 = -0.00000165597;
            j3oj2 = j3 / j2;
            break;
        case wgs84:
            // ------------ wgs-84 constants ------------
            mu = 398600.5;             // in km3 / s2
            radiusearthkm = 6378.137;  // km
            xke =
                60.0 / sqrt(radiusearthkm * radiusearthkm * radiusearthkm / mu);
            tumin = 1.0 / xke;
            j2 = 0.00108262998905;
            j3 = -0.00000253215306;
            j4 = -0.00000161098761;
            j3oj2 = j3 / j2;
            break;
        default:
            // unknown gravity option
            break;
    }

}  // getgravconst

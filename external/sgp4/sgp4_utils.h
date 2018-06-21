#ifndef EXTERNAL_SGP4_SGP4_UTILS_H_
#define EXTERNAL_SGP4_SGP4_UTILS_H_

#include <src/util/data_types.h>
#include <src/util/physical_constants.h>

typedef enum { wgs72old, wgs72, wgs84 } gravconsttype;

class Sgp4Utils {
   public:
    static void TrueEquatorMeanEquinoxToPsuedoEarthFixed(double rteme[3],
                                                         double vteme[3],
                                                         double jdut1,
                                                         double rpef[3],
                                                         double vpef[3]);

    static double GreenwichMeanSiderealTime(double jdut1);

    static double Sign(double x);

    static double GeodeticToGeocentric(double latgd);

    static void EcefXyzToLatLong(double recef[3], double& latgc, double& latgd,
                                 double& lon, double& hellp);

    static void DaysToMonthDayHourMinuteSecond(uint16_t year, double days,
                                               uint8_t& month, uint8_t& day,
                                               uint8_t& hour, uint8_t& minute,
                                               double& second);

    static void JulianDay(uint16_t year, uint8_t month, uint8_t day,
                          uint8_t hour, uint8_t minute, double second,
                          double& jd, double& jdFrac);

    static void GetGravConstants(gravconsttype whichconst, double& tumin,
                                 double& mu, double& radiusearthkm, double& xke,
                                 double& j2, double& j3, double& j4,
                                 double& j3oj2);

    static const double kJulianDayAtY2000H12 = 2451545.0;
    static const double kDegreesToRadians =
        3.14159265358979323846264338327950288419716939937 / 180.0;
    static const double xpdotp = 229.1831180523293;
    static const double kCnesJdOffset = -2433281.5;
    static const double kEccEarthSquared = 0.006694385000;
};

#endif  // EXTERNAL_SGP4_SGP4_UTILS_H_

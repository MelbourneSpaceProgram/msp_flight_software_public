#include <CppUTest/TestHarness.h>
#include <src/adcs/state_estimators/location_estimator.h>

TEST_GROUP(LocationEstimator){};

TEST(LocationEstimator, UpdateLocation) {
    /* TLE IN USE:
       1 00005U 58002B   00179.78495062  .00000023  00000-0  28098-4 0  4753
       2 00005  34.2682 348.7242 1859667 331.7664  19.3264 10.824191574 13667
    */

    LocationEstimator location_estimator;

    Tle tle = Tle_init_zero;
    tle.epoch = 00179.78495062;
    tle.mean_motion = 10.824191574;
    tle.eccentricity_1e7 = 1859667;
    tle.inclination = 34.2682;
    tle.raan = 348.7242;
    tle.argument_of_perigee = 331.7664;
    tle.mean_anomaly = 19.3264;
    tle.bstar_drag = 0.000028098;

    location_estimator.StoreTle(tle);

    double tsince_millis = 360.0 * 60 * 1000;
    location_estimator.UpdateLocation(tsince_millis);

    double calculated_lattitude_geodetic_degrees =
        location_estimator.GetLattitudeGeodeticDegrees();
    double calculated_longitude_degrees =
        location_estimator.GetLongitudeDegrees();
    double calculated_altitude_above_ellipsoid_km =
        location_estimator.GetAltitudeAboveEllipsoidKm();

    double expected_altitude_above_ellipsoid_km = 2456.906192274563;  // km
    double expected_lattitude_geodetic_degrees = -23.705347061214;    // degrees
    double expected_longitude_degrees = -81.146765103690;             // degrees

    DOUBLES_EQUAL(expected_longitude_degrees, calculated_longitude_degrees,
                  0.001);
    DOUBLES_EQUAL(expected_lattitude_geodetic_degrees,
                  calculated_lattitude_geodetic_degrees, 0.001);
    DOUBLES_EQUAL(expected_altitude_above_ellipsoid_km,
                  calculated_altitude_above_ellipsoid_km, 0.001);
}

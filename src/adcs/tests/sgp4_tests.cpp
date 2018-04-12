#include <external/sgp4/sgp4.h>
#include <external/sgp4/sgp4_utils.h>
#include <math.h>
#include <test_runners/sgp4_tests.h>
#include <test_runners/unity.h>

bool approx_equal(double var1, double var2) {
    double err = fabs((var1 - var2) / var2 * 100);

    if (err < 0.1) {
        return true;
    } else {
        return false;
    }
}

void TestSimplifiedGeneralPerturbationModel() {
    /* TLE IN USE:
   1 00005U 58002B   00179.78495062  .00000023  00000-0  28098-4 0  4753
   2 00005  34.2682 348.7242 1859667 331.7664  19.3264 10.824191574 13667
   */
    double sec, jd, jdFrac;
    int mon;
    int day;
    int hr;
    int minute;
    typedef char str3[4];
    double position[3];
    double velocity[3];
    elsetrec satrec;

    Sgp4Utils::DaysToMonthDayHourMinuteSecond(2000, 179.78495062, mon, day, hr,
                                              minute, sec);
    Sgp4Utils::JulianDay(2000, mon, day, hr, minute, sec, jd, jdFrac);
    gravconsttype whichconst = wgs72;
    char opsmode = 'a';
    satrec.satnum = 00005;
    satrec.jdsatepoch = jd;
    satrec.jdsatepochF = jdFrac;
    satrec.no_kozai = 10.824191574;
    satrec.ecco = 0.1859667;
    satrec.inclo = 34.2682;
    satrec.nodeo = 348.7242;
    satrec.argpo = 331.7664;
    satrec.mo = 19.3264;
    satrec.nddot = 0.00000e0;
    satrec.bstar = 0.000028098;
    satrec.ndot = 0.00000023;
    satrec.elnum = 475;
    satrec.revnum = 1366;
    satrec.classification = 'U';
    satrec.ephtype = 0;
    satrec.no_kozai = satrec.no_kozai / Sgp4Utils::xpdotp;
    satrec.ndot = satrec.ndot / (Sgp4Utils::xpdotp * Sgp4Utils::kMinutesPerDay);
    satrec.nddot = satrec.nddot / (Sgp4Utils::xpdotp * Sgp4Utils::kMinutesPerDay * Sgp4Utils::kMinutesPerDay);
    satrec.inclo = satrec.inclo * Sgp4Utils::kDegreesToRadians;
    satrec.nodeo = satrec.nodeo * Sgp4Utils::kDegreesToRadians;
    satrec.argpo = satrec.argpo * Sgp4Utils::kDegreesToRadians;
    satrec.mo = satrec.mo * Sgp4Utils::kDegreesToRadians;


    Sgp4::InitialisePropagator(
        whichconst, opsmode, satrec.satnum,
        satrec.jdsatepoch + satrec.jdsatepochF - Sgp4Utils::kCnesJdOffset,
        satrec.bstar, satrec.ndot, satrec.nddot, satrec.ecco, satrec.argpo,
        satrec.inclo, satrec.mo, satrec.no_kozai, satrec.nodeo, satrec);
    Sgp4::Propagate(satrec, 360, position, velocity);

    // TEST DATA:
    // TIME: 360.00000000  WE EXPECT: -7154.03120202   -3783.17682504
    // -3536.19412294  4.741887409 -4.151817765 -2.093935425
    // TIME: 0.00000000   WE EXPECT:   7022.46529266   -1400.08296755 0.03995155
    // 1.893841015  6.405893759  4.534807250
    TEST_ASSERT(approx_equal(-7154.03120202, position[0]));
    TEST_ASSERT(approx_equal(-3783.17682504, position[1]));
    TEST_ASSERT(approx_equal(-3536.19412294, position[2]));
    TEST_ASSERT(approx_equal(4.741887409, velocity[0]));
    TEST_ASSERT(approx_equal(-4.151817765, velocity[1]));
    TEST_ASSERT(approx_equal(-2.093935425, velocity[2]));
}

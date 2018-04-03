#include <external/sgp4/ast_math.h>
#include <external/sgp4/ast_time.h>
#include <external/sgp4/coord_fk5.h>
#include <external/sgp4/sgp4.h>
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
    double ro[3];
    double vo[3];
    const double deg2rad = 0.0174532925199433;
    const double xpdotp = 229.1831180523293;
    elsetrec satrec;

    SGP4Funcs::days2mdhms(2000, 179.78495062, mon, day, hr, minute, sec);
    SGP4Funcs::jday(2000, mon, day, hr, minute, sec, jd, jdFrac);
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
    satrec.no_kozai = satrec.no_kozai / xpdotp;
    satrec.ndot = satrec.ndot / (xpdotp * 1440.0);
    satrec.nddot = satrec.nddot / (xpdotp * 1440.0 * 1440);
    satrec.inclo = satrec.inclo * deg2rad;
    satrec.nodeo = satrec.nodeo * deg2rad;
    satrec.argpo = satrec.argpo * deg2rad;
    satrec.mo = satrec.mo * deg2rad;

    SGP4Funcs::sgp4init(whichconst, opsmode, satrec.satnum,
                        satrec.jdsatepoch + satrec.jdsatepochF - 2433281.5,
                        satrec.bstar, satrec.ndot, satrec.nddot, satrec.ecco,
                        satrec.argpo, satrec.inclo, satrec.mo, satrec.no_kozai,
                        satrec.nodeo, satrec);
    SGP4Funcs::sgp4(satrec, 360, ro, vo);

    // TEST DATA:
    //TIME: 360.00000000  WE EXPECT: -7154.03120202   -3783.17682504   -3536.19412294  4.741887409 -4.151817765 -2.093935425
    // TIME: 0.00000000   WE EXPECT:   7022.46529266   -1400.08296755       0.03995155  1.893841015  6.405893759  4.534807250
    TEST_ASSERT(approx_equal( -7154.03120202, ro[0]));
    TEST_ASSERT(approx_equal( -3783.17682504, ro[1]));
    TEST_ASSERT(approx_equal( -3536.19412294, ro[2]));
    TEST_ASSERT(approx_equal( 4.741887409, vo[0]));
    TEST_ASSERT(approx_equal( -4.151817765, vo[1]));
    TEST_ASSERT(approx_equal( -2.093935425, vo[2]));


}

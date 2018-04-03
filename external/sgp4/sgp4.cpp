/*     ----------------------------------------------------------------
 *
 *                               sgp4unit.cpp
 *
 *    this file contains the sgp4 procedures for analytical propagation
 *    of a satellite. the code was originally released in the 1980 and 1986
 *    spacetrack papers. a detailed discussion of the theory and history
 *    may be found in the 2006 aiaa paper by vallado, crawford, hujsak,
 *    and kelso.
 *
 *                            companion code for
 *               fundamentals of astrodynamics and applications
 *                                    2013
 *                              by david vallado
 *
 *     (w) 719-573-2600, email dvallado@agi.com, davallado@gmail.com
 *
 *       ----------------------------------------------------------------
 *    These procedures have been edited by the Melbourne Space Program
 *    for use on orbit onboard the ACRUX-1 satellite.
 *       ---------------------------------------------------------------- */
#include <external/sgp4/ast_time.h>
#include <external/sgp4/sgp4.h>
#include <external/etl/exception.h>

#define pi 3.14159265358979323846

namespace SGP4Funcs {
/* -----------------------------------------------------------------------------
*
*                           procedure days2mdhms
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
*    hr          - hour                           0 .. 23
*    min         - minute                         0 .. 59
*    sec         - second                         0.0 .. 59.999
*
*  locals        :
*    dayofyr     - day of year
*    temp        - temporary extended values
*    inttemp     - temporary int value
*    i           - index
*    lmonth[13]  - int array containing the number of days per month
*
*  coupling      :
*    none.
* --------------------------------------------------------------------------- */

void    days2mdhms
    (
    int year, double days,
    int& mon, int& day, int& hr, int& minute, double& sec
    )
{
    int i, inttemp, dayofyr;
    double    temp;
    int lmonth[] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

    dayofyr = (int)floor(days);
    /* ----------------- find month and day of month ---------------- */
    if ((year % 4) == 0)
        lmonth[2] = 29;

    i = 1;
    inttemp = 0;
    for(int i = 1; i < 12 && (dayofyr > inttemp + lmonth[i]); i++)
    {
        inttemp = inttemp + lmonth[i];
    }
    mon = i;
    day = dayofyr - inttemp;

    /* ----------------- find hours minutes and seconds ------------- */
    temp = (days - dayofyr) * 24.0;
    hr = (int)floor(temp);
    temp = (temp - hr) * 60.0;
    minute = (int)floor(temp);
    sec = (temp - minute) * 60.0;
}  // days2mdhms
/* -----------------------------------------------------------------------------
*
*                           procedure jday
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
* --------------------------------------------------------------------------- */

void    jday
    (
    int year, int mon, int day, int hr, int minute, double sec,
    double& jd, double& jdFrac
    )
{
    jd = 367.0 * year -
        floor((7 * (year + floor((mon + 9) / 12.0))) * 0.25) +
        floor(275 * mon / 9.0) +
        day + 1721013.5;  // use - 678987.0 to go to mjd directly
    jdFrac = (sec + minute * 60.0 + hr * 3600.0) / 86400.0;

    // check that the day and fractional day are correct
    if (fabs(jdFrac) > 1.0)
    {
        double dtt = floor(jdFrac);
        jd = jd + dtt;
        jdFrac = jdFrac - dtt;
    }

}

/* -----------------------------------------------------------------------------
    *
    *                           function getgravconst
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

void getgravconst(gravconsttype whichconst, double& tumin, double& mu,
                  double& radiusearthkm, double& xke, double& j2, double& j3,
                  double& j4, double& j3oj2) {
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

/*-----------------------------------------------------------------------------
*
*                           procedure initl
*
*  this procedure initializes the spg4 propagator. all the initialization is
*    consolidated here instead of having multiple loops inside other routines.
*
*  author        : david vallado                  719-573-2600   28 jun 2005
*
*  inputs        :
*    satn        - satellite number - not needed, placed in satrec
*    xke         - reciprocal of tumin
*    j2          - j2 zonal harmonic
*    ecco        - eccentricity                           0.0 - 1.0
*    epoch       - epoch time in days from jan 0, 1950. 0 hr
*    inclo       - inclination of satellite
*    no          - mean motion of satellite
*
*  outputs       :
*    ainv        - 1.0 / a
*    ao          - semi major axis
*    con41       -
*    con42       - 1.0 - 5.0 cos(i)
*    cosio       - cosine of inclination
*    cosio2      - cosio squared
*    eccsq       - eccentricity squared
*    method      - flag for deep space                    'd', 'n'
*    omeosq      - 1.0 - ecco * ecco
*    posq        - semi-parameter squared
*    rp          - radius of perigee
*    rteosq      - square root of (1.0 - ecco*ecco)
*    sinio       - sine of inclination
*    gsto        - gst at time of observation               rad
*    no          - mean motion of satellite
*
*  locals        :
*    ak          -
*    d1          -
*    del         -
*    adel        -
*    po          -
*
*  coupling      :
*    getgravconst- no longer used
*    gstime      - find greenwich sidereal time from the julian date
*
*  references    :
*    hoots, roehrich, norad spacetrack report #3 1980
*    hoots, norad spacetrack report #6 1986
*    hoots, schumacher and glover 2004
*    vallado, crawford, hujsak, kelso  2006
----------------------------------------------------------------------------*/

static void initl(
    // sgp4fix satn not needed. include in satrec in case needed later
    // int satn,
    // sgp4fix just pass in xke and j2
    // gravconsttype whichconst,
    double xke, double j2, double ecco, double epoch, double inclo,
    double no_kozai, char opsmode, char& method, double& ainv, double& ao,
    double& con41, double& con42, double& cosio, double& cosio2, double& eccsq,
    double& omeosq, double& posq, double& rp, double& rteosq, double& sinio,
    double& no_unkozai) {
    /* --------------------- local variables ------------------------ */
    double ak, d1, del, adel, po, x2o3;

    // sgp4fix use old way of finding gst

    /* ----------------------- earth constants ---------------------- */
    // sgp4fix identify constants and allow alternate values
    // only xke and j2 are used here so pass them in directly
    // getgravconst( whichconst, tumin, mu, radiusearthkm, xke, j2, j3, j4,
    // j3oj2 );
    x2o3 = 2.0 / 3.0;

    /* ------------- calculate auxillary epoch quantities ---------- */
    eccsq = ecco * ecco;
    omeosq = 1.0 - eccsq;
    rteosq = sqrt(omeosq);
    cosio = cos(inclo);
    cosio2 = cosio * cosio;

    /* ------------------ un-kozai the mean motion ----------------- */
    ak = pow(xke / no_kozai, x2o3);
    d1 = 0.75 * j2 * (3.0 * cosio2 - 1.0) / (rteosq * omeosq);
    del = d1 / (ak * ak);
    adel =
        ak * (1.0 - del * del - del * (1.0 / 3.0 + 134.0 * del * del / 81.0));
    del = d1 / (adel * adel);
    no_unkozai = no_kozai / (1.0 + del);

    ao = pow(xke / (no_unkozai), x2o3);
    sinio = sin(inclo);
    po = ao * omeosq;
    con42 = 1.0 - 5.0 * cosio2;
    con41 = -con42 - cosio2 - cosio2;
    ainv = 1.0 / ao;
    posq = po * po;
    rp = ao * (1.0 - ecco);
    method = 'n';


    // find greenwich location at epoch
}  // initl

/*-----------------------------------------------------------------------------
*
*                             procedure sgp4init
*
*  this procedure initializes variables for sgp4.
*
*  author        : david vallado                  719-573-2600   28 jun 2005
*
*  inputs        :
*    opsmode     - mode of operation afspc or improved 'a', 'i'
*    whichconst  - which set of constants to use  72, 84
*    satn        - satellite number
*    bstar       - sgp4 type drag coefficient              kg/m2er
*    ecco        - eccentricity
*    epoch       - epoch time in days from jan 0, 1950. 0 hr
*    argpo       - argument of perigee (output if ds)
*    inclo       - inclination
*    mo          - mean anomaly (output if ds)
*    no          - mean motion
*    nodeo       - right ascension of ascending node
*
*  outputs       :
*    satrec      - common values for subsequent calls
*    return code - non-zero on error.
*                   1 - mean elements, ecc >= 1.0 or ecc < -0.001 or a < 0.95 er
*                   2 - mean motion less than 0.0
*                   3 - pert elements, ecc < 0.0  or  ecc > 1.0
*                   4 - semi-latus rectum < 0.0
*                   5 - epoch elements are sub-orbital
*                   6 - satellite has decayed
*
*  locals        :
*    cnodm  , snodm  , cosim  , sinim  , cosomm , sinomm
*    cc1sq  , cc2    , cc3
*    coef   , coef1
*    cosio4      -
*    day         -
*    dndt        -
*    em          - eccentricity
*    eeta        -
*    etasq       -
*    gam         -
*    argpm       - argument of perigee
*    nodem       -
*    inclm       - inclination
*    mm          - mean anomaly
*    nm          - mean motion
*    perige      - perigee
*    pinvsq      -
*    psisq       -
*    qzms24      -
*    rtemsq      -
*    s1, s2, s3, s4, s5, s6, s7          -
*    sfour       -
*    ss1, ss2, ss3, ss4, ss5, ss6, ss7         -
*    sz1, sz2, sz3
*    sz11, sz12, sz13, sz21, sz22, sz23, sz31, sz32, sz33        -
*    tc          -
*    temp        -
*    temp1, temp2, temp3       -
*    tsi         -
*    xpidot      -
*    xhdot1      -
*    z1, z2, z3          -
*    z11, z12, z13, z21, z22, z23, z31, z32, z33         -
*
*  coupling      :
*    getgravconst-
*    initl       -
*    dscom       -
*    dpper       -
*    dsinit      -
*    sgp4        -
*
*  references    :
*    hoots, roehrich, norad spacetrack report #3 1980
*    hoots, norad spacetrack report #6 1986
*    hoots, schumacher and glover 2004
*    vallado, crawford, hujsak, kelso  2006
----------------------------------------------------------------------------*/

bool sgp4init(gravconsttype whichconst, char opsmode, const int satn,
              const double epoch, const double xbstar, const double xndot,
              const double xnddot, const double xecco, const double xargpo,
              const double xinclo, const double xmo, const double xno_kozai,
              const double xnodeo, elsetrec& satrec) {
    /* --------------------- local variables ------------------------ */
    double ao, ainv, con42, cosio, sinio, cosio2, eccsq, omeosq, posq, rp,
        rteosq, qzms2t, ss, x2o3, r[3], v[3], qzms2ttemp;

    /* ------------------------ initialization --------------------- */
    // sgp4fix divisor for divide by zero check on inclination
    // the old check used 1.0 + cos(pi-1.0e-9), but then compared it to
    // 1.5 e-12, so the threshold was changed to 1.5e-12 for consistency
    const double temp4 = 1.5e-12;

    /* ----------- set all near earth variables to zero ------------ */
    satrec.isimp = 0;
    satrec.method = 'n';
    satrec.aycof = 0.0;
    satrec.con41 = 0.0;
    satrec.cc1 = 0.0;
    satrec.cc4 = 0.0;
    satrec.cc5 = 0.0;
    satrec.d2 = 0.0;
    satrec.d3 = 0.0;
    satrec.d4 = 0.0;
    satrec.delmo = 0.0;
    satrec.eta = 0.0;
    satrec.argpdot = 0.0;
    satrec.omgcof = 0.0;
    satrec.sinmao = 0.0;
    satrec.t = 0.0;
    satrec.t2cof = 0.0;
    satrec.t3cof = 0.0;
    satrec.t4cof = 0.0;
    satrec.t5cof = 0.0;
    satrec.x1mth2 = 0.0;
    satrec.x7thm1 = 0.0;
    satrec.mdot = 0.0;
    satrec.nodedot = 0.0;
    satrec.xlcof = 0.0;
    satrec.xmcof = 0.0;
    satrec.nodecf = 0.0;

    /* ------------------------ earth constants ----------------------- */
    // sgp4fix identify constants and allow alternate values
    // this is now the only call for the constants
    getgravconst(whichconst, satrec.tumin, satrec.mu, satrec.radiusearthkm,
                 satrec.xke, satrec.j2, satrec.j3, satrec.j4, satrec.j3oj2);

    //-------------------------------------------------------------------------

    satrec.error = 0;
    satrec.operationmode = opsmode;
    satrec.satnum = satn;

    // sgp4fix - note the following variables are also passed directly via
    // satrec. it is possible to streamline the sgp4init call by deleting the
    // "x" variables, but the user would need to set the satrec.* values first.
    // we include the additional assignments in case twoline2rv is not used.
    satrec.bstar = xbstar;
    // sgp4fix allow additional parameters in the struct
    satrec.ndot = xndot;
    satrec.nddot = xnddot;
    satrec.ecco = xecco;
    satrec.argpo = xargpo;
    satrec.inclo = xinclo;
    satrec.mo = xmo;
    // sgp4fix rename variables to clarify which mean motion is intended
    satrec.no_kozai = xno_kozai;
    satrec.nodeo = xnodeo;

    // single averaged mean elements
    satrec.am = satrec.em = satrec.im = satrec.Om = satrec.mm = satrec.nm = 0.0;

    /* ------------------------ earth constants ----------------------- */
    // sgp4fix identify constants and allow alternate values no longer needed
    // getgravconst( whichconst, tumin, mu, radiusearthkm, xke, j2, j3, j4,
    // j3oj2 );
    ss = 78.0 / satrec.radiusearthkm + 1.0;
    // sgp4fix use multiply for speed instead of pow
    qzms2ttemp = (120.0 - 78.0) / satrec.radiusearthkm;
    qzms2t = qzms2ttemp * qzms2ttemp * qzms2ttemp * qzms2ttemp;
    x2o3 = 2.0 / 3.0;

    satrec.init = 'y';
    satrec.t = 0.0;

    // sgp4fix remove satn as it is not needed in initl
    initl(satrec.xke, satrec.j2, satrec.ecco, epoch, satrec.inclo,
          satrec.no_kozai, satrec.operationmode, satrec.method, ainv, ao,
          satrec.con41, con42, cosio, cosio2, eccsq, omeosq, posq, rp, rteosq,
          sinio, satrec.no_unkozai);
    satrec.a = pow(satrec.no_unkozai * satrec.tumin, (-2.0 / 3.0));
    satrec.alta = satrec.a * (1.0 + satrec.ecco) - 1.0;
    satrec.altp = satrec.a * (1.0 - satrec.ecco) - 1.0;
    satrec.error = 0;

    if ((omeosq >= 0.0) || (satrec.no_unkozai >= 0.0)) {
        satrec.isimp = 0;
        if (rp < (220.0 / satrec.radiusearthkm + 1.0)) satrec.isimp = 1;
        double sfour = ss;
        double qzms24 = qzms2t;
        double perige = (rp - 1.0) * satrec.radiusearthkm;

        /* - for perigees below 156 km, s and qoms2t are altered - */
        if (perige < 156.0) {
            sfour = perige - 78.0;
            if (perige < 98.0) sfour = 20.0;
            // sgp4fix use multiply for speed instead of pow
            double qzms24temp = (120.0 - sfour) / satrec.radiusearthkm;
            qzms24 = qzms24temp * qzms24temp * qzms24temp * qzms24temp;
            sfour = sfour / satrec.radiusearthkm + 1.0;
        }
        double pinvsq = 1.0 / posq;

        double tsi = 1.0 / (ao - sfour);
        satrec.eta = ao * satrec.ecco * tsi;
        double etasq = satrec.eta * satrec.eta;
        double eeta = satrec.ecco * satrec.eta;
        double psisq = fabs(1.0 - etasq);
        double coef = qzms24 * pow(tsi, 4.0);
        double coef1 = coef / pow(psisq, 3.5);
        double cc2 = coef1 * satrec.no_unkozai *
                     (ao * (1.0 + 1.5 * etasq + eeta * (4.0 + etasq)) +
                      0.375 * satrec.j2 * tsi / psisq * satrec.con41 *
                          (8.0 + 3.0 * etasq * (8.0 + etasq)));
        satrec.cc1 = satrec.bstar * cc2;
        double cc3 = 0.0;
        if (satrec.ecco > 1.0e-4)
            cc3 = -2.0 * coef * tsi * satrec.j3oj2 * satrec.no_unkozai * sinio /
                  satrec.ecco;
        satrec.x1mth2 = 1.0 - cosio2;
        satrec.cc4 =
            2.0 * satrec.no_unkozai * coef1 * ao * omeosq *
            (satrec.eta * (2.0 + 0.5 * etasq) +
             satrec.ecco * (0.5 + 2.0 * etasq) -
             satrec.j2 * tsi / (ao * psisq) *
                 (-3.0 * satrec.con41 *
                      (1.0 - 2.0 * eeta + etasq * (1.5 - 0.5 * eeta)) +
                  0.75 * satrec.x1mth2 * (2.0 * etasq - eeta * (1.0 + etasq)) *
                      cos(2.0 * satrec.argpo)));
        satrec.cc5 = 2.0 * coef1 * ao * omeosq *
                     (1.0 + 2.75 * (etasq + eeta) + eeta * etasq);
        double cosio4 = cosio2 * cosio2;
        double temp1 = 1.5 * satrec.j2 * pinvsq * satrec.no_unkozai;
        double temp2 = 0.5 * temp1 * satrec.j2 * pinvsq;
        double temp3 =
            -0.46875 * satrec.j4 * pinvsq * pinvsq * satrec.no_unkozai;
        satrec.mdot =
            satrec.no_unkozai + 0.5 * temp1 * rteosq * satrec.con41 +
            0.0625 * temp2 * rteosq * (13.0 - 78.0 * cosio2 + 137.0 * cosio4);
        satrec.argpdot =
            -0.5 * temp1 * con42 +
            0.0625 * temp2 * (7.0 - 114.0 * cosio2 + 395.0 * cosio4) +
            temp3 * (3.0 - 36.0 * cosio2 + 49.0 * cosio4);
        double xhdot1 = -temp1 * cosio;
        satrec.nodedot = xhdot1 + (0.5 * temp2 * (4.0 - 19.0 * cosio2) +
                                   2.0 * temp3 * (3.0 - 7.0 * cosio2)) *
                                      cosio;

        satrec.omgcof = satrec.bstar * cc3 * cos(satrec.argpo);
        satrec.xmcof = 0.0;
        if (satrec.ecco > 1.0e-4)
            satrec.xmcof = -x2o3 * coef * satrec.bstar / eeta;
        satrec.nodecf = 3.5 * omeosq * xhdot1 * satrec.cc1;
        satrec.t2cof = 1.5 * satrec.cc1;
        // sgp4fix for divide by zero with xinco = 180 deg
        if (fabs(cosio + 1.0) > 1.5e-12)
            satrec.xlcof = -0.25 * satrec.j3oj2 * sinio * (3.0 + 5.0 * cosio) /
                           (1.0 + cosio);
        else
            satrec.xlcof =
                -0.25 * satrec.j3oj2 * sinio * (3.0 + 5.0 * cosio) / temp4;
        satrec.aycof = -0.5 * satrec.j3oj2 * sinio;
        // sgp4fix use multiply for speed instead of pow
        double delmotemp = 1.0 + satrec.eta * cos(satrec.mo);
        satrec.delmo = delmotemp * delmotemp * delmotemp;
        satrec.sinmao = sin(satrec.mo);
        satrec.x7thm1 = 7.0 * cosio2 - 1.0;

        /* ----------- set variables if not deep space ----------- */
        if (satrec.isimp != 1) {
            double cc1sq = satrec.cc1 * satrec.cc1;
            satrec.d2 = 4.0 * ao * tsi * cc1sq;
            double temp = satrec.d2 * tsi * satrec.cc1 / 3.0;
            satrec.d3 = (17.0 * ao + sfour) * temp;
            satrec.d4 = 0.5 * temp * ao * tsi * (221.0 * ao + 31.0 * sfour) *
                        satrec.cc1;
            satrec.t3cof = satrec.d2 + 2.0 * cc1sq;
            satrec.t4cof =
                0.25 * (3.0 * satrec.d3 +
                        satrec.cc1 * (12.0 * satrec.d2 + 10.0 * cc1sq));
            satrec.t5cof =
                0.2 * (3.0 * satrec.d4 + 12.0 * satrec.cc1 * satrec.d3 +
                       6.0 * satrec.d2 * satrec.d2 +
                       15.0 * cc1sq * (2.0 * satrec.d2 + cc1sq));
        }
    }


    sgp4(satrec, 0.0, r, v);

    satrec.init = 'n';

    //#include "debug6.cpp"
    // sgp4fix return boolean. satrec.error contains any error codes
    return true;
}

/*-----------------------------------------------------------------------------
*
*                             procedure sgp4
*
*  this procedure is the sgp4 prediction model from space command. this is an
*    updated and combined version of sgp4 and sdp4, which were originally
*    published separately in spacetrack report #3. this version follows the
*    methodology from the aiaa paper (2006) describing the history and
*    development of the code.
*
*  author        : david vallado                  719-573-2600   28 jun 2005
*
*  inputs        :
*    satrec	 - initialised structure from sgp4init() call.
*    tsince	 - time since epoch (minutes)
*
*  outputs       :
*    r           - position vector                     km
*    v           - velocity                            km/sec
*  return code - non-zero on error.
*                   1 - mean elements, ecc >= 1.0 or ecc < -0.001 or a < 0.95 er
*                   2 - mean motion less than 0.0
*                   3 - pert elements, ecc < 0.0  or  ecc > 1.0
*                   4 - semi-latus rectum < 0.0
*                   5 - epoch elements are sub-orbital
*                   6 - satellite has decayed
*
*                   Code has been modified by Jack McRobbie and the Melbourne space Program
*                   */
bool sgp4
    (
    elsetrec& satrec, double tsince,
    double r[3], double v[3]
    )
{
    double am, axnl, aynl, cosim,
        coseo1, cosip,
        em, ecose, el2, eo1,
        ep, esine, argpm, argpp, argpdf, pl, mrt = 0.0,
        sinim,
        sineo1, sinip,
        t2, tem5, temp,
        tempa, tempe, templ, u,
        inclm, mm,
        nm, nodem, xincp, xl, xlm, mp,
        xmdf, nodedf, nodep,
        twopi, x2o3, vkmpersec;


    /* ------------------ set mathematical constants --------------- */
    // sgp4fix divisor for divide by zero check on inclination
    // the old check used 1.0 + cos(pi-1.0e-9), but then compared it to
    // 1.5 e-12, so the threshold was changed to 1.5e-12 for consistency

    twopi = 2.0 * pi;
    x2o3 = 2.0 / 3.0;
    // sgp4fix identify constants and allow alternate values
    // getgravconst( whichconst, tumin, mu, radiusearthkm, xke, j2, j3, j4, j3oj2 );
    vkmpersec = satrec.radiusearthkm * satrec.xke / 60.0;

    /* --------------------- clear sgp4 error flag ----------------- */
    satrec.t = tsince;
    satrec.error = 0;

    /* ------- update for secular gravity and atmospheric drag ----- */
    xmdf = satrec.mo + satrec.mdot * satrec.t;
    argpdf = satrec.argpo + satrec.argpdot * satrec.t;
    nodedf = satrec.nodeo + satrec.nodedot * satrec.t;
    argpm = argpdf;
    mm = xmdf;
    t2 = satrec.t * satrec.t;
    nodem = nodedf + satrec.nodecf * t2;
    tempa = 1.0 - satrec.cc1 * satrec.t;
    tempe = satrec.bstar * satrec.cc4 * satrec.t;
    templ = satrec.t2cof * t2;

    if (satrec.isimp != 1)
    {
        double delomg = satrec.omgcof * satrec.t;
        // sgp4fix use mutliply for speed instead of pow
        double delmtemp = 1.0 + satrec.eta * cos(xmdf);
        double delm = satrec.xmcof *
            (delmtemp * delmtemp * delmtemp -
            satrec.delmo);
        temp = delomg + delm;
        mm = xmdf + temp;
        argpm = argpdf - temp;
        double t3 = t2 * satrec.t;
        double t4 = t3 * satrec.t;
        tempa = tempa - satrec.d2 * t2 - satrec.d3 * t3 -
            satrec.d4 * t4;
        tempe = tempe + satrec.bstar * satrec.cc5 * (sin(mm) -
            satrec.sinmao);
        templ = templ + satrec.t3cof * t3 + t4 * (satrec.t4cof +
            satrec.t * satrec.t5cof);
    }

    nm = satrec.no_unkozai;
    em = satrec.ecco;
    inclm = satrec.inclo;

    if (nm <= 0.0)
    {

        satrec.error = 2;

        return false;
    }
    am = pow((satrec.xke / nm), x2o3) * tempa * tempa;
    nm = satrec.xke / pow(am, 1.5);
    em = em - tempe;

    // fix tolerance for error recognition
    // sgp4fix am is fixed from the previous nm check
    if ((em >= 1.0) || (em < -0.001)/* || (am < 0.95)*/)
    {
        //         printf("# error em %f\n", em);
        satrec.error = 1;
        // sgp4fix to return if there is an error in eccentricity
        return false;
    }
    // sgp4fix fix tolerance to avoid a divide by zero
    if (em < 1.0e-6)
        em = 1.0e-6;
    mm = mm + satrec.no_unkozai * templ;
    xlm = mm + argpm + nodem;

    nodem = fmod(nodem, twopi);
    argpm = fmod(argpm, twopi);
    xlm = fmod(xlm, twopi);
    mm = fmod(xlm - argpm - nodem, twopi);

    // sgp4fix recover singly averaged mean elements
    satrec.am = am;
    satrec.em = em;
    satrec.im = inclm;
    satrec.Om = nodem;
    satrec.om = argpm;
    satrec.mm = mm;
    satrec.nm = nm;

    /* ----------------- compute extra mean quantities ------------- */
    sinim = sin(inclm);
    cosim = cos(inclm);

    /* -------------------- add lunar-solar periodics -------------- */
    ep = em;
    xincp = inclm;
    argpp = argpm;
    nodep = nodem;
    mp = mm;
    sinip = sinim;
    cosip = cosim;



    /* -------------------- long period periodics ------------------ */

    axnl = ep * cos(argpp);
    temp = 1.0 / (am * (1.0 - ep * ep));
    aynl = ep* sin(argpp) + temp * satrec.aycof;
    xl = mp + argpp + nodep + temp * satrec.xlcof * axnl;


    /* --------------------- solve kepler's equation --------------- */
    u = fmod(xl - nodep, twopi);
    eo1 = u;
    tem5 = 9999.9;
    //   sgp4fix for kepler iteration
    for(int i =1; (fabs(tem5) >= 1.0e-12) && (i <= 10); i++)
    {
        sineo1 = sin(eo1);
        coseo1 = cos(eo1);
        tem5 = 1.0 - coseo1 * axnl - sineo1 * aynl;
        tem5 = (u - aynl * coseo1 + axnl * sineo1 - eo1) / tem5;
        if (fabs(tem5) >= 0.95)
            tem5 = tem5 > 0.0 ? 0.95 : -0.95;
        eo1 = eo1 + tem5;
    }

    // Check for convergence
    if (fabs(tem5) >= 1.0e-12) {
        etl::exception e("SGP4Funcs::sgp4 numerical solver didn't converge",
                         "__FILE__",__LINE__);
        throw e;
    }

    /* ------------- short period preliminary quantities ----------- */
    ecose = axnl*coseo1 + aynl*sineo1;
    esine = axnl*sineo1 - aynl*coseo1;
    el2 = axnl*axnl + aynl*aynl;
    pl = am*(1.0 - el2);
    if (pl < 0.0)
    {
        //         printf("# error pl %f\n", pl);
        satrec.error = 4;
        // sgp4fix add return
        return false;
    }
    else
    {
        double rl = am * (1.0 - ecose);
        double rdotl = sqrt(am) * esine / rl;
        double rvdotl = sqrt(pl) / rl;
        double betal = sqrt(1.0 - el2);
        temp = esine / (1.0 + betal);
        double sinu = am / rl * (sineo1 - aynl - axnl * temp);
        double cosu = am / rl * (coseo1 - axnl + aynl * temp);
        double su = atan2(sinu, cosu);
        double sin2u = (cosu + cosu) * sinu;
        double cos2u = 1.0 - 2.0 * sinu * sinu;
        temp = 1.0 / pl;
        double temp1 = 0.5 * satrec.j2 * temp;
        double temp2 = temp1 * temp;

        /* -------------- update for short period periodics ------------ */
        /*
        if (satrec.method == 'd')
        {
            cosisq = cosip * cosip;
            satrec.con41 = 3.0*cosisq - 1.0;
            satrec.x1mth2 = 1.0 - cosisq;
            satrec.x7thm1 = 7.0*cosisq - 1.0;
        } */
        mrt = rl * (1.0 - 1.5 * temp2 * betal * satrec.con41) +
            0.5 * temp1 * satrec.x1mth2 * cos2u;
        su = su - 0.25 * temp2 * satrec.x7thm1 * sin2u;
        double xnode = nodep + 1.5 * temp2 * cosip * sin2u;
        double xinc = xincp + 1.5 * temp2 * cosip * sinip * cos2u;
        double mvt = rdotl - nm * temp1 * satrec.x1mth2 * sin2u / satrec.xke;
        double rvdot = rvdotl + nm * temp1 * (satrec.x1mth2 * cos2u +
            1.5 * satrec.con41) / satrec.xke;

        /* --------------------- orientation vectors ------------------- */
        double sinsu = sin(su);
        double cossu = cos(su);
        double snod = sin(xnode);
        double cnod = cos(xnode);
        double sini = sin(xinc);
        double cosi = cos(xinc);
        double xmx = -snod * cosi;
        double xmy = cnod * cosi;
        double ux = xmx * sinsu + cnod * cossu;
        double uy = xmy * sinsu + snod * cossu;
        double uz = sini * sinsu;
        double vx = xmx * cossu - cnod * sinsu;
        double vy = xmy * cossu - snod * sinsu;
        double vz = sini * cossu;

        /* --------- position and velocity (in km and km/sec) ---------- */
        r[0] = (mrt * ux)* satrec.radiusearthkm;
        r[1] = (mrt * uy)* satrec.radiusearthkm;
        r[2] = (mrt * uz)* satrec.radiusearthkm;
        v[0] = (mvt * ux + rvdot * vx) * vkmpersec;
        v[1] = (mvt * uy + rvdot * vy) * vkmpersec;
        v[2] = (mvt * uz + rvdot * vz) * vkmpersec;
    }  // if pl > 0

    // sgp4fix for decaying satellites
    if (mrt < 1.0)
    {
        //         printf("# decay condition %11.6f \n",mrt);
        satrec.error = 6;
        return false;
    }

    //#include "debug7.cpp"
    return true;
}  // sgp4

}

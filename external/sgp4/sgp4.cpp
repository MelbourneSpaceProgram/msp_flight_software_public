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
#include <external/etl/exception.h>
#include <external/sgp4/sgp4.h>
#include <math.h>

/*-----------------------------------------------------------------------------
*
*                           procedure InitialiseSgp4
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

void Sgp4::InitialiseSgp4(double xke, double j2, double ecco, double epoch,
                          double inclo, double no_kozai, char opsmode,
                          char& method, double& ainv, double& ao, double& con41,
                          double& con42, double& cosio, double& cosio2,
                          double& eccsq, double& omeosq, double& posq,
                          double& rp, double& rteosq, double& sinio,
                          double& no_unkozai) {
    /* --------------------- local variables ------------------------ */
    double ak, d1, del, adel, po, x2o3;


    /* ----------------------- earth constants ---------------------- */
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
*                             procedure InitialisePropagator
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

void Sgp4::InitialisePropagator(gravconsttype whichconst, char opsmode,
                                const int satn, const double epoch,
                                const double xbstar, const double xndot,
                                const double xnddot, const double xecco,
                                const double xargpo, const double xinclo,
                                const double xmo, const double xno_kozai,
                                const double xnodeo, elsetrec& satrec) {
    /* --------------------- local variables ------------------------ */
    double ao, ainv, con42, cosio, sinio, cosio2, eccsq, omeosq, posq, rp,
        rteosq, qzms2t, ss, x2o3, r[3], v[3], qzms2ttemp;

    /* ------------------------ initialization --------------------- */
    const double threshold = 1.5e-12;

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
    Sgp4Utils::GetGravConstants(whichconst, satrec.tumin, satrec.mu,
                                satrec.radiusearthkm, satrec.xke, satrec.j2,
                                satrec.j3, satrec.j4, satrec.j3oj2);

    //-------------------------------------------------------------------------

    satrec.operationmode = opsmode;
    satrec.satnum = satn;

    satrec.bstar = xbstar;
    satrec.ndot = xndot;
    satrec.nddot = xnddot;
    satrec.ecco = xecco;
    satrec.argpo = xargpo;
    satrec.inclo = xinclo;
    satrec.mo = xmo;
    satrec.no_kozai = xno_kozai;
    satrec.nodeo = xnodeo;

    // single averaged mean elements
    satrec.am = satrec.em = satrec.im = satrec.Om = satrec.mm = satrec.nm = 0.0;

    /* ------------------------ earth constants ----------------------- */
    ss = 78.0 / satrec.radiusearthkm + 1.0;
    qzms2ttemp = (120.0 - 78.0) / satrec.radiusearthkm;
    qzms2t = qzms2ttemp * qzms2ttemp * qzms2ttemp * qzms2ttemp;
    x2o3 = 2.0 / 3.0;

    satrec.init = 'y';
    satrec.t = 0.0;

    Sgp4::InitialiseSgp4(satrec.xke, satrec.j2, satrec.ecco, epoch,
                         satrec.inclo, satrec.no_kozai, satrec.operationmode,
                         satrec.method, ainv, ao, satrec.con41, con42, cosio,
                         cosio2, eccsq, omeosq, posq, rp, rteosq, sinio,
                         satrec.no_unkozai);
    satrec.a = pow(satrec.no_unkozai * satrec.tumin, (-2.0 / 3.0));
    satrec.alta = satrec.a * (1.0 + satrec.ecco) - 1.0;
    satrec.altp = satrec.a * (1.0 - satrec.ecco) - 1.0;

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
        if (fabs(cosio + 1.0) > 1.5e-12)
            satrec.xlcof = -0.25 * satrec.j3oj2 * sinio * (3.0 + 5.0 * cosio) /
                           (1.0 + cosio);
        else
            satrec.xlcof =
                -0.25 * satrec.j3oj2 * sinio * (3.0 + 5.0 * cosio) / threshold;
        satrec.aycof = -0.5 * satrec.j3oj2 * sinio;
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

    Propagate(satrec, 0.0, r, v);

    satrec.init = 'n';
}

/*-----------------------------------------------------------------------------
 *
 *                             procedure Propagate
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
 *
 *  Code has been modified by Jack McRobbie and the Melbourne Space Program
 */
void Sgp4::Propagate(elsetrec& satrec, double tsince, double position[3],
                     double velocity[3]) {
    double am, axnl, aynl, cosim, coseo1, cosip, em, ecose, el2, eo1, ep, esine,
        argpm, argpp, argpdf, pl,
        mrt = 0.0, sinim, sineo1, sinip, t2, tem5, temp, tempa, tempe, templ, u,
        inclm, mm, nm, nodem, xincp, xl, xlm, mp, xmdf, nodedf, nodep, x2o3,
        vkmpersec;

    /* ------------------ set mathematical constants --------------- */
    x2o3 = 2.0 / 3.0;
    vkmpersec = satrec.radiusearthkm * satrec.xke / 60.0;

    satrec.t = tsince;

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

    if (satrec.isimp != 1) {
        double delomg = satrec.omgcof * satrec.t;
        double delmtemp = 1.0 + satrec.eta * cos(xmdf);
        double delm =
            satrec.xmcof * (delmtemp * delmtemp * delmtemp - satrec.delmo);
        temp = delomg + delm;
        mm = xmdf + temp;
        argpm = argpdf - temp;
        double t3 = t2 * satrec.t;
        double t4 = t3 * satrec.t;
        tempa = tempa - satrec.d2 * t2 - satrec.d3 * t3 - satrec.d4 * t4;
        tempe = tempe + satrec.bstar * satrec.cc5 * (sin(mm) - satrec.sinmao);
        templ = templ + satrec.t3cof * t3 +
                t4 * (satrec.t4cof + satrec.t * satrec.t5cof);
    }

    nm = satrec.no_unkozai;
    em = satrec.ecco;
    inclm = satrec.inclo;

    if (nm <= 0.0) {
        etl::exception e("mean motion <= 0.0", __FILE__, __LINE__);
        throw e;
    }

    am = pow((satrec.xke / nm), x2o3) * tempa * tempa;
    nm = satrec.xke / pow(am, 1.5);
    em = em - tempe;

    if ((em >= 1.0) || (em < -0.001)) {
        etl::exception e("Eccentricity out of bounds", __FILE__, __LINE__);
        throw e;
    }
    if (em < 1.0e-6) em = 1.0e-6;
    mm = mm + satrec.no_unkozai * templ;
    xlm = mm + argpm + nodem;

    nodem = fmod(nodem, 2 * Sgp4Utils::kPi);
    argpm = fmod(argpm, 2 * Sgp4Utils::kPi);
    xlm = fmod(xlm, 2 * Sgp4Utils::kPi);
    mm = fmod(xlm - argpm - nodem, 2 * Sgp4Utils::kPi);

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
    aynl = ep * sin(argpp) + temp * satrec.aycof;
    xl = mp + argpp + nodep + temp * satrec.xlcof * axnl;

    /* --------------------- solve kepler's equation --------------- */
    u = fmod(xl - nodep, 2 * Sgp4Utils::kPi);
    eo1 = u;
    tem5 = 9999.9;
    for (int i = 1; (fabs(tem5) >= 1.0e-12) && (i <= 10); i++) {
        sineo1 = sin(eo1);
        coseo1 = cos(eo1);
        tem5 = 1.0 - coseo1 * axnl - sineo1 * aynl;
        tem5 = (u - aynl * coseo1 + axnl * sineo1 - eo1) / tem5;
        if (fabs(tem5) >= 0.95) tem5 = tem5 > 0.0 ? 0.95 : -0.95;
        eo1 = eo1 + tem5;
    }

    // Check for convergence
    if (fabs(tem5) >= 1.0e-12) {
        etl::exception e("Numerical solver didn't converge", __FILE__,
                         __LINE__);
        throw e;
    }

    /* ------------- short period preliminary quantities ----------- */
    ecose = axnl * coseo1 + aynl * sineo1;
    esine = axnl * sineo1 - aynl * coseo1;
    el2 = axnl * axnl + aynl * aynl;
    pl = am * (1.0 - el2);
    if (pl < 0.0) {
        etl::exception e("pl < 0.0", __FILE__, __LINE__);
        throw e;
    } else {
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
        mrt = rl * (1.0 - 1.5 * temp2 * betal * satrec.con41) +
              0.5 * temp1 * satrec.x1mth2 * cos2u;
        su = su - 0.25 * temp2 * satrec.x7thm1 * sin2u;
        double xnode = nodep + 1.5 * temp2 * cosip * sin2u;
        double xinc = xincp + 1.5 * temp2 * cosip * sinip * cos2u;
        double mvt = rdotl - nm * temp1 * satrec.x1mth2 * sin2u / satrec.xke;
        double rvdot =
            rvdotl + nm * temp1 * (satrec.x1mth2 * cos2u + 1.5 * satrec.con41) /
                         satrec.xke;

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
        position[0] = (mrt * ux) * satrec.radiusearthkm;
        position[1] = (mrt * uy) * satrec.radiusearthkm;
        position[2] = (mrt * uz) * satrec.radiusearthkm;
        velocity[0] = (mvt * ux + rvdot * vx) * vkmpersec;
        velocity[1] = (mvt * uy + rvdot * vy) * vkmpersec;
        velocity[2] = (mvt * uz + rvdot * vz) * vkmpersec;
    }  // if pl > 0

    if (mrt < 1.0) {
        etl::exception e("mrt < 1.0", __FILE__, __LINE__);
        throw e;
    }
}  // sgp4

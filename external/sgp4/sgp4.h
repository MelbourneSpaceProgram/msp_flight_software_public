#ifndef EXTERNAL_SGP4_SGP4_H_
#define EXTERNAL_SGP4_SGP4_H_
/*     ----------------------------------------------------------------
 *
 *                                 SGP4.h
 *
 *    this file contains the sgp4 procedures for analytical propagation
 *    of a satellite. the code was originally released in the 1980 and 1986
 *    spacetrack papers. a detailed discussion of the theory and history
 *    may be found in the 2006 aiaa paper by vallado, crawford, hujsak,
 *    and kelso.
 *
 *    current :
 *               7 dec 15  david vallado
 *                           fix jd, jdfrac
 *    changes :
 *               3 nov 14  david vallado
 *                           update to msvs2013 c++
 *              30 Dec 11  david vallado
 *                           consolidate updated code version
 *              30 Aug 10  david vallado
 *                           delete unused variables in initl
 *                           replace pow inetger 2, 3 with multiplies for speed
 *               3 Nov 08  david vallado
 *                           put returns in for error codes
 *              29 sep 08  david vallado
 *                           fix atime for faster operation in dspace
 *                           add operationmode for afspc (a) or improved (i)
 *                           performance mode
 *              20 apr 07  david vallado
 *                           misc fixes for constants
 *              11 aug 06  david vallado
 *                           chg lyddane choice back to strn3, constants, misc
 * doc 15 dec 05  david vallado misc fixes 26 jul 05  david vallado fixes for
 * paper note that each fix is preceded by a comment
 * with "sgp4fix" and an explanation of what was
 * changed 10 aug 04  david vallado 2nd printing
 * baseline working 14 may 01  david vallado 2nd
 * edition baseline 80  norad original baseline
 *       ---------------------------------------------------------------- */

#include <external/sgp4/sgp4_utils.h>

typedef struct elsetrec {
    long int satnum;
    int epochyr, epochtynumrev;
    int error;
    char operationmode;
    char init, method;

    /* Near Earth */
    int isimp;
    double aycof, con41, cc1, cc4, cc5, d2, d3, d4, delmo, eta, argpdot, omgcof,
        sinmao, t, t2cof, t3cof, t4cof, t5cof, x1mth2, x7thm1, mdot, nodedot,
        xlcof, xmcof, nodecf;

    double a, altp, alta, epochdays, jdsatepoch, jdsatepochF, nddot, ndot,
        bstar, rcse, inclo, nodeo, ecco, argpo, mo, no_kozai;
    // sgp4fix add new variables from tle
    char classification, intldesg[11];
    int ephtype;
    long elnum, revnum;
    // sgp4fix add unkozai'd variable
    double no_unkozai;
    // sgp4fix add singly averaged variables
    double am, em, im, Om, om, mm, nm;
    // sgp4fix add constant parameters to eliminate mutliple calls during
    // execution
    double tumin, mu, radiusearthkm, xke, j2, j3, j4, j3oj2;

    //       Additional elements to capture relevant TLE and object information:
    long dia_mm;                // RSO dia in mm
    double period_sec;          // Period in seconds
    unsigned char active;       // "Active S/C" flag (0=n, 1=y)
    unsigned char not_orbital;  // "Orbiting S/C" flag (0=n, 1=y)
    double rcs_m2;              // "RCS (m^2)" storage

} elsetrec;

class Sgp4 {
   public:
    static void InitialisePropagator(gravconsttype whichconst, char opsmode,
                                     const int satn, const double epoch,
                                     const double xbstar, const double xndot,
                                     const double xnddot, const double xecco,
                                     const double xargpo, const double xinclo,
                                     const double xmo, const double xno,
                                     const double xnodeo, elsetrec& satrec);

    static void Propagate(elsetrec& satrec, double tsince, double position[3],
                          double velocity[3]);

   private:
    static void InitialiseSgp4(double xke, double j2, double ecco, double epoch,
                               double inclo, double no_kozai, char opsmode,
                               char& method, double& ainv, double& ao,
                               double& con41, double& con42, double& cosio,
                               double& cosio2, double& eccsq, double& omeosq,
                               double& posq, double& rp, double& rteosq,
                               double& sinio, double& no_unkozai);
};

#endif  // EXTERNAL_SGP4_SGP4_H_

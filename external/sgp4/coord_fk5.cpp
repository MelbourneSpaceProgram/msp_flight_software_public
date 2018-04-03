/*       ----------------------------------------------------------------
*
*                              coordFK5.cpp
*
*  this file contains routines for iau-76/fk5 transformations.
*
*                          companion code for
*             fundamentals of astrodynamics and applications
*                                  2013
*                            by david vallado
*
*       (w) 719-573-2600, email dvallado@agi.com
*

*     *****************************************************************       */

#include <external/sgp4/coord_fk5.h>

namespace coordFK5 {

/* -----------------------------------------------------------------------------
 *
 *                           function teme_pef
 *
 *  this function transforms a vector between the true equator mean equinox
 * system, (teme) and the pseudo earth fixed system (pef). this apporach is
 * preferred because there is little ambiguity in the implementation.
 *
 *  author        : david vallado                  719-573-2600   31 may 2007
 *
 *  revisions
 *
 *  inputs          description                    range / units
 *    rteme       - position vector of date
 *                    true equator, mean equinox   km
 *    vteme       - velocity vector of date
 *                    true equator, mean equinox   km/s
 *    ateme       - acceleration vector of date
 *                    true equator, mean equinox   km/s2
 *    direct      - direction of transfer          eFrom, 'TOO '
 *    jdut1       - julian centuries of ut1        days    // FUCK THIS IS
 * SUPPOSED TO BE JULIAN DATES NOT CENTURIES, CHECK gstime function
 *
 *  outputs       :
 *    rpef        - position vector pef            km
 *    vpef        - velocity vector pef            km/s
 *    apef        - acceleration vector pef        km/s2
 *
 *  locals        :
 *    tm          - matrix for gmst
 *    gmst        - mean greenwich sidereal time   0 to 2pi rad*
 *
 *  coupling      :
 *
 *  references    :
 *    vallado       2013, 233, Eq 3-90
 * ---------------------------------------------------------------------------
 */

void teme_pef(double rteme[3], double vteme[3], double ateme[3],
              edirection direct, double rpef[3], double vpef[3], double apef[3],
              double jdut1) {
    double gmst;
    //        std::vector< std::vector<double> > tm(3,3), tmp;
    std::vector<std::vector<double> > tm =
        std::vector<std::vector<double> >(3, std::vector<double>(3));
    std::vector<std::vector<double> > tmp;

    gmst = astTime::gstime(jdut1);

    tm[0][0] = cos(gmst);
    tm[0][1] = sin(gmst);
    tm[0][2] = 0.0;
    tm[1][0] = -sin(gmst);
    tm[1][1] = cos(gmst);
    tm[1][2] = 0.0;
    tm[2][0] = 0.0;
    tm[2][1] = 0.0;
    tm[2][2] = 1.0;

    if (direct == eTo) {
        astMath::matvecmult(tm, rteme, rpef);
        astMath::matvecmult(tm, vteme, vpef);
        astMath::matvecmult(tm, ateme, apef);
    } else {
        astMath::mattrans(tm, tmp, 3, 3);
        astMath::matvecmult(tmp, rpef, rteme);
        astMath::matvecmult(tmp, vpef, vteme);
        astMath::matvecmult(tmp, apef, ateme);
    }
}
}  // namespace coordFK5

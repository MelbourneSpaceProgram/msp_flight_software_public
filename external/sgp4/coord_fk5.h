#ifndef _coordFK5_h_
#define _coordFK5_h_
/*      ----------------------------------------------------------------
 *
 *                              coordFK5.h
 *
 *  this file contains routines for iau-76/fk5 conversions.
 *
 *    current :
 *              30 sep 15  david vallado
 *                           fix jd, jdfrac
 *    changes :
 *               3 nov 14  david vallado
 *                           update to msvs2013 c++
 *              22 may 09  david vallado
 *                           add all transformation
 *              21 jan 08  david vallado
 *                           fix matrix operations
 *              30 may 07  david vallado
 *                           3rd edition baseline
 *               1 dec 05  david vallado
 *                           add frame bias, restructure
 *              24 mar 05  david vallado
 *                           conversion to c++
 *              21 feb 05  david vallado
 *                           original baseline
 *     ***************************************************************** */

#include <math.h>
#include "ast_math.h"
#include "ast_time.h"

namespace coordFK5 {

void teme_pef(double rteme[3], double vteme[3], double ateme[3],
              edirection direct, double rpef[3], double vpef[3], double apef[3],
              double jdut1);

}  // namespace coordFK5

#endif

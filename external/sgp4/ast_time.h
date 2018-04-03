#ifndef _astTime_h_
#define _astTime_h_
/* --------------------------------------------------------------------
*
*                                astTime.h
*
*    this file contains miscallaneous time functions.
*
*    current :
*              30 sep 15  david vallado
*                           fix jd, jdfrac
*    changes :
*               3 nov 14  david vallado
*                           update to msvs2013 c++
*               4 may 09  david vallado
*                           misc updates
*              13 feb 08  david vallado
*                           add getmon
*              15 mar 07  david vallado
*                           3rd edition baseline
*              20 jul 05  david vallado
*                           2nd printing baseline
*              14 may 01  david vallado
*                           2nd edition baseline
*              23 nov 87  david vallado
*                           original baseline
  ---------------------------------------------------------------------- */

#include <math.h>

// global object definitions
typedef enum { eTo, eFrom } edirection;

namespace astTime {
double gstime(double jdut1);
};

#endif

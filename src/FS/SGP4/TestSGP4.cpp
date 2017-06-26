/* ---------------------------------------------------------------------
*
*                              testSGP4.cpp
*
*  this program tests the sgp4 propagator. an stk ephemeris file is generated
*  along with the test output. the code for this is left justified for easy
*  location.
*
*                          companion code for
*             fundamentals of astrodynamics and applications
*                                  2013
*                            by david vallado
*
*     (w) 719-573-2600, email dvallado@agi.com, davallado@gmail.com
*     *****************************************************************
*    current :
*               7 dec 15  david vallado
*                           fix jd, jdfrac
*    changes :
*               3 nov 14  david vallado
*                           update to msvs2013 c++
*              11 nov 13  david vallado
*                           conversion to msvs c++
*                           misc fixes to constants and options
*                           add singly averaged state elements to be exported
*               3 sep 08  david vallado
*                           add switch for afspc compatibility and improved operation
*              14 may 08  david vallado
*                           fixes for linux suggested by brian micek
*                           misc fixes noted by the community - manual operation,
*                           formats, char lengths
*              14 aug 06  david vallado
*                           update mfe for verification time steps, constants
*              20 jul 05  david vallado
*                           fixes for paper, corrections from paul crawford
*               7 jul 04  david vallado
*                           fix record file and get working
*              14 may 01  david vallado
*                           2nd edition baseline
*                     80  norad
*                           original baseline
*       ----------------------------------------------------------------      */


//#include "stdafx.h"
#include "astTime.h"
#include "coordFK5.h"
#include "ast2Body.h"
#include "astMath.h"
#include <math.h>
#include "SGP4.h"

//using namespace System;
//using namespace SGP4Funcs;

#define pi 3.14159265358979323846

char typerun, typeinput, opsmode;
gravconsttype  whichconst;



int _tmain()
{
	// ----------------------------  locals  -------------------------------
	double p, a, ecc, incl, node, argp, nu, m, arglat, truelon, lonper;
	double sec, jd, jdFrac, rad, tsince, startmfe, stopmfe, deltamin;
	int  year; int mon; int day; int hr; int min;
	char longstr1[130];
	typedef char str3[4];
	str3 monstr[13];
	char outname[64];
	char longstr2[130];
	double ro[3];
	double vo[3];
	// sgp4fix demonstrate method of running SGP4 directly from orbital element values
	//1 08195U 75081A   06176.33215444  .00000099  00000-0  11873-3 0   813
	//2 08195  64.1586 279.0717 6877146 264.7651  20.2257  2.00491383225656

	// ISS 2017 Jun 25th
	//1 25544U 98067A   17175.57344519  .00001454  00000-0  29274-4 0  9991
	//2 25544  51.6422   1.5150 0004595 318.8096 119.3859 15.54078614 62906


	//ISS (ZARYA)             
	//1 25544U 98067A   17177.22888889  .00001560  00000-0  30859-4 0  9997
//2 25544  51.6430 353.2604 0004633 325.9069  20.1341 15.54085667 63157

	elsetrec satrec;
	const double deg2rad = pi / 180.0;         //   0.0174532925199433
	const double xpdotp = 1440.0 / (2.0 *pi);  // 229.1831180523293

	whichconst = wgs72;
	opsmode = 'a';
	satrec.satnum = 25544; //08195
	satrec.jdsatepoch = 2457931.0;//2453911.0;
	satrec.jdsatepochF = 0.077326389;//0.8321544402;
	satrec.no_kozai = 15.54085667;//2.00491383;
	satrec.ecco = 0.0004633;//0.6877146;
	satrec.inclo = 51.6430;//64.1586;
	satrec.nodeo = 353.2604;//279.0717;
	satrec.argpo = 325.9069;//264.7651;
	satrec.mo = 20.1341;//20.2257;
	satrec.nddot = 0.00000e0;//0.00000e0;
	satrec.bstar = 0.30859e-4;//0.11873e-3;
	satrec.ndot = .00001560;//0.00000099;
	satrec.elnum = 9991;//813;
	satrec.revnum = 6290;//22565;
	satrec.classification = 'U';
//	strncpy_s(satrec.intldesg, "          ", 11 * sizeof(char));
	//satrec.intldesg = ['h','a','h','a','h','a','h','a','h','a','h'];
	satrec.ephtype = 0;

	// convert units and initialize
	satrec.no_kozai = satrec.no_kozai / xpdotp; //* rad/min
	satrec.ndot = satrec.ndot / (xpdotp*1440.0);  //* ? * minperday
	satrec.nddot = satrec.nddot / (xpdotp*1440.0 * 1440);
	satrec.inclo = satrec.inclo  * deg2rad;
	satrec.nodeo = satrec.nodeo  * deg2rad;
	satrec.argpo = satrec.argpo  * deg2rad;
	satrec.mo = satrec.mo     * deg2rad;

	// set start/stop times for propagation
	startmfe = 0.0;
	stopmfe = 2880.0;
	deltamin = 1.0;

	SGP4Funcs::sgp4init(whichconst, opsmode, satrec.satnum, satrec.jdsatepoch + satrec.jdsatepochF - 2433281.5, satrec.bstar,
		satrec.ndot, satrec.nddot, satrec.ecco, satrec.argpo, satrec.inclo, satrec.mo, satrec.no_kozai,
		satrec.nodeo, satrec);

	tsince = startmfe;
	while ((tsince < stopmfe) && (satrec.error == 0))
	{
		tsince = tsince + deltamin;

		if (tsince > stopmfe)
			tsince = stopmfe;

		SGP4Funcs::sgp4(satrec, tsince, ro, vo);
		jd = satrec.jdsatepoch;
		jdFrac = satrec.jdsatepochF + tsince / 1440.0;
		if (jdFrac < 0.0)
		{
			jd = jd - 1.0;
			jdFrac = jdFrac - 1.0;
		}
		SGP4Funcs::invjday(jd, jdFrac, year, mon, day, hr, min, sec);

//		fprintf(outfile, " %16.8f %16.8f %16.8f %16.8f %12.9f %12.9f %12.9f",
//			tsince, ro[0], ro[1], ro[2], vo[0], vo[1], vo[2]);
		double distance = sqrt(pow(ro[0],2) + pow(ro[1], 2) + pow(ro[2],2))-6371;
		printf("%lf",startmfe);
		printf("distance is %16.8f km for %d/%d/%d %d:%d:%2.3lf \n",distance,year,mon,day,hr,min,sec);
	} // while propagating the orbit


	return 0;
}  // testSGP4


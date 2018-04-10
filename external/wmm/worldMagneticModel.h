#ifndef _WORLDMAGNETICMODEL_H_
#define _WORLDMAGNETICMODEL_H_

#include <math.h>

struct r_vector {
    double x;
    double y;
    double z;
    double h;
    double ti;
};
static void E0000(int IENTRY, int *maxdeg, double alt, double glat, double glon,
                  double time, double *dec, double *dip, double *ti,
                  double *gv);

void geomag(int *maxdeg);

void geomg1(double alt, double glat, double glon, double time, double *dec,
            double *dip, double *ti, double *gv);

char geomag_introduction(double epochlowlim);

r_vector MagModel(double date, double altitude, double lat, double longitude);

extern const double wmmdata[90][6];

#endif /* EXTERNAL_WORLDMAGNETICMODEL_WORLDMAGNETICMODEL_H_ */

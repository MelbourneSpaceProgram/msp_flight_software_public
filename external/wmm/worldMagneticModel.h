#ifndef _WORLDMAGNETICMODEL_H_
#define _WORLDMAGNETICMODEL_H_

#include <math.h>

struct r_vector {
    float x;
    float y;
    float z;
    float h;
    float ti;
};
static void E0000(int IENTRY, int *maxdeg, float alt, float glat, float glon,
                  float time, float *dec, float *dip, float *ti,
                  float *gv);

void geomag(int *maxdeg);

void geomg1(float alt, float glat, float glon, float time, float *dec,
            float *dip, float *ti, float *gv);

char geomag_introduction(float epochlowlim);

r_vector MagModel(float date, float altitude, float lat, float longitude);

extern const float wmmdata[90][6];

#endif /* EXTERNAL_WORLDMAGNETICMODEL_WORLDMAGNETICMODEL_H_ */

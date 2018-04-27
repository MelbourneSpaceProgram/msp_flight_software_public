#include <external/wmm/worldMagneticModel.h>
#include <src/util/data_types.h>
int my_isnan(float d) {
    return (d != d); /* IEEE: only NaN is not equal to itself */
}

#define NaN log(-1.0)

const float wmmdata[90][6] = {
    {1, 0, -29438.5, 0.0, 10.7, 0.0},   {1, 1, -1501.1, 4796.2, 17.9, -26.8},
    {2, 0, -2445.3, 0.0, -8.6, 0.0},    {2, 1, 3012.5, -2845.6, -3.3, -27.1},
    {2, 2, 1676.6, -642.0, 2.4, -13.3}, {3, 0, 1351.1, 0.0, 3.1, 0.0},
    {3, 1, -2352.3, -115.3, -6.2, 8.4}, {3, 2, 1225.6, 245.0, -0.4, -0.4},
    {3, 3, 581.9, -538.3, -10.4, 2.3},  {4, 0, 907.2, 0.0, -0.4, 0.0},
    {4, 1, 813.7, 283.4, 0.8, -0.6},    {4, 2, 120.3, -188.6, -9.2, 5.3},
    {4, 3, -335.0, 180.9, 4.0, 3.0},    {4, 4, 70.3, -329.5, -4.2, -5.3},
    {5, 0, -232.6, 0.0, -0.2, 0.0},     {5, 1, 360.1, 47.4, 0.1, 0.4},
    {5, 2, 192.4, 196.9, -1.4, 1.6},    {5, 3, -141.0, -119.4, 0.0, -1.1},
    {5, 4, -157.4, 16.1, 1.3, 3.3},     {5, 5, 4.3, 100.1, 3.8, 0.1},
    {6, 0, 69.5, 0.0, -0.5, 0.0},       {6, 1, 67.4, -20.7, -0.2, 0.0},
    {6, 2, 72.8, 33.2, -0.6, -2.2},     {6, 3, -129.8, 58.8, 2.4, -0.7},
    {6, 4, -29.0, -66.5, -1.1, 0.1},    {6, 5, 13.2, 7.3, 0.3, 1.0},
    {6, 6, -70.9, 62.5, 1.5, 1.3},      {7, 0, 81.6, 0.0, 0.2, 0.0},
    {7, 1, -76.1, -54.1, -0.2, 0.7},    {7, 2, -6.8, -19.4, -0.4, 0.5},
    {7, 3, 51.9, 5.6, 1.3, -0.2},       {7, 4, 15.0, 24.4, 0.2, -0.1},
    {7, 5, 9.3, 3.3, -0.4, -0.7},       {7, 6, -2.8, -27.5, -0.9, 0.1},
    {7, 7, 6.7, -2.3, 0.3, 0.1},        {8, 0, 24.0, 0.0, 0.0, 0.0},
    {8, 1, 8.6, 10.2, 0.1, -0.3},       {8, 2, -16.9, -18.1, -0.5, 0.3},
    {8, 3, -3.2, 13.2, 0.5, 0.3},       {8, 4, -20.6, -14.6, -0.2, 0.6},
    {8, 5, 13.3, 16.2, 0.4, -0.1},      {8, 6, 11.7, 5.7, 0.2, -0.2},
    {8, 7, -16.0, -9.1, -0.4, 0.3},     {8, 8, -2.0, 2.2, 0.3, 0.0},
    {9, 0, 5.4, 0.0, 0.0, 0.0},         {9, 1, 8.8, -21.6, -0.1, -0.2},
    {9, 2, 3.1, 10.8, -0.1, -0.1},      {9, 3, -3.1, 11.7, 0.4, -0.2},
    {9, 4, 0.6, -6.8, -0.5, 0.1},       {9, 5, -13.3, -6.9, -0.2, 0.1},
    {9, 6, -0.1, 7.8, 0.1, 0.0},        {9, 7, 8.7, 1.0, 0.0, -0.2},
    {9, 8, -9.1, -3.9, -0.2, 0.4},      {9, 9, -10.5, 8.5, -0.1, 0.3},
    {10, 0, -1.9, 0.0, 0.0, 0.0},       {10, 1, -6.5, 3.3, 0.0, 0.1},
    {10, 2, 0.2, -0.3, -0.1, -0.1},     {10, 3, 0.6, 4.6, 0.3, 0.0},
    {10, 4, -0.6, 4.4, -0.1, 0.0},      {10, 5, 1.7, -7.9, -0.1, -0.2},
    {10, 6, -0.7, -0.6, -0.1, 0.1},     {10, 7, 2.1, -4.1, 0.0, -0.1},
    {10, 8, 2.3, -2.8, -0.2, -0.2},     {10, 9, -1.8, -1.1, -0.1, 0.1},
    {10, 10, -3.6, -8.7, -0.2, -0.1},   {11, 0, 3.1, 0.0, 0.0, 0.0},
    {11, 1, -1.5, -0.1, 0.0, 0.0},      {11, 2, -2.3, 2.1, -0.1, 0.1},
    {11, 3, 2.1, -0.7, 0.1, 0.0},       {11, 4, -0.9, -1.1, 0.0, 0.1},
    {11, 5, 0.6, 0.7, 0.0, 0.0},        {11, 6, -0.7, -0.2, 0.0, 0.0},
    {11, 7, 0.2, -2.1, 0.0, 0.1},       {11, 8, 1.7, -1.5, 0.0, 0.0},
    {11, 9, -0.2, -2.5, 0.0, -0.1},     {11, 10, 0.4, -2.0, -0.1, 0.0},
    {11, 11, 3.5, -2.3, -0.1, -0.1},    {12, 0, -2.0, 0.0, 0.1, 0.0},
    {12, 1, -0.3, -1.0, 0.0, 0.0},      {12, 2, 0.4, 0.5, 0.0, 0.0},
    {12, 3, 1.3, 1.8, 0.1, -0.1},       {12, 4, -0.9, -2.2, -0.1, 0.0},
    {12, 5, 0.9, 0.3, 0.0, 0.0},        {12, 6, 0.1, 0.7, 0.1, 0.0},
    {12, 7, 0.5, -0.1, 0.0, 0.0},       {12, 8, -0.4, 0.3, 0.0, 0.0},
    {12, 9, -0.4, 0.2, 0.0, 0.0},       {12, 10, 0.2, -0.9, 0.0, 0.0},
    {12, 11, -0.9, -0.2, 0.0, 0.0},     {12, 12, 0.0, 0.7, 0.0, 0.0}};

r_vector MagModel(float date, float alt, float lat, float longitude) {
    static int maxdeg;
    float dec, dip, ti, gv;
    float rTd = 0.017453292;
    r_vector r_vector_data;

    /* INITIALIZE GEOMAG ROUTINE */

    maxdeg = 12;

    geomag(&maxdeg);

    geomg1(alt, lat, longitude, date, &dec, &dip, &ti, &gv);

    /*COMPUTE X, Y, Z, AND H COMPONENTS OF THE MAGNETIC FIELD*/
    r_vector_data.x = ti * cos(dec * rTd) * cos(dip * rTd);
    r_vector_data.y = ti * cos(dip * rTd) * sin(dec * rTd);
    r_vector_data.z = ti * sin(dip * rTd);
    r_vector_data.h = ti * cos(dip * rTd);
    r_vector_data.ti = ti;

    return r_vector_data;
}

static void schmidt_norm_to_unnorm(float snorm[169], float fm[13], int maxord,
                                   int j, int D1, int D2, float k[13][13],
                                   float flnmj, float c[13][13],
                                   float cd[13][13], float fn[13], int &m,
                                   float &otime, float &oalt, float &olat,
                                   float &olon) {
    snorm[0] = 1.0;
    fm[0] = 0.0;
    for (int n = 1; n <= maxord; n++) {
        snorm[n] = snorm[n - 1] * (float)((2 * n - 1)) / (float)(n);
        j = 2;
        for (m = 0, D1 = 1, D2 = (n - m + D1) / D1; D2 > 0; D2--, m += D1) {
            k[m][n] = (float)((((n - 1) * (n - 1)) - (m * m))) /
                      (float)(((2 * n - 1) * (2 * n - 3)));
            if (m > 0) {
                flnmj = (float)(((n - m + 1) * j)) / (float)((n + m));
                snorm[n + 13 * m] = snorm[n + 13 * (m - 1)] * sqrt(flnmj);
                j = 1;
                c[n][m - 1] = snorm[n + 13 * m] * c[n][m - 1];
                cd[n][m - 1] = snorm[n + 13 * m] * cd[n][m - 1];
            }
            c[m][n] = snorm[n + m * 13] * c[m][n];
            cd[m][n] = snorm[n + m * 13] * cd[m][n];
        }
        fn[n] = (float)((n + 1));
        fm[n] = (float)(n);
    }
    k[1][1] = 0.0;
    otime = oalt = olat = olon = -1000.0;
    return;
}

void time_adjust_gauss_coeff(float time, float otime, float tc[13][13],
                             int m, int n, float c[13][13], float dt,
                             float cd[13][13]) {
    /*
     TIME ADJUST THE GAUSS COEFFICIENTS
     */
    if (time != otime) {
        tc[m][n] = c[m][n] + dt * cd[m][n];
        if (m != 0) tc[n][m - 1] = c[n][m - 1] + dt * cd[n][m - 1];
    }
}

float accumulate_spherical_harmonic_expansions(
    float par, float ar, int n, int m, float tc[13][13], float cp[13],
    float sp[13], float dp[13][13], float fm[13], float fn[13], float *p,
    float &bt, float &bp, float &br) {
    /*
     ACCUMULATE TERMS OF THE SPHERICAL HARMONIC EXPANSIONS
     */
    par = ar * *(p + n + m * 13);
    float temp1;
    float temp2;

    if (m == 0) {
        temp1 = tc[m][n] * cp[m];
        temp2 = tc[m][n] * sp[m];
    } else {
        temp1 = tc[m][n] * cp[m] + tc[n][m - 1] * sp[m];
        temp2 = tc[m][n] * sp[m] - tc[n][m - 1] * cp[m];
    }
    bt -= ar * temp1 * dp[m][n];
    bp += fm[m] * temp2 * par;
    br += fn[n] * temp1 * par;
    return par;
}

float geodetic_to_spherical_coords(
    float alt, float oalt, float glat, float olat, float q, float a2,
    float c2, float srlat2, float q1, float q2, float b2, float ct,
    float srlat, float crlat2, float r2, float a4, float c4, float d,
    float crlat, float &st, float &r, float &ca, float &sa) {
    /* CONVERT FROM GEODETIC COORDS. TO SPHERICAL COORDS. */
    if (alt != oalt || glat != olat) {
        q = sqrt(a2 - c2 * srlat2);
        q1 = alt * q;
        q2 = ((q1 + a2) / (q1 + b2)) * ((q1 + a2) / (q1 + b2));
        ct = srlat / sqrt(q2 * crlat2 + srlat2);
        st = sqrt(1.0 - (ct * ct));
        r2 = (alt * alt) + 2.0 * q1 + (a4 - c4 * srlat2) / (q * q);
        r = sqrt(r2);
        d = sqrt(a2 * crlat2 + b2 * srlat2);
        ca = (alt + d) / r;
        sa = c2 * crlat * srlat / (r * d);
    }
    return ct;
}

static void E0000(int IENTRY, int *maxdeg, float alt, float glat, float glon,
                  float time, float *dec, float *dip, float *ti,
                  float *gv) {
    static int maxord, n, m, j, D1, D2, D3, D4;
    static float c[13][13], cd[13][13], tc[13][13], dp[13][13], snorm[169],
        sp[13], cp[13], fn[13], fm[13], pp[13], k[13][13], pi, dtr, a, b, re,
        a2, b2, c2, a4, b4, c4, gnm, hnm, dgnm, dhnm, flnmj, otime, oalt, olat,
        olon, dt, rlon, rlat, srlat, crlat, srlat2, crlat2, q, q1, q2, ct, st,
        r2, r, d, ca, sa, aor, ar, br, bt, bp, bpp, par, temp1, temp2, parp, bx,
        by, bz, bh;
    float epoch = 2015.0;
    static float *p = snorm;

    switch (IENTRY) {
        case 0:
            goto GEOMAG;
        case 1:
            goto GEOMG1;
    }

GEOMAG:

    /* INITIALIZE CONSTANTS */
    maxord = *maxdeg;
    sp[0] = 0.0;
    cp[0] = *p = pp[0] = 1.0;
    dp[0][0] = 0.0;
    a = 6378.137;
    b = 6356.7523142;
    re = 6371.2;
    a2 = a * a;
    b2 = b * b;
    c2 = a2 - b2;
    a4 = a2 * a2;
    b4 = b2 * b2;
    c4 = a4 - b4;

    /* READ WORLD MAGNETIC MODEL SPHERICAL HARMONIC COEFFICIENTS */
    c[0][0] = 0.0;
    cd[0][0] = 0.0;

    for (int integer = 0; integer < 90; integer++) {
        n = wmmdata[integer][0];
        m = wmmdata[integer][1];
        gnm = wmmdata[integer][2];
        hnm = wmmdata[integer][3];
        dgnm = wmmdata[integer][4];
        dhnm = wmmdata[integer][5];

        if (n > maxord) {
            goto S4;
        }

        if (m <= n) {
            c[m][n] = gnm;
            cd[m][n] = dgnm;
            if (m != 0) {
                c[n][m - 1] = hnm;
                cd[n][m - 1] = dhnm;
            }
        }
        cd[0][0] = 0;
    }

    /* CONVERT SCHMIDT NORMALIZED GAUSS COEFFICIENTS TO UNNORMALIZED */
S4:

    schmidt_norm_to_unnorm(snorm, fm, maxord, j, D1, D2, k, flnmj, c, cd, fn, m,
                           otime, oalt, olat, olon);
    return;

    /*************************************************************************/

GEOMG1:

    dt = time - epoch;
    pi = 3.14159265359;
    dtr = pi / 180.0;
    rlon = glon * dtr;
    rlat = glat * dtr;
    srlat = sin(rlat);
    crlat = cos(rlat);
    srlat2 = srlat * srlat;
    crlat2 = crlat * crlat;
    sp[1] = sin(rlon);
    cp[1] = cos(rlon);

    /* CONVERT FROM GEODETIC COORDS. TO SPHERICAL COORDS. */
    ct = geodetic_to_spherical_coords(alt, oalt, glat, olat, q, a2, c2, srlat2,
                                      q1, q2, b2, ct, srlat, crlat2, r2, a4, c4,
                                      d, crlat, st, r, ca, sa);
    if (glon != olon) {
        for (m = 2; m <= maxord; m++) {
            sp[m] = sp[1] * cp[m - 1] + cp[1] * sp[m - 1];
            cp[m] = cp[1] * cp[m - 1] - sp[1] * sp[m - 1];
        }
    }
    aor = re / r;
    ar = aor * aor;
    br = bt = bp = bpp = 0.0;
    for (n = 1; n <= maxord; n++) {
        ar = ar * aor;
        for (m = 0, D3 = 1, D4 = (n + m + D3) / D3; D4 > 0; D4--, m += D3) {
            /*
               COMPUTE UNNORMALIZED ASSOCIATED LEGENDRE POLYNOMIALS
               AND DERIVATIVES VIA RECURSION RELATIONS
            */
            if (alt != oalt || glat != olat) {
                if (n == m) {
                    *(p + n + m * 13) = st * *(p + n - 1 + (m - 1) * 13);
                    dp[m][n] = st * dp[m - 1][n - 1] +
                               ct * *(p + n - 1 + (m - 1) * 13);
                    goto S50;
                }
                if (n == 1 && m == 0) {
                    *(p + n + m * 13) = ct * *(p + n - 1 + m * 13);
                    dp[m][n] = ct * dp[m][n - 1] - st * *(p + n - 1 + m * 13);
                    goto S50;
                }
                if (n > 1 && n != m) {
                    if (m > n - 2) *(p + n - 2 + m * 13) = 0.0;
                    if (m > n - 2) dp[m][n - 2] = 0.0;
                    *(p + n + m * 13) = ct * *(p + n - 1 + m * 13) -
                                        k[m][n] * *(p + n - 2 + m * 13);
                    dp[m][n] = ct * dp[m][n - 1] - st * *(p + n - 1 + m * 13) -
                               k[m][n] * dp[m][n - 2];
                }
            }
        S50:
            /*
                TIME ADJUST THE GAUSS COEFFICIENTS
            */
            time_adjust_gauss_coeff(time, otime, tc, m, n, c, dt, cd);
            /*
                ACCUMULATE TERMS OF THE SPHERICAL HARMONIC EXPANSIONS
            */
            par = accumulate_spherical_harmonic_expansions(
                par, ar, n, m, tc, cp, sp, dp, fm, fn, p, bt, bp, br);
            /*
                SPECIAL CASE:  NORTH/SOUTH GEOGRAPHIC POLES
            */
            if (st == 0.0 && m == 1) {
                if (n == 1)
                    pp[n] = pp[n - 1];
                else
                    pp[n] = ct * pp[n - 1] - k[m][n] * pp[n - 2];
                parp = ar * pp[n];
                bpp += (fm[m] * temp2 * parp);
            }
        }
    }
    if (st == 0.0)
        bp = bpp;
    else
        bp /= st;
    /*
        ROTATE MAGNETIC VECTOR COMPONENTS FROM SPHERICAL TO
        GEODETIC COORDINATES
    */
    bx = -bt * ca - br * sa;
    by = bp;
    bz = bt * sa - br * ca;
    /*
        COMPUTE DECLINATION (DEC), INCLINATION (DIP) AND
        TOTAL INTENSITY (TI)
    */
    bh = sqrt((bx * bx) + (by * by));
    *ti = sqrt((bh * bh) + (bz * bz));
    *dec = atan2(by, bx) / dtr;
    *dip = atan2(bz, bh) / dtr;
    /*
        COMPUTE MAGNETIC GRID VARIATION IF THE CURRENT
        GEODETIC POSITION IS IN THE ARCTIC OR ANTARCTIC
        (I.E. GLAT > +55 DEGREES OR GLAT < -55 DEGREES)

        OTHERWISE, SET MAGNETIC GRID VARIATION TO -999.0
    */
    *gv = -999.0;
    if (fabs(glat) >= 55.) {
        if (glat > 0.0 && glon >= 0.0) *gv = *dec - glon;
        if (glat > 0.0 && glon < 0.0) *gv = *dec + fabs(glon);
        if (glat < 0.0 && glon >= 0.0) *gv = *dec + glon;
        if (glat < 0.0 && glon < 0.0) *gv = *dec - fabs(glon);
        if (*gv > +180.0) *gv -= 360.0;
        if (*gv < -180.0) *gv += 360.0;
    }
    otime = time;
    oalt = alt;
    olat = glat;
    olon = glon;
    return;
}

/*************************************************************************/

void geomag(int *maxdeg) {
    E0000(0, maxdeg, 0.0, 0.0, 0.0, 0.0, NULL, NULL, NULL, NULL);
}

/*************************************************************************/

void geomg1(float alt, float glat, float glon, float time, float *dec,
            float *dip, float *ti, float *gv) {
    E0000(1, NULL, alt, glat, glon, time, dec, dip, ti, gv);
}

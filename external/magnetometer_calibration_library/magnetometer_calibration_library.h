/*
Copyright (C) 2013 www.sailboatinstruments.blogspot.com

 Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

Some refactoring by MSP for use onboard the ACRUX1 mission
*/

#ifndef MAGNETOMETER_CALIBRATION_LIBRARY_H_
#define MAGNETOMETER_CALIBRATION_LIBRARY_H_

#include <src/util/matrix.h>

class MagnetometerCalibrationLibrary {
   public:
    static void ComputeMatrixSS(const Matrix &S, Matrix &SS, Matrix &S22a);
    static void InitialiseMatrixC(Matrix &C);
    static void CalculateEigenVector1(const Matrix &SS, const Matrix &C,
                                      Matrix &v1);
    static void CalculateEigenVector2(const Matrix &S22a, const Matrix &v1,
                                      Matrix &v2);
    static void InitialiseEigenVector(const Matrix &SS, const Matrix &S22a,
                                      Matrix &v);
    static double CalculateHMB(const Matrix &B_est, const Matrix &Q,
                               const Matrix &v);
    static void Hessenberg_Form_Elementary(double *A, double *S, uint8_t n);
    static void Hessenberg_Elementary_Transform(double *H, double *S,
                                                const uint8_t perm[],
                                                uint8_t n);
    static void Copy_Vector(double *, double *, uint8_t);
    static void QR_Hessenberg_Matrix(double *, double *, double[], double[],
                                     uint8_t, uint8_t);
    static void One_Real_Eigenvalue(double[], double[], double[], uint8_t,
                                    double);
    static void Two_Eigenvalues(double *, double *, double[], double[], uint8_t,
                                uint8_t, double);
    static void Update_Row(double *, double, double, uint8_t, uint8_t);
    static void Update_Column(double *, double, double, uint8_t, uint8_t);
    static void Update_Transformation(double *, double, double, uint8_t,
                                      uint8_t);
    static void Double_QR_Iteration(double *, double *, uint8_t, uint8_t,
                                    uint8_t, double *, uint8_t);
    static void Product_and_Sum_of_Shifts(double *, uint8_t, uint8_t, double *,
                                          double *, double *, uint8_t);
    static uint8_t Two_Consecutive_Small_Subdiagonal(double *, uint8_t, uint8_t,
                                                     uint8_t, double, double);
    static void Double_QR_Step(double *, uint8_t, uint8_t, uint8_t, double,
                               double, double *, uint8_t);
    static void BackSubstitution(double *, double[], double[], uint8_t);
    static void BackSubstitute_Real_Vector(double *, double[], double[],
                                           uint8_t, double, uint8_t);
    static void BackSubstitute_Complex_Vector(double *, double[], double[],
                                              uint8_t, double, uint8_t);
    static void Calculate_Eigenvectors(double *, double *, double[], double[],
                                       uint8_t);
    static void Complex_Division(double, double, double, double, double *,
                                 double *);
    static void Interchange_Rows(double *, uint8_t, uint8_t, uint8_t);
    static void Identity_Matrix(double *, uint8_t);
    static void Interchange_Columns(double *, uint8_t, uint8_t, uint8_t,
                                    uint8_t);
    static const double kHm = 60000;
};

#endif /* MAGNETOMETER_CALIBRATION_LIBRARY_H_ */

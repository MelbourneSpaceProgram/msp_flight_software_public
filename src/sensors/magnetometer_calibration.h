#ifndef MAGNETOMETER_CALIBRATION_H_
#define MAGNETOMETER_CALIBRATION_H_

#include <src/util/matrix.h>

class MagnetometerCalibration
{
public:
    MagnetometerCalibration();
    void ComputeCalibrationParameters();
    void ComputeMatrixS(Matrix magData);
    Matrix GetBiases();
    Matrix GetScaleFactors();
    Matrix GetS();
    Matrix GetSS();

    static const uint8_t data_size = 40;

private:
    void ComputeMatrixSS(Matrix S, Matrix &S22a);
    void InitialiseMatrixC(Matrix &C);
    void CalculateEigenVector1(Matrix &v1, Matrix C);
    void CalculateEigenVector2(Matrix &v2, Matrix v1, Matrix S22a);
    void InitialiseEigenVector(Matrix &v, Matrix S22a);
    void GenerateMatrixB(Matrix v, Matrix Q);
    double CalculateHMB(Matrix Q, Matrix v);
    void GenerateMatrixD(double *eigen_real3, double hmb, Matrix SSSS);
    static void Invert44Matrix(Matrix &A, Matrix B);
    static void Invert33Matrix(Matrix &A, Matrix B);
    static int Hessenberg_Form_Elementary(double *, double *, int);
    static void Hessenberg_Elementary_Transform(double *, double *, int[], int);
    static int Lower_Triangular_Inverse(double *, int);
    static void Copy_Vector(double *, double *, int);
    static int QR_Hessenberg_Matrix(double *, double *, double[], double[], int,
                                    int);
    static void One_Real_Eigenvalue(double[], double[], double[], int, double);
    static void Two_Eigenvalues(double *, double *, double[], double[], int,
                                int, double);
    static void Update_Row(double *, double, double, int, int);
    static void Update_Column(double *, double, double, int, int);
    static void Update_Transformation(double *, double, double, int, int);
    static void Double_QR_Iteration(double *, double *, int, int, int, double *,
                                    int);
    static void Product_and_Sum_of_Shifts(double *, int, int, double *,
                                          double *, double *, int);
    static int Two_Consecutive_Small_Subdiagonal(double *, int, int, int,
                                                 double, double);
    static void Double_QR_Step(double *, int, int, int, double, double,
                               double *, int);
    static void BackSubstitution(double *, double[], double[], int);
    static void BackSubstitute_Real_Vector(double *, double[], double[], int,
                                           double, int);
    static void BackSubstitute_Complex_Vector(double *, double[], double[], int,
                                              double, int);
    static void Calculate_Eigenvectors(double *, double *, double[], double[],
                                       int);
    static void Complex_Division(double, double, double, double, double *,
                                 double *);
    static void Interchange_Rows(double *, int, int, int);
    static void Identity_Matrix(double *, int);
    static void Interchange_Columns(double *, int, int, int, int);

    double S_data[10][10];
    double SS_data[6][6];
    double B_est_data[3][1];
    double D_est_data[3][3];
    Matrix B_est;
    Matrix D_est;
    Matrix S;
    Matrix SS;
};

#endif /* MAGNETOMETER_CALIBRATION_H_ */

#ifndef MAGNETOMETER_CALIBRATION_H_
#define MAGNETOMETER_CALIBRATION_H_

#include <src/util/matrix.h>

class MagnetometerCalibration {
   public:
    MagnetometerCalibration();
    void ComputeCalibrationParameters();
    void ComputeMatrixS(Matrix<float> &magData);
    Matrix<float> GetBiases();
    Matrix<float> GetScaleFactors();
    Matrix<double> GetS();

    static const uint8_t data_size = 40;

   private:
    void ComputeMatrixSS(Matrix<double> &SS, Matrix<double> &S,
                         Matrix<double> &S22a);
    void InitialiseMatrixC(Matrix<double> &C);
    void CalculateEigenVector1(Matrix<double> &v1, Matrix<double> &SS,
                               Matrix<double> &C);
    void CalculateEigenVector2(Matrix<double> &v2, Matrix<double> &v1,
                               Matrix<double> &S22a);
    void InitialiseEigenVector(Matrix<double> &v, Matrix<double> &SS,
                               Matrix<double> &S22a);
    void GenerateMatrixB(Matrix<float> &v, Matrix<float> &Q);
    float CalculateHMB(Matrix<float> &Q, Matrix<float> &v);
    void GenerateMatrixD(float *eigen_real3, float hmb, Matrix<float> &SSSS);
    static void Invert44Matrix(Matrix<double> &A, Matrix<double> &B);
    static void Invert33Matrix(Matrix<float> &A, Matrix<float> &B);
    static int Hessenberg_Form_Elementary(double *, double *, int);
    static void Hessenberg_Elementary_Transform(float *, float *, int[], int);
    static void Copy_Vector(float *, float *, int);
    static int QR_Hessenberg_Matrix(float *, float *, float[], float[], int,
                                    int);
    static void One_Real_Eigenvalue(float[], float[], float[], int, float);
    static void Two_Eigenvalues(float *, float *, float[], float[], int, int,
                                float);
    static void Update_Row(float *, float, float, int, int);
    static void Update_Column(float *, float, float, int, int);
    static void Update_Transformation(float *, float, float, int, int);
    static void float_QR_Iteration(float *, float *, int, int, int, float *,
                                   int);
    static void Product_and_Sum_of_Shifts(float *, int, int, float *, float *,
                                          float *, int);
    static int Two_Consecutive_Small_Subdiagonal(float *, int, int, int, float,
                                                 float);
    static void float_QR_Step(float *, int, int, int, float, float, float *,
                              int);
    static void BackSubstitution(float *, float[], float[], int);
    static void BackSubstitute_Real_Vector(float *, float[], float[], int,
                                           float, int);
    static void BackSubstitute_Complex_Vector(float *, float[], float[], int,
                                              float, int);
    static void Calculate_Eigenvectors(float *, float *, float[], float[], int);
    static void Complex_Division(float, float, float, float, float *, float *);
    static void Interchange_Rows(float *, int, int, int);
    static void Identity_Matrix(float *, int);
    static void Interchange_Columns(float *, int, int, int, int);

    double S_data[10][10];
    float B_est_data[3][1];
    float D_est_data[3][3];
    Matrix<float> B_est;
    Matrix<float> D_est;
    Matrix<double> S;
};

#endif /* MAGNETOMETER_CALIBRATION_H_ */

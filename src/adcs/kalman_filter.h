#ifndef SRC_ADCS_KALMAN_FILTER_H_
#define SRC_ADCS_KALMAN_FILTER_H_

#include <src/util/matrix.h>

class KalmanFilter {
   public:
    KalmanFilter(uint16_t sample_time_millis, Matrix &r1, Matrix &r2,
                 Matrix &Q0, Matrix &R0, Matrix &P0, Matrix &q0);
    void Predict(const Matrix &omega);
    void Update(const Matrix &y);

    Matrix ref1;
    Matrix ref2;
    Matrix Q;
    Matrix R;
    Matrix P;
    Matrix q_estimate;

   private:
    void ComputeF(Matrix &F_out, const Matrix &omega);
    void ComputeXi(Matrix &Xi_out, const Matrix &q);
    void Computeh(Matrix &h_out, const Matrix &q);
    void ComputeH(Matrix &H_out, const Matrix &q);
    void ComputeK(Matrix &K_out, const Matrix &H);
    void Computedx(Matrix dx_out, const Matrix &K, const Matrix &y);

    uint16_t sample_time_millis;
    double ref1_data[3][1];
    double ref2_data[3][1];
    double Q_data[3][3];
    double R_data[6][6];
    double P_data[3][3];
    double q_estimate_data[4][1];
};

#endif  // SRC_ADCS_KALMAN_FILTER_H_

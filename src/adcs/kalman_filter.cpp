#include <math.h>
#include <src/adcs/kalman_filter.h>

KalmanFilter::KalmanFilter(uint16_t sample_time_millis, Matrix &r1, Matrix &r2,
                           Matrix &Q0, Matrix &R0, Matrix &P0, Matrix &q0)
    : ref1(r1, ref1_data),
      ref2(r2, ref2_data),
      Q(Q0, Q_data),
      R(R0, R_data),
      P(P0, P_data),
      q_estimate(q0, q_estimate_data),
      sample_time_millis(sample_time_millis) {}

void KalmanFilter::Predict(const Matrix &omega) {
    // Propagation for quaternion q
    double q_delta_data[4][1];
    Matrix q_delta(q_delta_data);

    double Xi_data[4][3];
    Matrix Xi(Xi_data);

    ComputeXi(Xi, q_estimate);
    q_delta.Multiply(Xi, omega);
    q_delta.MultiplyScalar(q_delta, 0.5 * static_cast<double>(sample_time_millis)/1000);

    q_estimate.Add(q_estimate, q_delta);

    // Propagation for error covariance P
    double F_data[3][3];
    Matrix F(F_data);
    double F_transpose_data[3][3];
    Matrix F_transpose(F_transpose_data);

    ComputeF(F, omega);
    F_transpose.Transpose(F);

    double FP_data[3][3];
    Matrix FP(FP_data);
    double PF_transpose_data[3][3];
    Matrix PF_transpose(PF_transpose_data);
    double FP_PF_transpose_Q_deltat_data[3][3];
    Matrix FP_PF_transpose_Q_deltat(FP_PF_transpose_Q_deltat_data);

    FP.Multiply(F, P);
    PF_transpose.Multiply(P, F_transpose);
    FP_PF_transpose_Q_deltat.Add(FP, PF_transpose);
    FP_PF_transpose_Q_deltat.Add(FP_PF_transpose_Q_deltat, Q);
    FP_PF_transpose_Q_deltat.MultiplyScalar(FP_PF_transpose_Q_deltat,
                                            static_cast<double>(sample_time_millis)/1000);
    P.Add(P, FP_PF_transpose_Q_deltat);
}

void KalmanFilter::Update(const Matrix &y) {
    double H_data[6][3];
    Matrix H(H_data);
    ComputeH(H, q_estimate);

    double K_data[3][6];
    Matrix K(K_data);
    ComputeK(K, H);

    double dx_data[3][1];
    Matrix dx(dx_data);
    Computedx(dx, K, y);

    // Quaternion update
    double q_delta_data[4][1];
    Matrix q_delta(q_delta_data);
    double Xi_data[4][3];
    Matrix Xi(Xi_data);

    ComputeXi(Xi, q_estimate);

    q_delta.Multiply(Xi, dx);
    q_delta.MultiplyScalar(q_delta, 0.5);

    q_estimate.Add(q_estimate, q_delta);

    q_estimate.QuaternionNormalise(q_estimate);

    // Error covariance update
    double I_minus_KH_data[3][3];
    Matrix I_minus_KH(I_minus_KH_data);
    double identity_33_data[3][3];
    Matrix identity_33(identity_33_data);

    identity_33.Identity();

    I_minus_KH.Multiply(K, H);
    I_minus_KH.Subtract(identity_33, I_minus_KH);

    double I_minus_KH_P_data[3][3];
    Matrix I_minus_KH_P(I_minus_KH_P_data);

    I_minus_KH_P.Multiply(I_minus_KH, P);
    P.CopyInto(0, 0, I_minus_KH_P);
}

void KalmanFilter::ComputeXi(Matrix &Xi_out, const Matrix &q) {
    Xi_out.Set(0, 0, q.Get(3, 0));
    Xi_out.Set(0, 1, -q.Get(2, 0));
    Xi_out.Set(0, 2, q.Get(1, 0));
    Xi_out.Set(1, 0, q.Get(2, 0));
    Xi_out.Set(1, 1, q.Get(3, 0));
    Xi_out.Set(1, 2, -q.Get(0, 0));
    Xi_out.Set(2, 0, -q.Get(1, 0));
    Xi_out.Set(2, 1, q.Get(0, 0));
    Xi_out.Set(2, 2, q.Get(3, 0));
    Xi_out.Set(3, 0, -q.Get(0, 0));
    Xi_out.Set(3, 1, -q.Get(1, 0));
    Xi_out.Set(3, 2, -q.Get(2, 0));
}

/* Compute the output function */
void KalmanFilter::Computeh(Matrix &h_out, const Matrix &q) {
    double rotation_matrix_transpose_data[3][3];
    Matrix rotation_matrix_transpose(rotation_matrix_transpose_data);
    double rotation_matrix_data[3][3];
    Matrix rotation_matrix(rotation_matrix_data);

    rotation_matrix_transpose.RotationMatrixFromQuaternion(q);
    rotation_matrix.Transpose(rotation_matrix_transpose);

    double h1_data[3][1];
    Matrix h1(h1_data);
    double h2_data[3][1];
    Matrix h2(h2_data);

    h1.Multiply(rotation_matrix, ref1);
    h2.Multiply(rotation_matrix, ref2);

    h_out.ConcatenateVertically(h1, h2);
}

/* Compute the state Jacobian */
void KalmanFilter::ComputeF(Matrix &F_out, const Matrix &omega) {
    F_out.SkewSymmetricFill(omega);
}

/* Compute the output Jacobian */
void KalmanFilter::ComputeH(Matrix &H_out, const Matrix &q) {
    double rotation_matrix_transpose_data[3][3];
    Matrix rotation_matrix_transpose(rotation_matrix_transpose_data);
    double rotation_matrix_data[3][3];
    Matrix rotation_matrix(rotation_matrix_data);

    rotation_matrix_transpose.RotationMatrixFromQuaternion(q);
    rotation_matrix.Transpose(rotation_matrix_transpose);

    double rotated_ref1_data[3][1];
    Matrix rotated_ref1(rotated_ref1_data);
    double rotated_ref2_data[3][1];
    Matrix rotated_ref2(rotated_ref2_data);

    rotated_ref1.Multiply(rotation_matrix, ref1);
    rotated_ref2.Multiply(rotation_matrix, ref2);

    double H1_data[3][3];
    Matrix H1(H1_data);
    double H2_data[3][3];
    Matrix H2(H2_data);

    H1.SkewSymmetricFill(rotated_ref1);
    H2.SkewSymmetricFill(rotated_ref2);

    H_out.ConcatenateVertically(H1, H2);
}

/* Compute the Kalman gain */
void KalmanFilter::ComputeK(Matrix &K_out, const Matrix &H) {
    double H_transpose_data[3][6];
    Matrix H_transpose(H_transpose_data);
    double PH_transpose_data[3][6];
    Matrix PH_transpose(PH_transpose_data);
    double HPH_transpose_data[6][6];
    Matrix HPH_transpose(HPH_transpose_data);

    H_transpose.Transpose(H);
    PH_transpose.Multiply(P, H_transpose);
    HPH_transpose.Multiply(H, PH_transpose);

    double HPH_transpose_plus_R_data[6][6];
    Matrix HPH_transpose_plus_R(HPH_transpose_plus_R_data);

    HPH_transpose_plus_R.Add(HPH_transpose, R);

    // Invert (H*P*H_tranpose + R)
    double augmented_data[6][12];
    Matrix augmented(augmented_data);
    augmented.CopyInto(0, 0, HPH_transpose_plus_R);

    double identity_66_data[6][6];
    Matrix identity_66(identity_66_data);
    identity_66.Identity();
    augmented.CopyInto(0, 6, identity_66);

    augmented.RowReduce();

    // Put it all together
    double HPH_transpose_plus_R_inv_data[6][6];
    Matrix HPH_transpose_plus_R_inv(HPH_transpose_plus_R_inv_data);
    HPH_transpose_plus_R_inv.Slice(0, 5, 6, 11, augmented);

    K_out.Multiply(PH_transpose, HPH_transpose_plus_R_inv);
}

/* Compute the state update */
void KalmanFilter::Computedx(Matrix dx_out, const Matrix &K, const Matrix &y) {
    double dy_data[6][1];
    Matrix dy(dy_data);
    double h_data[6][1];
    Matrix h(h_data);

    Computeh(h, q_estimate);

    dy.Subtract(y, h);

    dx_out.Multiply(K, dy);
}

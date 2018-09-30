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
    NewStackMatrixMacro(q_delta, 4, 1);

    NewStackMatrixMacro(Xi, 4, 3);

    ComputeXi(Xi, q_estimate);
    q_delta.Multiply(Xi, omega);
    q_delta.MultiplyScalar(
        q_delta, 0.5 * static_cast<double>(sample_time_millis) / 1000);

    q_estimate.Add(q_estimate, q_delta);

    // Propagation for error covariance P
    NewStackMatrixMacro(F, 3, 3);
    NewStackMatrixMacro(F_transpose, 3, 3);

    ComputeF(F, omega);
    F_transpose.Transpose(F);

    NewStackMatrixMacro(FP, 3, 3);
    NewStackMatrixMacro(PF_transpose, 3, 3);
    NewStackMatrixMacro(FP_PF_transpose_Q_deltat, 3, 3);

    FP.Multiply(F, P);
    PF_transpose.Multiply(P, F_transpose);
    FP_PF_transpose_Q_deltat.Add(FP, PF_transpose);
    FP_PF_transpose_Q_deltat.Add(FP_PF_transpose_Q_deltat, Q);
    FP_PF_transpose_Q_deltat.MultiplyScalar(
        FP_PF_transpose_Q_deltat,
        static_cast<double>(sample_time_millis) / 1000);
    P.Add(P, FP_PF_transpose_Q_deltat);
}

void KalmanFilter::Update(const Matrix &y) {
    NewStackMatrixMacro(H, 6, 3);
    ComputeH(H, q_estimate);

    NewStackMatrixMacro(K, 3, 6);
    ComputeK(K, H);

    NewStackMatrixMacro(dx, 3, 1);
    Computedx(dx, K, y);

    // Quaternion update
    NewStackMatrixMacro(q_delta, 4, 1);
    NewStackMatrixMacro(Xi, 4, 3);

    ComputeXi(Xi, q_estimate);

    q_delta.Multiply(Xi, dx);
    q_delta.MultiplyScalar(q_delta, 0.5);

    q_estimate.Add(q_estimate, q_delta);

    q_estimate.QuaternionNormalise(q_estimate);

    // Error covariance update
    NewStackMatrixMacro(I_minus_KH, 3, 3);
    NewStackMatrixMacro(identity_33, 3, 3);

    identity_33.Identity();

    I_minus_KH.Multiply(K, H);
    I_minus_KH.Subtract(identity_33, I_minus_KH);

    NewStackMatrixMacro(I_minus_KH_P, 3, 3);

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
    NewStackMatrixMacro(rotation_matrix_transpose, 3, 3);
    NewStackMatrixMacro(rotation_matrix, 3, 3);

    rotation_matrix_transpose.RotationMatrixFromQuaternion(q);
    rotation_matrix.Transpose(rotation_matrix_transpose);

    NewStackMatrixMacro(h1, 3, 1);
    NewStackMatrixMacro(h2, 3, 1);

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
    NewStackMatrixMacro(rotation_matrix_transpose, 3, 3);
    NewStackMatrixMacro(rotation_matrix, 3, 3);

    rotation_matrix_transpose.RotationMatrixFromQuaternion(q);
    rotation_matrix.Transpose(rotation_matrix_transpose);

    NewStackMatrixMacro(rotated_ref1, 3, 1);
    NewStackMatrixMacro(rotated_ref2, 3, 1);

    rotated_ref1.Multiply(rotation_matrix, ref1);
    rotated_ref2.Multiply(rotation_matrix, ref2);

    NewStackMatrixMacro(H1, 3, 3);
    NewStackMatrixMacro(H2, 3, 3);

    H1.SkewSymmetricFill(rotated_ref1);
    H2.SkewSymmetricFill(rotated_ref2);

    H_out.ConcatenateVertically(H1, H2);
}

/* Compute the Kalman gain */
void KalmanFilter::ComputeK(Matrix &K_out, const Matrix &H) {
    NewStackMatrixMacro(H_transpose, 3, 6);
    NewStackMatrixMacro(PH_transpose, 3, 6);
    NewStackMatrixMacro(HPH_transpose, 6, 6);

    H_transpose.Transpose(H);
    PH_transpose.Multiply(P, H_transpose);
    HPH_transpose.Multiply(H, PH_transpose);

    NewStackMatrixMacro(HPH_transpose_plus_R, 6, 6);

    HPH_transpose_plus_R.Add(HPH_transpose, R);

    // Invert (H*P*H_tranpose + R)
    NewStackMatrixMacro(augmented, 6, 12);
    augmented.CopyInto(0, 0, HPH_transpose_plus_R);

    NewStackMatrixMacro(identity_66, 6, 6);
    identity_66.Identity();
    augmented.CopyInto(0, 6, identity_66);

    augmented.RowReduce();

    // Put it all together
    NewStackMatrixMacro(HPH_transpose_plus_R_inv, 6, 6);
    HPH_transpose_plus_R_inv.CopySlice(0, 5, 6, 11, augmented);

    K_out.Multiply(PH_transpose, HPH_transpose_plus_R_inv);
}

/* Compute the state update */
void KalmanFilter::Computedx(Matrix dx_out, const Matrix &K, const Matrix &y) {
    NewStackMatrixMacro(dy, 6, 1);
    NewStackMatrixMacro(h, 6, 1);

    Computeh(h, q_estimate);

    dy.Subtract(y, h);

    dx_out.Multiply(K, dy);
}

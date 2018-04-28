#include <float.h>
#include <math.h>
#include <src/sensors/magnetometer_calibration.h>

void MagnetometerCalibration::ComputeMatrixS(Matrix<float> &magData)
{
    float x, y, z;
    float D_data[10][data_size];
    Matrix<float> D(D_data);
    for (uint8_t i = 0; i < data_size; i++)
    {
        x = magData.Get(i, 0);
        y = magData.Get(i, 1);
        z = magData.Get(i, 2);
        D.Set(0, i, x * x);
        D.Set(1, i, y * y);
        D.Set(2, i, z * z);
        D.Set(3, i, 2.0 * y * z);
        D.Set(4, i, 2.0 * x * z);
        D.Set(5, i, 2.0 * x * y);
        D.Set(6, i, 2.0 * x);
        D.Set(7, i, 2.0 * y);
        D.Set(8, i, 2.0 * z);
        D.Set(9, i, 1.0);
    }
    float Dt_data[data_size][10];
    Matrix<float> Dt(Dt_data);
    Dt.Transpose(D);
    float S_float_data[10][10];
    Matrix<float> S_float(S_float_data);
    S_float.Multiply(D, Dt);
    for (uint8_t i = 0; i < 10; i++) {
        for (uint8_t j = 0; j < 10; j++) {
            S.Set(i, j, (double)S_float.Get(i, j));
        }
    }
}

void MagnetometerCalibration::Invert44Matrix(Matrix<double> &A, Matrix<double> &B)
{
    double augmented_data[4][8];
    Matrix<double> augmented(augmented_data);
    augmented.CopyInto(0, 0, B);

    double identity_data[4][4];
    Matrix<double> identity(identity_data);
    identity.Identity();
    augmented.CopyInto(0, 4, identity);

    augmented.RowReduce();

    A.Slice(0, 3, 4, 7, augmented);
}

void MagnetometerCalibration::Invert33Matrix(Matrix<float> &A, Matrix<float> &B)
{
    float augmented_data[3][6];
    Matrix<float> augmented(augmented_data);
    augmented.CopyInto(0, 0, B);

    float identity_data[3][3];
    Matrix<float> identity(identity_data);
    identity.Identity();
    augmented.CopyInto(0, 3, identity);

    augmented.RowReduce();

    A.Slice(0, 2, 3, 5, augmented);
}

void MagnetometerCalibration::InitialiseMatrixC(Matrix<double> &C)
{
    C.Fill(0.0);
    C.Set(0, 1, 0.5);
    C.Set(0, 2, 0.5);
    C.Set(1, 0, 0.5);
    C.Set(1, 2, 0.5);
    C.Set(2, 0, 0.5);
    C.Set(2, 1, 0.5);
    C.Set(3, 3, -0.25);
    C.Set(4, 4, -0.25);
    C.Set(5, 5, -0.25);
}

void MagnetometerCalibration::ComputeMatrixSS(Matrix<double> &SS,
                                              Matrix<double> &S,
                                              Matrix<double> &S22a)
{
    double S11_data[6][6];
    Matrix<double> S11(S11_data);
    S11.Slice(0, 5, 0, 5, S);
    double S12_data[6][4];
    Matrix<double> S12(S12_data);
    S12.Slice(0, 5, 6, 9, S);
    double S12t_data[4][6];
    Matrix<double> S12t(S12t_data);
    S12t.Slice(6, 9, 0, 5, S);
    double S22_data[4][4];
    Matrix<double> S22(S22_data);
    S22.Slice(6, 9, 6, 9, S);

    double S22_1_data[4][4];
    Matrix<double> S22_1(S22_1_data);
    Invert44Matrix(S22_1, S22);

    // Calculate S22a = S22_1 * S12t   4*6 = 4x4 * 4x6   C = AB
    S22a.Multiply(S22_1, S12t);

    // Then calculate S22b = S12 * S22a      ( 6x6 = 6x4 * 4x6)
    double S22b_data[6][6];
    Matrix<double> S22b(S22b_data);
    S22b.Multiply(S12, S22a);

    double ans = S11.Get(0, 0) - S22b.Get(0, 0);
    // Calculate SS = S11 - S22b
    SS.Subtract(S11, S22b);
}

void MagnetometerCalibration::CalculateEigenVector1(Matrix<double> &v1,
                                                    Matrix<double> &SS,
                                                    Matrix<double> &C)
{
    uint8_t i, j, index;
    double maxVal, norm;
    double E_data[6][6];
    Matrix<double> E(E_data);

    E.Multiply(C, SS);

    double E_array[36];
    for (i = 0; i < 6; i++)
    {
        for (j = 0; j < 6; j++)
        {
            E_array[i * 6 + j] = E.Get(i, j);
        }
    }
    double SSS[36];
    Hessenberg_Form_Elementary(E_array, SSS, 6);

    double eigen_real[6];
    double eigen_imag[6];

    QR_Hessenberg_Matrix(E_array, SSS, eigen_real, eigen_imag, 6, 100);

    index = 0;
    maxVal = eigen_real[0];
    for (i = 1; i < 6; i++)
    {
        if (eigen_real[i] > maxVal)
        {
            maxVal = eigen_real[i];
            index = i;
        }
    }
    for (i = 0; i < 6; i++)
    {
        v1.Set(i, 0, SSS[index + i * 6]);
    }

    // normalize v1
    norm = v1.VectorNorm(v1);
    for (uint8_t i = 0; i < 6; i++)
    {
        v1.Set(i, 0, v1.Get(i, 0) / norm);
    }
    if (v1.Get(0, 0) < 0.0)
    {
        for (uint8_t i = 0; i < 6; i++)
        {
            v1.Set(i, 0, -v1.Get(i, 0));
        }
    }
}

void MagnetometerCalibration::InitialiseEigenVector(Matrix<double> &v,
                                                    Matrix<double> &SS,
                                                    Matrix<double> &S22a)
{
    double C_data[6][6];
    double v1_data[6][1];
    double v2_data[4][1];
    Matrix<double> v1(v1_data);
    Matrix<double> v2(v2_data);
    Matrix<double> C(C_data);

    // Create pre-inverted constraint matrix C
    InitialiseMatrixC(C);
    CalculateEigenVector1(v1, SS, C);
    // Calculate v2 = S22a * v1      ( 4x1 = 4x6 * 6x1)
    CalculateEigenVector2(v2, v1, S22a);

    v.Set(0, 0, v1.Get(0, 0));
    v.Set(1, 0, v1.Get(1, 0));
    v.Set(2, 0, v1.Get(2, 0));
    v.Set(3, 0, v1.Get(3, 0));
    v.Set(4, 0, v1.Get(4, 0));
    v.Set(5, 0, v1.Get(5, 0));
    v.Set(6, 0, -v2.Get(0, 0));
    v.Set(7, 0, -v2.Get(1, 0));
    v.Set(8, 0, -v2.Get(2, 0));
    v.Set(9, 0, -v2.Get(3, 0));
}

void MagnetometerCalibration::GenerateMatrixB(Matrix<float> &v,
                                              Matrix<float> &Q)
{
    float U_data[3][1];
    Matrix<float> U(U_data);

    U.Set(0, 0, v.Get(6, 0));
    U.Set(1, 0, v.Get(7, 0));
    U.Set(2, 0, v.Get(8, 0));

    float Q_data[3][3];
    Matrix<float> Q_1(Q_data);
    Invert33Matrix(Q_1, Q);

    // Calculate B = Q-1 * U   ( 3x1 = 3x3 * 3x1)
    B_est.Multiply(Q_1, U);
    B_est.Set(0, 0, -B_est.Get(0, 0));
    B_est.Set(1, 0, -B_est.Get(1, 0));
    B_est.Set(2, 0, -B_est.Get(2, 0));
}

float MagnetometerCalibration::CalculateHMB(Matrix<float> &Q, Matrix<float> &v)
{
    float Bt_data[1][3];
    Matrix<float> Bt(Bt_data);
    Bt.Transpose(B_est);

    // First calculate QB = Q * B   ( 3x1 = 3x3 * 3x1)
    float QB_data[3][1];
    Matrix<float> QB(QB_data);
    QB.Multiply(Q, B_est);

    // Then calculate btqb = BT * QB    ( 1x1 = 1x3 * 3x1)
    float btqb_data[1][1];
    Matrix<float> btqb(btqb_data);
    btqb.Multiply(Bt, QB);

    // Calculate hmb = sqrt(btqb - J).
    float J = v.Get(9, 0);
    return sqrt(btqb.Get(0, 0) - J);
}

void MagnetometerCalibration::GenerateMatrixD(float *eigen_real3, float hmb,
                                              Matrix<float> &SSSS)
{
    uint8_t i, j;
    float hm;
    float Dz_data[3][3];
    Matrix<float> Dz(Dz_data);
    Dz.Fill(0);
    Dz.Set(0, 0, sqrt(eigen_real3[0]));
    Dz.Set(1, 1, sqrt(eigen_real3[1]));
    Dz.Set(2, 2, sqrt(eigen_real3[2]));

    float vdz_data[3][3];
    Matrix<float> vdz(vdz_data);
    vdz.Multiply(SSSS, Dz);

    float SSSS_transpose_data[3][3];
    Matrix<float> SSSS_transpose(SSSS_transpose_data);
    SSSS_transpose.Transpose(SSSS);

    float SQ_data[3][3];
    Matrix<float> SQ(SQ_data);
    SQ.Multiply(vdz, SSSS_transpose);

    hm = 60000;
    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 3; j++)
        {
            D_est.Set(i, j, SQ.Get(i, j) * hm / hmb);
        }
    }
}

void MagnetometerCalibration::CalculateEigenVector2(Matrix<float> &v2,
                                                    Matrix<float> &v1,
                                                    Matrix<float> &S22a)
{
    v2.Multiply(S22a, v1);
}

MagnetometerCalibration::MagnetometerCalibration() :
        B_est(B_est_data), D_est(D_est_data), S(S_data)
{
}

void MagnetometerCalibration::ComputeCalibrationParameters()
{
    uint8_t i, j;
    float hmb, norm1, norm2, norm3;
    double S22a_data[4][6];
    float v_float_data[10][1];
    double v_data[10][1];
    float Q_data[3][3];
    double SS_data[6][6];
    float SSSS_data[3][3];
    Matrix<double> S22a(S22a_data);
    Matrix<float> v_float(v_float_data);
    Matrix<double> v(v_data);
    Matrix<float> Q(Q_data);
    Matrix<double> SS(SS_data);
    Matrix<float> SSSS(SSSS_data);

    ComputeMatrixSS(SS, S, S22a);

    InitialiseEigenVector(v, SS, S22a);
    for (i = 0; i < 10; i++) {
        v_float.Set(i, 0, (float)v.Get(i, 0));
    }
    Q.Set(0, 0, v_float.Get(0, 0));
    Q.Set(0, 1, v_float.Get(5, 0));
    Q.Set(0, 2, v_float.Get(4, 0));
    Q.Set(1, 0, v_float.Get(5, 0));
    Q.Set(1, 1, v_float.Get(1, 0));
    Q.Set(1, 2, v_float.Get(3, 0));
    Q.Set(2, 0, v_float.Get(4, 0));
    Q.Set(2, 1, v_float.Get(3, 0));
    Q.Set(2, 2, v_float.Get(2, 0));

    float Q_array[9];
    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 3; j++)
        {
            Q_array[i * 3 + j] = Q.Get(i, j);
        }
    }

    GenerateMatrixB(v_float, Q);

    hmb = CalculateHMB(Q, v_float);

    // Calculate SQ, the square root of matrix Q
    float SSSS_array[9];
    Hessenberg_Form_Elementary(Q_array, SSSS_array, 3);

    float eigen_real3[3];
    float eigen_imag3[3];
    QR_Hessenberg_Matrix(Q_array, SSSS_array, eigen_real3, eigen_imag3, 3, 100);

    for (i = 0; i < 3; i++)
        for (j = 0; j < 3; j++)
            SSSS.Set(i, j, SSSS_array[i * 3 + j]);

    // normalize eigenvectors
    norm1 = sqrt(
            SSSS.Get(0, 0) * SSSS.Get(0, 0) + SSSS.Get(1, 0) * SSSS.Get(1, 0)
                    + SSSS.Get(2, 0) * SSSS.Get(2, 0));
    SSSS.Set(0, 0, SSSS.Get(0, 0) / norm1);
    SSSS.Set(1, 0, SSSS.Get(1, 0) / norm1);
    SSSS.Set(2, 0, SSSS.Get(2, 0) / norm1);
    norm2 = sqrt(
            SSSS.Get(0, 1) * SSSS.Get(0, 1) + SSSS.Get(1, 1) * SSSS.Get(1, 1)
                    + SSSS.Get(2, 1) * SSSS.Get(2, 1));
    SSSS.Set(0, 1, SSSS.Get(0, 1) / norm2);
    SSSS.Set(1, 1, SSSS.Get(1, 1) / norm2);
    SSSS.Set(2, 1, SSSS.Get(2, 1) / norm2);
    norm3 = sqrt(
            SSSS.Get(0, 2) * SSSS.Get(0, 2) + SSSS.Get(1, 2) * SSSS.Get(1, 2)
                    + SSSS.Get(2, 2) * SSSS.Get(2, 2));
    SSSS.Set(0, 2, SSSS.Get(0, 2) / norm3);
    SSSS.Set(1, 2, SSSS.Get(1, 2) / norm3);
    SSSS.Set(2, 2, SSSS.Get(2, 2) / norm3);

    GenerateMatrixD(eigen_real3, hmb, SSSS);
}

Matrix<float> MagnetometerCalibration::GetBiases()
{
    return B_est;
}

Matrix<float> MagnetometerCalibration::GetScaleFactors()
{
    return D_est;
}

Matrix<double> MagnetometerCalibration::GetS()
{
    return S;
}

void MagnetometerCalibration::Hessenberg_Elementary_Transform(float *H,
                                                              float *S,
                                                              int perm[], int n)
{
    int i, j;
    float *pS, *pH;
    //   float x;

    Identity_Matrix(S, n);
    for (i = n - 2; i >= 1; i--)
    {
        pH = H + n * (i + 1);
        pS = S + n * (i + 1);
        for (j = i + 1; j < n; pH += n, pS += n, j++)
        {
            *(pS + i) = *(pH + i - 1);
            *(pH + i - 1) = 0.0;
        }
        if (perm[i] != i)
        {
            pS = S + n * i;
            pH = S + n * perm[i];
            for (j = i; j < n; j++)
            {
                *(pS + j) = *(pH + j);
                *(pH + j) = 0.0;
            }
            *(pH + i) = 1.0;
        }
    }
}

int MagnetometerCalibration::Hessenberg_Form_Elementary(double *A, double *S,
                                                        int n)
{
    int i, j, col, row;
    // int k;
    int *perm;
    float *p_row, *pS_row;
    float max;
    float s;
    float *pA, *pB, *pC, *pS;

    // n x n matrices for which n <= 2 are already in Hessenberg form

    if (n <= 1)
    {
        *S = 1.0;
        return 0;
    }
    if (n == 2)
    {
        *S++ = 1.0;
        *S++ = 0.0;
        *S++ = 1.0;
        *S = 0.0;
        return 0;
    }

    // Allocate working memory

    // perm = (int *)malloc(n * sizeof(int));
    perm = new int[n];
    if (perm == NULL)
        return -1;  // not enough memory

    // For each column use Elementary transformations
    //   to zero the entries below the subdiagonal.

    p_row = A + n;
    pS_row = S + n;
    for (col = 0; col < (n - 2); p_row += n, pS_row += n, col++)
    {
        // Find the row in column "col" with maximum magnitude where
        // row >= col + 1.

        row = col + 1;
        perm[row] = row;
        for (pA = p_row + col, max = 0.0, i = row; i < n; pA += n, i++)
            if (fabs(*pA) > max)
            {
                perm[row] = i;
                max = fabs(*pA);
            }

        // If perm[row] != row, then interchange row "row" and row
        // perm[row] and interchange column "row" and column perm[row].

        if (perm[row] != row)
        {
            Interchange_Rows(A, row, perm[row], n);
            Interchange_Columns(A, row, perm[row], n, n);
        }

        // Zero out the components lying below the subdiagonal.

        pA = p_row + n;
        pS = pS_row + n;
        for (i = col + 2; i < n; pA += n, pS += n, i++)
        {
            s = *(pA + col) / *(p_row + col);
            for (j = 0; j < n; j++)
                *(pA + j) -= *(p_row + j) * s;
            *(pS + col) = s;
            for (j = 0, pB = A + col + 1, pC = A + i; j < n;
                    pB += n, pC += n, j++)
                *pB += s * *pC;
        }
    }
    pA = A + n + n;
    pS = S + n + n;
    for (i = 2; i < n; pA += n, pS += n, i++)
        Copy_Vector(pA, pS, i - 1);

    Hessenberg_Elementary_Transform(A, S, perm, n);

    // free(perm);
    return 0;
}

void MagnetometerCalibration::Interchange_Rows(float *A, int row1, int row2,
                                               int ncols)
{
    int i;
    float *pA1, *pA2;
    float temp;

    pA1 = A + row1 * ncols;
    pA2 = A + row2 * ncols;
    for (i = 0; i < ncols; i++)
    {
        temp = *pA1;
        *pA1++ = *pA2;
        *pA2++ = temp;
    }
}

void MagnetometerCalibration::Interchange_Columns(float *A, int col1, int col2,
                                                  int nrows, int ncols)
{
    int i;
    float *pA1, *pA2;
    float temp;

    pA1 = A + col1;
    pA2 = A + col2;
    for (i = 0; i < nrows; pA1 += ncols, pA2 += ncols, i++)
    {
        temp = *pA1;
        *pA1 = *pA2;
        *pA2 = temp;
    }
}

void MagnetometerCalibration::Copy_Vector(float *d, float *s, int n)
{
    // memcpy(d, s, sizeof(float) * n);
    for (uint8_t i = 0; i < n; i++)
    {
        d[i] = s[i];
    }
}

int MagnetometerCalibration::QR_Hessenberg_Matrix(double *H, double *S,
                                                  double eigen_real[],
                                                  double eigen_imag[], int n,
                                                  int max_iteration_count)
{
    int i;
    int row;
    int iteration;
    int found_eigenvalue;
    double shift = 0.0;
    double *pH;

    for (row = n - 1; row >= 0; row--)
    {
        found_eigenvalue = 0;
        for (iteration = 1; iteration <= max_iteration_count; iteration++)
        {
            // Search for small subdiagonal element

            for (i = row, pH = H + row * n; i > 0; i--, pH -= n)
                if (fabs(*(pH + i - 1)) <=
                DBL_EPSILON * (fabs(*(pH - n + i - 1)) + fabs(*(pH + i))))
                    break;

            // If the subdiagonal element on row "row" is small, then
            // that row element is an eigenvalue.  If the subdiagonal
            // element on row "row-1" is small, then the eigenvalues
            // of the 2x2 diagonal block consisting rows "row-1" and
            // "row" are eigenvalues.  Otherwise perform a float QR
            // iteration.

            switch (row - i)
            {
            case 0:  // One real eigenvalue
                One_Real_Eigenvalue(pH, eigen_real, eigen_imag, i, shift);
                found_eigenvalue = 1;
                break;
            case 1:  // Either two real eigenvalues or a complex pair
                row--;
                Two_Eigenvalues(H, S, eigen_real, eigen_imag, n, row, shift);
                found_eigenvalue = 1;
                break;
            default:
                float_QR_Iteration(H, S, i, row, n, &shift, iteration);
            }
            if (found_eigenvalue)
                break;
        }
        if (iteration > max_iteration_count)
            return -1;
    }

    BackSubstitution(H, eigen_real, eigen_imag, n);
    Calculate_Eigenvectors(H, S, eigen_real, eigen_imag, n);

    return 0;
}

void MagnetometerCalibration::One_Real_Eigenvalue(float Hrow[],
                                                  float eigen_real[],
                                                  float eigen_imag[], int row,
                                                  float shift)
{
    Hrow[row] += shift;
    eigen_real[row] = Hrow[row];
    eigen_imag[row] = 0.0;
}

void MagnetometerCalibration::Two_Eigenvalues(float *H, float *S,
                                              float eigen_real[],
                                              float eigen_imag[], int n,
                                              int row, float shift)
{
    float p, q, x, discriminant, r;
    float cos, sin;
    float *Hrow = H + n * row;
    float *Hnextrow = Hrow + n;
    int nextrow = row + 1;

    p = 0.5 * (Hrow[row] - Hnextrow[nextrow]);
    x = Hrow[nextrow] * Hnextrow[row];
    discriminant = p * p + x;
    Hrow[row] += shift;
    Hnextrow[nextrow] += shift;
    if (discriminant > 0.0)
    {  // pair of real roots
        q = sqrt(discriminant);
        if (p < 0.0)
            q = p - q;
        else
            q += p;
        eigen_real[row] = Hnextrow[nextrow] + q;
        eigen_real[nextrow] = Hnextrow[nextrow] - x / q;
        eigen_imag[row] = 0.0;
        eigen_imag[nextrow] = 0.0;
        r = sqrt(Hnextrow[row] * Hnextrow[row] + q * q);
        sin = Hnextrow[row] / r;
        cos = q / r;
        Update_Row(Hrow, cos, sin, n, row);
        Update_Column(H, cos, sin, n, row);
        Update_Transformation(S, cos, sin, n, row);
    }
    else
    {  // pair of complex roots
        eigen_real[nextrow] = eigen_real[row] = Hnextrow[nextrow] + p;
        eigen_imag[row] = sqrt(fabs(discriminant));
        eigen_imag[nextrow] = -eigen_imag[row];
    }
}

void MagnetometerCalibration::Update_Row(float *Hrow, float cos, float sin,
                                         int n, int row)
{
    float x;
    float *Hnextrow = Hrow + n;
    int i;

    for (i = row; i < n; i++)
    {
        x = Hrow[i];
        Hrow[i] = cos * x + sin * Hnextrow[i];
        Hnextrow[i] = cos * Hnextrow[i] - sin * x;
    }
}

void MagnetometerCalibration::Update_Column(float *H, float cos, float sin,
                                            int n, int col)
{
    float x;
    int i;
    int next_col = col + 1;

    for (i = 0; i <= next_col; i++, H += n)
    {
        x = H[col];
        H[col] = cos * x + sin * H[next_col];
        H[next_col] = cos * H[next_col] - sin * x;
    }
}

void MagnetometerCalibration::Update_Transformation(float *S, float cos,
                                                    float sin, int n, int k)
{
    float x;
    int i;
    int k1 = k + 1;

    for (i = 0; i < n; i++, S += n)
    {
        x = S[k];
        S[k] = cos * x + sin * S[k1];
        S[k1] = cos * S[k1] - sin * x;
    }
}

void MagnetometerCalibration::float_QR_Iteration(float *H, float *S,
                                                 int min_row, int max_row,
                                                 int n, float *shift,
                                                 int iteration)
{
    int k;
    float trace, det;

    Product_and_Sum_of_Shifts(H, n, max_row, shift, &trace, &det, iteration);
    k = Two_Consecutive_Small_Subdiagonal(H, min_row, max_row, n, trace, det);
    float_QR_Step(H, min_row, max_row, k, trace, det, S, n);
}

void MagnetometerCalibration::Product_and_Sum_of_Shifts(float *H, int n,
                                                        int max_row,
                                                        float *shift,
                                                        float *trace,
                                                        float *det,
                                                        int iteration)
{
    float *pH = H + max_row * n;
    float *p_aux;
    int i;
    int min_col = max_row - 1;

    if ((iteration % 10) == 0)
    {
        *shift += pH[max_row];
        for (i = 0, p_aux = H; i <= max_row; p_aux += n, i++)
            p_aux[i] -= pH[max_row];
        p_aux = pH - n;
        *trace = fabs(pH[min_col]) + fabs(p_aux[min_col - 1]);
        *det = *trace * *trace;
        *trace *= 1.5;
    }
    else
    {
        p_aux = pH - n;
        *trace = p_aux[min_col] + pH[max_row];
        *det = p_aux[min_col] * pH[max_row] - p_aux[max_row] * pH[min_col];
    }
}

int MagnetometerCalibration::Two_Consecutive_Small_Subdiagonal(float *H,
                                                               int min_row,
                                                               int max_row,
                                                               int n,
                                                               float trace,
                                                               float det)
{
    float x, y, z, s;
    float *pH;
    int i, k;

    for (k = max_row - 2, pH = H + k * n; k >= min_row; pH -= n, k--)
    {
        x = (pH[k] * (pH[k] - trace) + det) / pH[n + k] + pH[k + 1];
        y = pH[k] + pH[n + k + 1] - trace;
        z = pH[n + n + k + 1];
        s = fabs(x) + fabs(y) + fabs(z);
        x /= s;
        y /= s;
        z /= s;
        if (k == min_row)
            break;
        if ((fabs(pH[k - 1]) * (fabs(y) + fabs(z)))
                <=
                DBL_EPSILON * fabs(x)
                        * (fabs(pH[k - 1 - n]) + fabs(pH[k])
                                + fabs(pH[n + k + 1])))
            break;
    }
    for (i = k + 2, pH = H + i * n; i <= max_row; pH += n, i++)
        pH[i - 2] = 0.0;
    for (i = k + 3, pH = H + i * n; i <= max_row; pH += n, i++)
        pH[i - 3] = 0.0;
    return k;
}

void MagnetometerCalibration::float_QR_Step(float *H, int min_row, int max_row,
                                            int min_col, float trace, float det,
                                            float *S, int n)
{
    float s, x, y, z;
    float a, b, c;
    float *pH;
    float *tH;
    float *pS;
    int i, j, k;
    int last_test_row_col = max_row - 1;

    k = min_col;
    pH = H + min_col * n;
    a = (pH[k] * (pH[k] - trace) + det) / pH[n + k] + pH[k + 1];
    b = pH[k] + pH[n + k + 1] - trace;
    c = pH[n + n + k + 1];
    s = fabs(a) + fabs(b) + fabs(c);
    a /= s;
    b /= s;
    c /= s;

    for (; k <= last_test_row_col; k++, pH += n)
    {
        if (k > min_col)
        {
            c = (k == last_test_row_col) ? 0.0 : pH[n + n + k - 1];
            x = fabs(pH[k - 1]) + fabs(pH[n + k - 1]) + fabs(c);
            if (x == 0.0)
                continue;
            a = pH[k - 1] / x;
            b = pH[n + k - 1] / x;
            c /= x;
        }
        s = sqrt(a * a + b * b + c * c);
        if (a < 0.0)
            s = -s;
        if (k > min_col)
            pH[k - 1] = -s * x;
        else if (min_row != min_col)
            pH[k - 1] = -pH[k - 1];
        a += s;
        x = a / s;
        y = b / s;
        z = c / s;
        b /= a;
        c /= a;

        // Update rows k, k+1, k+2
        for (j = k; j < n; j++)
        {
            a = pH[j] + b * pH[n + j];
            if (k != last_test_row_col)
            {
                a += c * pH[n + n + j];
                pH[n + n + j] -= a * z;
            }
            pH[n + j] -= a * y;
            pH[j] -= a * x;
        }

        // Update column k+1

        j = k + 3;
        if (j > max_row)
            j = max_row;
        for (i = 0, tH = H; i <= j; i++, tH += n)
        {
            a = x * tH[k] + y * tH[k + 1];
            if (k != last_test_row_col)
            {
                a += z * tH[k + 2];
                tH[k + 2] -= a * c;
            }
            tH[k + 1] -= a * b;
            tH[k] -= a;
        }

        // Update transformation matrix

        for (i = 0, pS = S; i < n; pS += n, i++)
        {
            a = x * pS[k] + y * pS[k + 1];
            if (k != last_test_row_col)
            {
                a += z * pS[k + 2];
                pS[k + 2] -= a * c;
            }
            pS[k + 1] -= a * b;
            pS[k] -= a;
        }
    };
}

void MagnetometerCalibration::BackSubstitution(float *H, float eigen_real[],
                                               float eigen_imag[], int n)
{
    float zero_tolerance;
    float *pH;
    int i, j, row;

    // Calculate the zero tolerance

    pH = H;
    zero_tolerance = fabs(pH[0]);
    for (pH += n, i = 1; i < n; pH += n, i++)
        for (j = i - 1; j < n; j++)
            zero_tolerance += fabs(pH[j]);
    zero_tolerance *= DBL_EPSILON;

    // Start Backsubstitution

    for (row = n - 1; row >= 0; row--)
    {
        if (eigen_imag[row] == 0.0)
            BackSubstitute_Real_Vector(H, eigen_real, eigen_imag, row,
                                       zero_tolerance, n);
        else if (eigen_imag[row] < 0.0)
            BackSubstitute_Complex_Vector(H, eigen_real, eigen_imag, row,
                                          zero_tolerance, n);
    }
}

void MagnetometerCalibration::BackSubstitute_Real_Vector(float *H,
                                                         float eigen_real[],
                                                         float eigen_imag[],
                                                         int row,
                                                         float zero_tolerance,
                                                         int n)
{
    float *pH;
    float *pV;
    float x;
    float u[4];
    float v[2];
    int i, j, k;

    k = row;
    pH = H + row * n;
    pH[row] = 1.0;
    for (i = row - 1, pH -= n; i >= 0; i--, pH -= n)
    {
        u[0] = pH[i] - eigen_real[row];
        v[0] = pH[row];
        pV = H + n * k;
        for (j = k; j < row; j++, pV += n)
            v[0] += pH[j] * pV[row];
        if (eigen_imag[i] < 0.0)
        {
            u[3] = u[0];
            v[1] = v[0];
        }
        else
        {
            k = i;
            if (eigen_imag[i] == 0.0)
            {
                if (u[0] != 0.0)
                    pH[row] = -v[0] / u[0];
                else
                    pH[row] = -v[0] / zero_tolerance;
            }
            else
            {
                u[1] = pH[i + 1];
                u[2] = pH[n + i];
                x = (eigen_real[i] - eigen_real[row]);
                x *= x;
                x += eigen_imag[i] * eigen_imag[i];
                pH[row] = (u[1] * v[1] - u[3] * v[0]) / x;
                if (fabs(u[1]) > fabs(u[3]))
                    pH[n + row] = -(v[0] + u[0] * pH[row]) / u[1];
                else
                    pH[n + row] = -(v[1] + u[2] * pH[row]) / u[3];
            }
        }
    }
}

void MagnetometerCalibration::BackSubstitute_Complex_Vector(
        float *H, float eigen_real[], float eigen_imag[], int row,
        float zero_tolerance, int n)
{
    float *pH;
    float *pV;
    float x, y;
    float u[4];
    float v[2];
    float w[2];
    int i, j, k;

    k = row - 1;
    pH = H + n * row;
    if (fabs(pH[k]) > fabs(pH[row - n]))
    {
        pH[k - n] = -(pH[row] - eigen_real[row]) / pH[k];
        pH[row - n] = -eigen_imag[row] / pH[k];
    }
    else
        Complex_Division(-pH[row - n], 0.0, pH[k - n] - eigen_real[row],
                         eigen_imag[row], &pH[k - n], &pH[row - n]);
    pH[k] = 1.0;
    pH[row] = 0.0;
    for (i = row - 2, pH = H + n * i; i >= 0; pH -= n, i--)
    {
        u[0] = pH[i] - eigen_real[row];
        w[0] = pH[row];
        w[1] = 0.0;
        pV = H + k * n;
        for (j = k; j < row; j++, pV += n)
        {
            w[0] += pH[j] * pV[row - 1];
            w[1] += pH[j] * pV[row];
        }
        if (eigen_imag[i] < 0.0)
        {
            u[3] = u[0];
            v[0] = w[0];
            v[1] = w[1];
        }
        else
        {
            k = i;
            if (eigen_imag[i] == 0.0)
            {
                Complex_Division(-w[0], -w[1], u[0], eigen_imag[row],
                                 &pH[row - 1], &pH[row]);
            }
            else
            {
                u[1] = pH[i + 1];
                u[2] = pH[n + i];
                x = eigen_real[i] - eigen_real[row];
                y = 2.0 * x * eigen_imag[row];
                x = x * x + eigen_imag[i] * eigen_imag[i]
                        - eigen_imag[row] * eigen_imag[row];
                if (x == 0.0 && y == 0.0)
                    x = zero_tolerance
                            * (fabs(u[0]) + fabs(u[1]) + fabs(u[2]) + fabs(u[3])
                                    + fabs(eigen_imag[row]));
                Complex_Division(
                        u[1] * v[0] - u[3] * w[0] + w[1] * eigen_imag[row],
                        u[1] * v[1] - u[3] * w[1] - w[0] * eigen_imag[row], x,
                        y, &pH[row - 1], &pH[row]);
                if (fabs(u[1]) > (fabs(u[3]) + fabs(eigen_imag[row])))
                {
                    pH[n + row - 1] = -w[0] - u[0] * pH[row - 1]
                            + eigen_imag[row] * pH[row] / u[1];
                    pH[n + row] = -w[1] - u[0] * pH[row]
                            - eigen_imag[row] * pH[row - 1] / u[1];
                }
                else
                {
                    Complex_Division(-v[0] - u[2] * pH[row - 1],
                                     -v[1] - u[2] * pH[row], u[3],
                                     eigen_imag[row], &pH[n + row - 1],
                                     &pH[n + row]);
                }
            }
        }
    }
}

void MagnetometerCalibration::Calculate_Eigenvectors(float *H, float *S,
                                                     float eigen_real[],
                                                     float eigen_imag[], int n)
{
    float *pH;
    float *pS;
    float x, y;
    int i, j, k;

    for (k = n - 1; k >= 0; k--)
    {
        if (eigen_imag[k] < 0.0)
        {
            for (i = 0, pS = S; i < n; pS += n, i++)
            {
                x = 0.0;
                y = 0.0;
                for (j = 0, pH = H; j <= k; pH += n, j++)
                {
                    x += pS[j] * pH[k - 1];
                    y += pS[j] * pH[k];
                }
                pS[k - 1] = x;
                pS[k] = y;
            }
        }
        else if (eigen_imag[k] == 0.0)
        {
            for (i = 0, pS = S; i < n; i++, pS += n)
            {
                x = 0.0;
                for (j = 0, pH = H; j <= k; j++, pH += n)
                    x += pS[j] * pH[k];
                pS[k] = x;
            }
        }
    }
}

void MagnetometerCalibration::Identity_Matrix(float *A, int n)
{
    int i, j;

    for (i = 0; i < n - 1; i++)
    {
        *A++ = 1.0;
        for (j = 0; j < n; j++)
            *A++ = 0.0;
    }
    *A = 1.0;
}

void MagnetometerCalibration::Complex_Division(float x, float y, float u,
                                               float v, float *a, float *b)
{
    float q = u * u + v * v;

    *a = (x * u + y * v) / q;
    *b = (y * u - x * v) / q;
}

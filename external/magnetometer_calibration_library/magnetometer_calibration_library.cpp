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

#include <external/magnetometer_calibration_library/magnetometer_calibration_library.h>
#include <float.h>
#include <math.h>

void MagnetometerCalibrationLibrary::InitialiseMatrixC(Matrix &C) {
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

void MagnetometerCalibrationLibrary::ComputeMatrixSS(const Matrix &S,
                                                     Matrix &SS, Matrix &S22a) {
    double S11_data[6][6];
    Matrix S11(S11_data);
    S11.Slice(0, 5, 0, 5, S);
    double S12_data[6][4];
    Matrix S12(S12_data);
    S12.Slice(0, 5, 6, 9, S);
    double S12t_data[4][6];
    Matrix S12t(S12t_data);
    S12t.Slice(6, 9, 0, 5, S);
    double S22_data[4][4];
    Matrix S22(S22_data);
    S22.Slice(6, 9, 6, 9, S);

    double S22_1_data[4][4];
    Matrix S22_1(S22_1_data);
    S22_1.InvertMatrix<4>(S22);

    // Calculate S22a = S22_1 * S12t   4*6 = 4x4 * 4x6   C = AB
    S22a.Multiply(S22_1, S12t);

    // Then calculate S22b = S12 * S22a      ( 6x6 = 6x4 * 4x6)
    double S22b_data[6][6];
    Matrix S22b(S22b_data);
    S22b.Multiply(S12, S22a);

    // Calculate SS = S11 - S22b
    SS.Subtract(S11, S22b);
}

void MagnetometerCalibrationLibrary::CalculateEigenVector1(const Matrix &SS,
                                                           const Matrix &C,
                                                           Matrix &v1) {
    uint8_t i, j, index;
    double maxVal, norm;
    double E_data[6][6];
    Matrix E(E_data);

    E.Multiply(C, SS);

    double E_array[36];
    for (i = 0; i < 6; i++) {
        for (j = 0; j < 6; j++) {
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
    for (i = 1; i < 6; i++) {
        if (eigen_real[i] > maxVal) {
            maxVal = eigen_real[i];
            index = i;
        }
    }
    for (i = 0; i < 6; i++) {
        v1.Set(i, 0, SSS[index + i * 6]);
    }

    // normalize v1
    norm = Matrix::VectorNorm(v1);
    for (uint8_t i = 0; i < 6; i++) {
        v1.Set(i, 0, v1.Get(i, 0) / norm);
    }
    if (v1.Get(0, 0) < 0.0) {
        for (uint8_t i = 0; i < 6; i++) {
            v1.Set(i, 0, -v1.Get(i, 0));
        }
    }
}

void MagnetometerCalibrationLibrary::InitialiseEigenVector(const Matrix &SS,
                                                           const Matrix &S22a,
                                                           Matrix &v) {
    double C_data[6][6];
    double v1_data[6][1];
    double v2_data[4][1];
    Matrix v1(v1_data);
    Matrix v2(v2_data);
    Matrix C(C_data);

    // Create pre-inverted constraint matrix C
    InitialiseMatrixC(C);
    CalculateEigenVector1(SS, C, v1);
    // Calculate v2 = S22a * v1      ( 4x1 = 4x6 * 6x1)
    CalculateEigenVector2(S22a, v1, v2);

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

double MagnetometerCalibrationLibrary::CalculateHMB(const Matrix &B_est,
                                                    const Matrix &Q,
                                                    const Matrix &v) {
    double Bt_data[1][3];
    Matrix Bt(Bt_data);
    Bt.Transpose(B_est);

    // First calculate QB = Q * B   ( 3x1 = 3x3 * 3x1)
    double QB_data[3][1];
    Matrix QB(QB_data);
    QB.Multiply(Q, B_est);

    // Then calculate btqb = BT * QB    ( 1x1 = 1x3 * 3x1)
    double btqb_data[1][1];
    Matrix btqb(btqb_data);
    btqb.Multiply(Bt, QB);

    // Calculate hmb = sqrt(btqb - J).
    double J = v.Get(9, 0);
    return sqrt(btqb.Get(0, 0) - J);
}

void MagnetometerCalibrationLibrary::CalculateEigenVector2(const Matrix &S22a,
                                                           const Matrix &v1,
                                                           Matrix &v2) {
    v2.Multiply(S22a, v1);
}

void MagnetometerCalibrationLibrary::Hessenberg_Elementary_Transform(
    double *H, double *S, const uint8_t perm[], uint8_t n) {
    uint8_t i, j;

    Identity_Matrix(S, n);
    for (i = n - 2; i >= 1; i--) {
        double *pH = H + n * (i + 1);
        double *pS = S + n * (i + 1);
        for (j = i + 1; j < n; pH += n, pS += n, j++) {
            *(pS + i) = *(pH + i - 1);
            *(pH + i - 1) = 0.0;
        }
        if (perm[i] != i) {
            pS = S + n * i;
            pH = S + n * perm[i];
            for (j = i; j < n; j++) {
                *(pS + j) = *(pH + j);
                *(pH + j) = 0.0;
            }
            *(pH + i) = 1.0;
        }
    }
}

void MagnetometerCalibrationLibrary::Hessenberg_Form_Elementary(double *A,
                                                                double *S,
                                                                uint8_t n) {
    uint8_t i, j, col;
    double *p_row, *pS_row;
    double max;
    double s;
    double *pA, *pB, *pC, *pS;

    // n x n matrices for which n <= 2 are already in Hessenberg form

    if (n <= 1) {
        *S = 1.0;
        return;
    }
    if (n == 2) {
        *S++ = 1.0;
        *S++ = 0.0;
        *S++ = 1.0;
        *S = 0.0;
        return;
    }

    // Allocate working memory
    uint8_t perm[n];

    // For each column use Elementary transformations
    //   to zero the entries below the subdiagonal.

    p_row = A + n;
    pS_row = S + n;
    for (col = 0; col < (n - 2); p_row += n, pS_row += n, col++) {
        // Find the row in column "col" with maximum magnitude where
        // row >= col + 1.

        uint8_t row = col + 1;
        perm[row] = row;
        for (pA = p_row + col, max = 0.0, i = row; i < n; pA += n, i++)
            if (fabs(*pA) > max) {
                perm[row] = i;
                max = fabs(*pA);
            }

        // If perm[row] != row, then interchange row "row" and row
        // perm[row] and interchange column "row" and column perm[row].

        if (perm[row] != row) {
            Interchange_Rows(A, row, perm[row], n);
            Interchange_Columns(A, row, perm[row], n, n);
        }

        // Zero out the components lying below the subdiagonal.

        pA = p_row + n;
        pS = pS_row + n;
        for (i = col + 2; i < n; pA += n, pS += n, i++) {
            s = *(pA + col) / *(p_row + col);
            for (j = 0; j < n; j++) *(pA + j) -= *(p_row + j) * s;
            *(pS + col) = s;
            for (j = 0, pB = A + col + 1, pC = A + i; j < n;
                 pB += n, pC += n, j++)
                *pB += s * *pC;
        }
    }
    pA = A + n + n;
    pS = S + n + n;
    for (i = 2; i < n; pA += n, pS += n, i++) Copy_Vector(pA, pS, i - 1);

    Hessenberg_Elementary_Transform(A, S, perm, n);
}

void MagnetometerCalibrationLibrary::Interchange_Rows(double *A, uint8_t row1,
                                                      uint8_t row2,
                                                      uint8_t ncols) {
    uint8_t i;
    double *pA1, *pA2;

    pA1 = A + row1 * ncols;
    pA2 = A + row2 * ncols;
    for (i = 0; i < ncols; i++) {
        double temp = *pA1;
        *pA1++ = *pA2;
        *pA2++ = temp;
    }
}

void MagnetometerCalibrationLibrary::Interchange_Columns(
    double *A, uint8_t col1, uint8_t col2, uint8_t nrows, uint8_t ncols) {
    uint8_t i;
    double *pA1, *pA2;

    pA1 = A + col1;
    pA2 = A + col2;
    for (i = 0; i < nrows; pA1 += ncols, pA2 += ncols, i++) {
        double temp = *pA1;
        *pA1 = *pA2;
        *pA2 = temp;
    }
}

void MagnetometerCalibrationLibrary::Copy_Vector(double *d, double *s,
                                                 uint8_t n) {
    // memcpy(d, s, sizeof(double) * n);
    for (uint8_t i = 0; i < n; i++) {
        d[i] = s[i];
    }
}

void MagnetometerCalibrationLibrary::QR_Hessenberg_Matrix(
    double *H, double *S, double eigen_real[], double eigen_imag[], uint8_t n,
    uint8_t max_iteration_count) {
    uint8_t i;
    int row;
    uint8_t iteration;
    double shift = 0.0;
    double *pH;

    for (row = n - 1; row >= 0; row--) {
        bool found_eigenvalue = false;
        for (iteration = 1; iteration <= max_iteration_count; iteration++) {
            // Search for small subdiagonal element

            for (i = row, pH = H + row * n; i > 0; i--, pH -= n)
                if (fabs(*(pH + i - 1)) <=
                    DBL_EPSILON * (fabs(*(pH - n + i - 1)) + fabs(*(pH + i))))
                    break;

            // If the subdiagonal element on row "row" is small, then
            // that row element is an eigenvalue.  If the subdiagonal
            // element on row "row-1" is small, then the eigenvalues
            // of the 2x2 diagonal block consisting rows "row-1" and
            // "row" are eigenvalues.  Otherwise perform a double QR
            // iteration.

            switch (row - i) {
                case 0:  // One real eigenvalue
                    One_Real_Eigenvalue(pH, eigen_real, eigen_imag, i, shift);
                    found_eigenvalue = true;
                    break;
                case 1:  // Either two real eigenvalues or a complex pair
                    row--;
                    Two_Eigenvalues(H, S, eigen_real, eigen_imag, n, row,
                                    shift);
                    found_eigenvalue = true;
                    break;
                default:
                    Double_QR_Iteration(H, S, i, row, n, &shift, iteration);
            }
            if (found_eigenvalue) break;
        }
        if (iteration > max_iteration_count) return;
    }

    BackSubstitution(H, eigen_real, eigen_imag, n);
    Calculate_Eigenvectors(H, S, eigen_real, eigen_imag, n);

    return;
}

void MagnetometerCalibrationLibrary::One_Real_Eigenvalue(double Hrow[],
                                                         double eigen_real[],
                                                         double eigen_imag[],
                                                         uint8_t row,
                                                         double shift) {
    Hrow[row] += shift;
    eigen_real[row] = Hrow[row];
    eigen_imag[row] = 0.0;
}

void MagnetometerCalibrationLibrary::Two_Eigenvalues(double *H, double *S,
                                                     double eigen_real[],
                                                     double eigen_imag[],
                                                     uint8_t n, uint8_t row,
                                                     double shift) {
    double p, x, discriminant;
    double *Hrow = H + n * row;
    double *Hnextrow = Hrow + n;
    uint8_t nextrow = row + 1;

    p = 0.5 * (Hrow[row] - Hnextrow[nextrow]);
    x = Hrow[nextrow] * Hnextrow[row];
    discriminant = p * p + x;
    Hrow[row] += shift;
    Hnextrow[nextrow] += shift;
    if (discriminant > 0.0) {  // pair of real roots
        double q = sqrt(discriminant);
        if (p < 0.0)
            q = p - q;
        else
            q += p;
        eigen_real[row] = Hnextrow[nextrow] + q;
        eigen_real[nextrow] = Hnextrow[nextrow] - x / q;
        eigen_imag[row] = 0.0;
        eigen_imag[nextrow] = 0.0;
        double r = sqrt(Hnextrow[row] * Hnextrow[row] + q * q);
        double sin_ = Hnextrow[row] / r;
        double cos_ = q / r;
        Update_Row(Hrow, cos_, sin_, n, row);
        Update_Column(H, cos_, sin_, n, row);
        Update_Transformation(S, cos_, sin_, n, row);
    } else {  // pair of complex roots
        eigen_real[nextrow] = eigen_real[row] = Hnextrow[nextrow] + p;
        eigen_imag[row] = sqrt(fabs(discriminant));
        eigen_imag[nextrow] = -eigen_imag[row];
    }
}

void MagnetometerCalibrationLibrary::Update_Row(double *Hrow, double cos_,
                                                double sin_, uint8_t n,
                                                uint8_t row) {
    double *Hnextrow = Hrow + n;
    uint8_t i;

    for (i = row; i < n; i++) {
        double x = Hrow[i];
        Hrow[i] = cos_ * x + sin_ * Hnextrow[i];
        Hnextrow[i] = cos_ * Hnextrow[i] - sin_ * x;
    }
}

void MagnetometerCalibrationLibrary::Update_Column(double *H, double cos_,
                                                   double sin_, uint8_t n,
                                                   uint8_t col) {
    uint8_t i;
    uint8_t next_col = col + 1;

    for (i = 0; i <= next_col; i++, H += n) {
        double x = H[col];
        H[col] = cos_ * x + sin_ * H[next_col];
        H[next_col] = cos_ * H[next_col] - sin_ * x;
    }
}

void MagnetometerCalibrationLibrary::Update_Transformation(
    double *S, double cos_, double sin_, uint8_t n, uint8_t k) {
    uint8_t i;
    uint8_t k1 = k + 1;

    for (i = 0; i < n; i++, S += n) {
        double x = S[k];
        S[k] = cos_ * x + sin_ * S[k1];
        S[k1] = cos_ * S[k1] - sin_ * x;
    }
}

void MagnetometerCalibrationLibrary::Double_QR_Iteration(
    double *H, double *S, uint8_t min_row, uint8_t max_row, uint8_t n,
    double *shift, uint8_t iteration) {
    uint8_t k;
    double trace, det;

    Product_and_Sum_of_Shifts(H, n, max_row, shift, &trace, &det, iteration);
    k = Two_Consecutive_Small_Subdiagonal(H, min_row, max_row, n, trace, det);
    Double_QR_Step(H, min_row, max_row, k, trace, det, S, n);
}

void MagnetometerCalibrationLibrary::Product_and_Sum_of_Shifts(
    double *H, uint8_t n, uint8_t max_row, double *shift, double *trace,
    double *det, uint8_t iteration) {
    double *pH = H + max_row * n;
    double *p_aux;
    uint8_t min_col = max_row - 1;

    if ((iteration % 10) == 0) {
        *shift += pH[max_row];
        p_aux = H;
        for (uint8_t i = 0; i <= max_row; p_aux += n, i++) {
            p_aux[i] -= pH[max_row];
        }
        p_aux = pH - n;
        *trace = fabs(pH[min_col]) + fabs(p_aux[min_col - 1]);
        *det = *trace * *trace;
        *trace *= 1.5;
    } else {
        p_aux = pH - n;
        *trace = p_aux[min_col] + pH[max_row];
        *det = p_aux[min_col] * pH[max_row] - p_aux[max_row] * pH[min_col];
    }
}

uint8_t MagnetometerCalibrationLibrary::Two_Consecutive_Small_Subdiagonal(
    double *H, uint8_t min_row, uint8_t max_row, uint8_t n, double trace,
    double det) {
    double *pH;
    int k;
    uint8_t i;

    for (k = max_row - 2, pH = H + k * n; k >= min_row; pH -= n, k--) {
        double x = (pH[k] * (pH[k] - trace) + det) / pH[n + k] + pH[k + 1];
        double y = pH[k] + pH[n + k + 1] - trace;
        double z = pH[n + n + k + 1];
        double s = fabs(x) + fabs(y) + fabs(z);
        x /= s;
        y /= s;
        z /= s;
        if (k == min_row) break;
        if ((fabs(pH[k - 1]) * (fabs(y) + fabs(z))) <=
            DBL_EPSILON * fabs(x) *
                (fabs(pH[k - 1 - n]) + fabs(pH[k]) + fabs(pH[n + k + 1])))
            break;
    }
    for (i = k + 2, pH = H + i * n; i <= max_row; pH += n, i++) pH[i - 2] = 0.0;
    for (i = k + 3, pH = H + i * n; i <= max_row; pH += n, i++) pH[i - 3] = 0.0;
    return k;
}

void MagnetometerCalibrationLibrary::Double_QR_Step(double *H, uint8_t min_row,
                                                    uint8_t max_row,
                                                    uint8_t min_col,
                                                    double trace, double det,
                                                    double *S, uint8_t n) {
    double s, x, y, z;
    double a, b, c;
    double *pH;
    double *tH;
    double *pS;
    uint8_t i, j, k;
    uint8_t last_test_row_col = max_row - 1;

    k = min_col;
    pH = H + min_col * n;
    a = (pH[k] * (pH[k] - trace) + det) / pH[n + k] + pH[k + 1];
    b = pH[k] + pH[n + k + 1] - trace;
    c = pH[n + n + k + 1];
    s = fabs(a) + fabs(b) + fabs(c);
    a /= s;
    b /= s;
    c /= s;

    for (; k <= last_test_row_col; k++, pH += n) {
        if (k > min_col) {
            c = (k == last_test_row_col) ? 0.0 : pH[n + n + k - 1];
            x = fabs(pH[k - 1]) + fabs(pH[n + k - 1]) + fabs(c);
            if (x == 0.0) continue;
            a = pH[k - 1] / x;
            b = pH[n + k - 1] / x;
            c /= x;
        }
        s = sqrt(a * a + b * b + c * c);
        if (a < 0.0) s = -s;
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
        for (j = k; j < n; j++) {
            a = pH[j] + b * pH[n + j];
            if (k != last_test_row_col) {
                a += c * pH[n + n + j];
                pH[n + n + j] -= a * z;
            }
            pH[n + j] -= a * y;
            pH[j] -= a * x;
        }

        // Update column k+1

        j = k + 3;
        if (j > max_row) j = max_row;
        for (i = 0, tH = H; i <= j; i++, tH += n) {
            a = x * tH[k] + y * tH[k + 1];
            if (k != last_test_row_col) {
                a += z * tH[k + 2];
                tH[k + 2] -= a * c;
            }
            tH[k + 1] -= a * b;
            tH[k] -= a;
        }

        // Update transformation matrix

        for (i = 0, pS = S; i < n; pS += n, i++) {
            a = x * pS[k] + y * pS[k + 1];
            if (k != last_test_row_col) {
                a += z * pS[k + 2];
                pS[k + 2] -= a * c;
            }
            pS[k + 1] -= a * b;
            pS[k] -= a;
        }
    };
}

void MagnetometerCalibrationLibrary::BackSubstitution(double *H,
                                                      double eigen_real[],
                                                      double eigen_imag[],
                                                      uint8_t n) {
    double zero_tolerance;
    double *pH;
    uint8_t i, j;
    int row;

    // Calculate the zero tolerance

    pH = H;
    zero_tolerance = fabs(pH[0]);
    for (pH += n, i = 1; i < n; pH += n, i++) {
        for (j = i - 1; j < n; j++) {
            zero_tolerance += fabs(pH[j]);
        }
    }
    zero_tolerance *= DBL_EPSILON;

    // Start Backsubstitution

    for (row = n - 1; row >= 0; row--) {
        if (eigen_imag[row] == 0.0) {
            BackSubstitute_Real_Vector(H, eigen_real, eigen_imag, row,
                                       zero_tolerance, n);
        } else if (eigen_imag[row] < 0.0) {
            BackSubstitute_Complex_Vector(H, eigen_real, eigen_imag, row,
                                          zero_tolerance, n);
        }
    }
}

void MagnetometerCalibrationLibrary::BackSubstitute_Real_Vector(
    double *H, double eigen_real[], double eigen_imag[], uint8_t row,
    double zero_tolerance, uint8_t n) {
    double *pH;
    double x;
    double u[4];
    double v[2];
    int i;
    uint8_t j, k;

    k = row;
    pH = H + row * n;
    pH[row] = 1.0;
    for (i = row - 1, pH -= n; i >= 0; i--, pH -= n) {
        u[0] = pH[i] - eigen_real[row];
        v[0] = pH[row];
        double *pV = H + n * k;
        for (j = k; j < row; j++, pV += n) v[0] += pH[j] * pV[row];
        if (eigen_imag[i] < 0.0) {
            u[3] = u[0];
            v[1] = v[0];
        } else {
            k = i;
            if (eigen_imag[i] == 0.0) {
                if (u[0] != 0.0)
                    pH[row] = -v[0] / u[0];
                else
                    pH[row] = -v[0] / zero_tolerance;
            } else {
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

void MagnetometerCalibrationLibrary::BackSubstitute_Complex_Vector(
    double *H, double eigen_real[], double eigen_imag[], uint8_t row,
    double zero_tolerance, uint8_t n) {
    double *pH;
    double x, y;
    double u[4];
    double v[2];
    double w[2];
    int i;
    uint8_t j, k;

    k = row - 1;
    pH = H + n * row;
    if (fabs(pH[k]) > fabs(pH[row - n])) {
        pH[k - n] = -(pH[row] - eigen_real[row]) / pH[k];
        pH[row - n] = -eigen_imag[row] / pH[k];
    } else
        Complex_Division(-pH[row - n], 0.0, pH[k - n] - eigen_real[row],
                         eigen_imag[row], &pH[k - n], &pH[row - n]);
    pH[k] = 1.0;
    pH[row] = 0.0;
    for (i = row - 2, pH = H + n * i; i >= 0; pH -= n, i--) {
        u[0] = pH[i] - eigen_real[row];
        w[0] = pH[row];
        w[1] = 0.0;
        double *pV = H + k * n;
        for (j = k; j < row; j++, pV += n) {
            w[0] += pH[j] * pV[row - 1];
            w[1] += pH[j] * pV[row];
        }
        if (eigen_imag[i] < 0.0) {
            u[3] = u[0];
            v[0] = w[0];
            v[1] = w[1];
        } else {
            k = i;
            if (eigen_imag[i] == 0.0) {
                Complex_Division(-w[0], -w[1], u[0], eigen_imag[row],
                                 &pH[row - 1], &pH[row]);
            } else {
                u[1] = pH[i + 1];
                u[2] = pH[n + i];
                x = eigen_real[i] - eigen_real[row];
                y = 2.0 * x * eigen_imag[row];
                x = x * x + eigen_imag[i] * eigen_imag[i] -
                    eigen_imag[row] * eigen_imag[row];
                if (x == 0.0 && y == 0.0)
                    x = zero_tolerance * (fabs(u[0]) + fabs(u[1]) + fabs(u[2]) +
                                          fabs(u[3]) + fabs(eigen_imag[row]));
                Complex_Division(
                    u[1] * v[0] - u[3] * w[0] + w[1] * eigen_imag[row],
                    u[1] * v[1] - u[3] * w[1] - w[0] * eigen_imag[row], x, y,
                    &pH[row - 1], &pH[row]);
                if (fabs(u[1]) > (fabs(u[3]) + fabs(eigen_imag[row]))) {
                    pH[n + row - 1] = -w[0] - u[0] * pH[row - 1] +
                                      eigen_imag[row] * pH[row] / u[1];
                    pH[n + row] = -w[1] - u[0] * pH[row] -
                                  eigen_imag[row] * pH[row - 1] / u[1];
                } else {
                    Complex_Division(
                        -v[0] - u[2] * pH[row - 1], -v[1] - u[2] * pH[row],
                        u[3], eigen_imag[row], &pH[n + row - 1], &pH[n + row]);
                }
            }
        }
    }
}

void MagnetometerCalibrationLibrary::Calculate_Eigenvectors(
    double *H, double *S, double eigen_real[], double eigen_imag[], uint8_t n) {
    double *pH;
    double *pS;
    double x, y;
    uint8_t i, j;
    int k;

    for (k = n - 1; k >= 0; k--) {
        if (eigen_imag[k] < 0.0) {
            for (i = 0, pS = S; i < n; pS += n, i++) {
                x = 0.0;
                y = 0.0;
                for (j = 0, pH = H; j <= k; pH += n, j++) {
                    x += pS[j] * pH[k - 1];
                    y += pS[j] * pH[k];
                }
                pS[k - 1] = x;
                pS[k] = y;
            }
        } else if (eigen_imag[k] == 0.0) {
            for (i = 0, pS = S; i < n; i++, pS += n) {
                x = 0.0;
                for (j = 0, pH = H; j <= k; j++, pH += n) x += pS[j] * pH[k];
                pS[k] = x;
            }
        }
    }
}

void MagnetometerCalibrationLibrary::Identity_Matrix(double *A, uint8_t n) {
    uint8_t i, j;

    for (i = 0; i < n - 1; i++) {
        *A++ = 1.0;
        for (j = 0; j < n; j++) *A++ = 0.0;
    }
    *A = 1.0;
}

void MagnetometerCalibrationLibrary::Complex_Division(double x, double y,
                                                      double u, double v,
                                                      double *a, double *b) {
    double q = u * u + v * v;

    *a = (x * u + y * v) / q;
    *b = (y * u - x * v) / q;
}

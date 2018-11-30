#include <math.h>
#include <src/util/matrix.h>
#include <src/util/msp_exception.h>

// TODO(dingbenjamin): Split up exception throws into row/column mismatches to
// return failed arguments

void Matrix::CopySlice(uint8_t row_start, uint8_t row_end, uint8_t column_start,
                       uint8_t column_end, const Matrix &A) {
    if (row_start >= A.GetNRows() || row_end >= A.GetNRows() ||
        row_end < row_start || column_start >= A.GetNColumns() ||
        column_end >= A.GetNColumns() || column_end < column_start ||
        nrows != row_end - row_start + 1 ||
        ncolumns != column_end - column_start + 1) {
        throw MspException("Matrix::Slice arguments outside bounds",
                           kMatrixSliceArgumentFail, __FILE__, __LINE__);
    }
    nrows = row_end - row_start + 1;
    ncolumns = column_end - column_start + 1;
    for (uint8_t i = 0; i < nrows; i++) {
        for (uint8_t j = 0; j < ncolumns; j++) {
            Set(i, j, A.Get(i + row_start, j + column_start));
        }
    }
}

uint8_t Matrix::GetNRows() const { return nrows; }

uint8_t Matrix::GetNColumns() const { return ncolumns; }

bool Matrix::IsSquare() const { return nrows == ncolumns; }

double Matrix::Get(uint8_t row, uint8_t column) const {
    if (row >= nrows || column >= ncolumns) {  // uint8_t always > 0
        throw MspException("Matrix::Get indices out of bounds",
                           kMatrixGetOutOfBoundsFail, __FILE__, __LINE__);
    }
    return data[(row * ncolumns) + column];
}

void Matrix::Set(uint8_t row, uint8_t column, double value) {
    if (row >= nrows || column >= ncolumns) {  // uint8_t always > 0
        throw MspException("Matrix::Set indices out of bounds",
                           kMatrixIndexOutOfBoundsFail, __FILE__, __LINE__);
    }
    data[(row * ncolumns) + column] = value;
}

//  Test equality using relative difference. Additive tolerance allows
//  comparison with zero, but it's set arbitrarily.
bool Matrix::DoubleIsEqual(double a, double b) {
    if (isinf(a) || isinf(b)) {
        throw MspException("Matrix::DoubleIsEqual argument isinf",
                           kMatrixDoubleIsEqualInfFail, __FILE__, __LINE__);
    }
    if (isnan(a) || isnan(b)) {
        throw MspException("Matrix::DoubleIsEqual argument is nan",
                           kMatrixDoubleIsEqualNanFail, __FILE__, __LINE__);
    }
    if (a > b) {
        return fabs(a - b) <= fabs(a * EPSILON_MULT + EPSILON_ADD);
    }
    return fabs(a - b) <= fabs(b * EPSILON_MULT + EPSILON_ADD);
}

bool Matrix::IsEqual(const Matrix &A) const {
    if (!SameSize(A)) {
        throw MspException("Matrix::IsEqual arguments' sizes don't match",
                           kMatrixIsEqualArgumentFail, __FILE__, __LINE__);
    }
    for (uint8_t i = 0; i < nrows; i++) {
        for (uint8_t j = 0; j < ncolumns; j++) {
            if (!DoubleIsEqual(Get(i, j), A.Get(i, j))) {
                return false;
            }
        }
    }
    return true;
}

bool Matrix::SameSize(const Matrix &A) const {
    return SameNRows(A) && SameNColumns(A);
}

bool Matrix::SameNRows(const Matrix &A) const { return nrows == A.GetNRows(); }

bool Matrix::SameNColumns(const Matrix &A) const {
    return ncolumns == A.GetNColumns();
}

void Matrix::Transpose(const Matrix &A) {
    if (nrows != A.GetNColumns() || ncolumns != A.GetNRows()) {
        throw MspException("Matrix::Transpose arguments' sizes don't match",
                           kMatrixTransposeSizeFail, __FILE__, __LINE__);
    }
    for (uint8_t i = 0; i < nrows; i++) {
        for (uint8_t j = 0; j < ncolumns; j++) {
            Set(i, j, A.Get(j, i));
        }
    }
}

double Matrix::VectorNorm(const Matrix &A) {
    if (A.GetNColumns() != 1) {
        throw MspException("Matrix::VectorNorm must be a column vector",
                           kMatrixVectorNormNotColumnFail, __FILE__, __LINE__);
    }
    double sum_of_squares = 0;
    for (uint8_t i = 0; i < A.GetNRows(); i++) {
        sum_of_squares += A.Get(i, 0) * A.Get(i, 0);
    }
    return sqrt(sum_of_squares);
}

void Matrix::Add(const Matrix &A, const Matrix &B, const Matrix &C) {
    if (!SameSize(A) || !SameSize(B) || !SameSize(C)) {
        MspException("Matrix::Add arguments' sizes don't match",
                     kMatrixAddInvalidSizeFail, __FILE__, __LINE__);
    }
    for (uint8_t i = 0; i < nrows; i++) {
        for (uint8_t j = 0; j < ncolumns; j++) {
            Set(i, j, A.Get(i, j) + B.Get(i, j) + C.Get(i, j));
        }
    }
}

void Matrix::Add(const Matrix &A, const Matrix &B) {
    if (!SameSize(A) || !SameSize(B)) {
        MspException("Matrix::Add arguments' sizes don't match",
                     kMatrixAddInvalidSizeFail, __FILE__, __LINE__);
    }
    for (uint8_t i = 0; i < nrows; i++) {
        for (uint8_t j = 0; j < ncolumns; j++) {
            Set(i, j, A.Get(i, j) + B.Get(i, j));
        }
    }
}

void Matrix::Subtract(const Matrix &A, const Matrix &B) {
    if (!SameSize(A) || !SameSize(B)) {
        throw MspException("Matrix::Subtract arguments' sizes don't match",
                           kMatrixSubtractInvalidSizeFail, __FILE__, __LINE__);
    }
    for (uint8_t i = 0; i < nrows; i++) {
        for (uint8_t j = 0; j < ncolumns; j++) {
            Set(i, j, A.Get(i, j) - B.Get(i, j));
        }
    }
}

void Matrix::Multiply(const Matrix &A, const Matrix &B) {
    if (A.GetNColumns() != B.GetNRows() || !SameNRows(A) || !SameNColumns(B)) {
        throw MspException("Matrix::Multiply dimensions don't match",
                           kMatrixMultiplyInvalidDimensionFail, __FILE__,
                           __LINE__);
    }
    for (uint8_t i = 0; i < nrows; i++) {
        for (uint8_t j = 0; j < ncolumns; j++) {
            double n = 0;
            for (uint8_t k = 0; k < A.GetNColumns(); k++) {
                n += A.Get(i, k) * B.Get(k, j);
            }
            Set(i, j, n);
        }
    }
}

void Matrix::MultiplyScalar(const Matrix &A, double scale) {
    if (!SameSize(A)) {
        throw MspException(
            "Matrix::MultiplyScalar arguments' sizes don't match",
            kMatrixMultiplyScalarSizeFail, __FILE__, __LINE__);
    }
    for (uint8_t i = 0; i < nrows; i++) {
        for (uint8_t j = 0; j < ncolumns; j++) {
            Set(i, j, A.Get(i, j) * scale);
        }
    }
}

// Only valid for vectors represented as 3x1 matrices
void Matrix::CrossProduct(const Matrix &A, const Matrix &B) {
    if (A.GetNRows() != 3 || A.GetNColumns() != 1 || !B.SameSize(A) ||
        !SameSize(A)) {
        throw MspException(
            "Matrix::CrossProduct arguments or 'this' aren't column vectors",
            kMatrixCrossProductArgumentsFail, __FILE__, __LINE__);
    }
    Set(0, 0, A.Get(1, 0) * B.Get(2, 0) - A.Get(2, 0) * B.Get(1, 0));
    Set(1, 0, A.Get(2, 0) * B.Get(0, 0) - A.Get(0, 0) * B.Get(2, 0));
    Set(2, 0, A.Get(0, 0) * B.Get(1, 0) - A.Get(1, 0) * B.Get(0, 0));
}

void Matrix::Fill(double value) {
    for (uint8_t i = 0; i < nrows; i++) {
        for (uint8_t j = 0; j < ncolumns; j++) {
            Set(i, j, value);
        }
    }
}

void Matrix::CopyInto(uint8_t row_start, uint8_t column_start,
                      const Matrix &A) {
    if (nrows < A.GetNRows() + row_start ||
        ncolumns < A.GetNColumns() + column_start) {
        throw MspException(
            "Matrix::CopyInto arguments exceed the bounds of 'this'",
            kMatrixCopyIntoArgumentsFail, __FILE__, __LINE__);
    }
    for (uint8_t i = 0; i < A.GetNRows(); i++) {
        for (uint8_t j = 0; j < A.GetNColumns(); j++) {
            Set(i + row_start, j + column_start, A.Get(i, j));
        }
    }
}

void Matrix::Identity() {
    if (!IsSquare()) {
        throw MspException("Matrix::Identity argument not square",
                           kMatrixIdentityNotSquareFail, __FILE__, __LINE__);
    }
    Fill(0);

    for (uint8_t i = 0; i < nrows; i++) {
        Set(i, i, 1);
    }
}

// Only valid for quaternions represented as 4x1 matrices
void Matrix::QuaternionNormalise(const Matrix &q) {
    // TODO(rskew) break this out into a standalone quaternion library
    if (nrows != 4 || ncolumns != 1 || q.GetNRows() != 4 ||
        q.GetNColumns() != 1) {
        throw MspException("Matrix::QuaternionNormalise not a 4x1 matrix",
                           kMatrixQuaternionNormaliseArgumentFail, __FILE__,
                           __LINE__);
    }
    MultiplyScalar(q, 1 / VectorNorm(q));
}

// Only valid for quaternions represented as 4x1 matrices
void Matrix::RotationMatrixFromQuaternion(const Matrix &q) {
    if (nrows != 3 || ncolumns != 3 || q.nrows != 4 || q.ncolumns != 1) {
        throw MspException(
            "Matrix::RotationMatrixFromQuaternion incorrect dimensions",
            kMatrixRotateQuaternionArgumentFail, __FILE__, __LINE__);
    }
    NewStackMatrixMacro(q_normed, 4, 1);

    q_normed.QuaternionNormalise(q);

    double qx = q_normed.Get(0, 0);
    double qy = q_normed.Get(1, 0);
    double qz = q_normed.Get(2, 0);
    double qw = q_normed.Get(3, 0);

    Set(0, 0, 1 - 2 * qy * qy - 2 * qz * qz);
    Set(0, 1, 2 * qx * qy - 2 * qz * qw);
    Set(0, 2, 2 * qx * qz + 2 * qy * qw);
    Set(1, 0, 2 * qx * qy + 2 * qz * qw);
    Set(1, 1, 1 - 2 * qx * qx - 2 * qz * qz);
    Set(1, 2, 2 * qy * qz - 2 * qx * qw);
    Set(2, 0, 2 * qx * qz - 2 * qy * qw);
    Set(2, 1, 2 * qy * qz + 2 * qx * qw);
    Set(2, 2, 1 - 2 * qx * qx - 2 * qy * qy);
}

// Only valid for vectors represented as 3x1 matrices
void Matrix::SkewSymmetricFill(const Matrix &A) {
    if (A.GetNColumns() != 1 || A.GetNRows() != 3 || nrows != 3 ||
        ncolumns != 3) {
        throw MspException("Matrix::SkewSymmetricFill incorrect dimensions",
                           kMatrixSkewSymmetricFillArgumentFail, __FILE__,
                           __LINE__);
    }
    Set(0, 0, 0);
    Set(0, 1, -A.Get(2, 0));
    Set(0, 2, A.Get(1, 0));
    Set(1, 0, A.Get(2, 0));
    Set(1, 1, 0);
    Set(1, 2, -A.Get(0, 0));
    Set(2, 0, -A.Get(1, 0));
    Set(2, 1, A.Get(0, 0));
    Set(2, 2, 0);
}

void Matrix::ConcatenateHorizontally(const Matrix &A, const Matrix &B) {
    if (!SameNRows(A) || !SameNRows(B) ||
        ncolumns != A.GetNColumns() + B.GetNColumns()) {
        throw MspException(
            "Matrix::ConcatenateHorizontally arguments' dimensions don't match",
            kMatrixConcatenateHorizontalArgumentFail, __FILE__, __LINE__);
    }
    for (uint8_t i = 0; i < A.GetNRows(); i++) {
        for (uint8_t j = 0; j < A.GetNColumns(); j++) {
            Set(i, j, A.Get(i, j));
        }
    }
    for (uint8_t i = 0; i < B.GetNRows(); i++) {
        for (uint8_t j = 0; j < B.GetNColumns(); j++) {
            Set(i, A.GetNColumns() + j, B.Get(i, j));
        }
    }
}

void Matrix::ConcatenateVertically(const Matrix &A, const Matrix &B) {
    if (!SameNColumns(A) || !SameNColumns(B) ||
        nrows != A.GetNRows() + B.GetNRows()) {
        throw MspException(
            "Matrix::ConcatenateVertically arguments' dimensions don't match",
            kMatrixConcatenateVerticalArgumentFail, __FILE__, __LINE__);
    }
    for (uint8_t i = 0; i < A.GetNRows(); i++) {
        for (uint8_t j = 0; j < A.GetNColumns(); j++) {
            Set(i, j, A.Get(i, j));
        }
    }
    for (uint8_t i = 0; i < B.GetNRows(); i++) {
        for (uint8_t j = 0; j < B.GetNColumns(); j++) {
            Set(A.GetNRows() + i, j, B.Get(i, j));
        }
    }
}

void Matrix::AddRows(uint8_t row_to, uint8_t row_from, double scale) {
    if (row_to >= nrows || row_from >= nrows) {
        throw MspException(
            "Matrix::AddRows arguments are outside matrix dimensions",
            kMatrixAddRowsArgumentFail, __FILE__, __LINE__);
    }
    for (uint8_t i = 0; i < ncolumns; i++) {
        Set(row_to, i, Get(row_to, i) + (Get(row_from, i) * scale));
    }
}

void Matrix::MultiplyRow(uint8_t row, double scale) {
    if (row >= nrows) {
        throw MspException(
            "Matrix::MultiplyRow arguments are outside matrix dimensions",
            kMatrixMultiplyRowArgumentFail, __FILE__, __LINE__);
    }
    for (uint8_t i = 0; i < ncolumns; i++) {
        Set(row, i, Get(row, i) * scale);
    }
}

void Matrix::SwitchRows(uint8_t row_a, uint8_t row_b) {
    if (row_a >= nrows || row_b >= nrows) {
        throw MspException(
            "Matrix::SwitchRows arguments are outside matrix dimensions",
            kMatrixSwitchRowsArgumentFail, __FILE__, __LINE__);
    }
    for (uint8_t i = 0; i < ncolumns; i++) {
        double temp = Get(row_a, i);
        Set(row_a, i, Get(row_b, i));
        Set(row_b, i, temp);
    }
}

void Matrix::RowReduce() {
    uint8_t square;

    if (nrows < ncolumns) {
        square = nrows;
    } else {
        square = ncolumns;
    }

    for (uint8_t i = 0; i < square; i++) {
        uint8_t max_row = i;
        double max_element = fabs(Get(i, i));
        for (uint8_t j = i + 1; j < nrows; j++) {
            if (fabs(Get(j, i)) > max_element) {
                max_element = fabs(Get(j, i));
                max_row = j;
            }
        }
        if (DoubleIsEqual(max_element, 0)) {
            throw MspException(
                "Matrix::RowReduce linear system has no solution",
                kMatrixRowReduceNoSolutionFail, __FILE__, __LINE__);
        }

        SwitchRows(i, max_row);

        MultiplyRow(i, 1 / Get(i, i));

        for (uint8_t j = 0; j < nrows; j++) {
            if (i != j) {
                AddRows(j, i, -(Get(j, i)));
            }
        }
    }
}
void Matrix::QuaternionConjugate() {
    Set(1, 0, -1 * Get(1, 0));
    Set(2, 0, -1 * Get(2, 0));
    Set(3, 0, -1 * Get(3, 0));
    return;
}

void Matrix::QuaternionProductCross(Matrix &a, Matrix &b) {
    double x = a.Get(0, 0) * b.Get(0, 0) - a.Get(1, 0) * b.Get(1, 0) -
               a.Get(2, 0) * b.Get(2, 0) - a.Get(3, 0) * b.Get(3, 0);
    double y = a.Get(0, 0) * b.Get(1, 0) + a.Get(1, 0) * b.Get(0, 0) -
               a.Get(2, 0) * b.Get(3, 0) + a.Get(3, 0) * b.Get(2, 0);
    double z = a.Get(0, 0) * b.Get(2, 0) + a.Get(2, 0) * b.Get(0, 0) +
               a.Get(1, 0) * b.Get(3, 0) - a.Get(3, 0) * b.Get(1, 0);
    double w = a.Get(0, 0) * b.Get(3, 0) - a.Get(1, 0) * b.Get(2, 0) +
               a.Get(2, 0) * b.Get(1, 0) + a.Get(3, 0) * b.Get(0, 0);
    Set(0, 0, x);
    Set(1, 0, y);
    Set(2, 0, z);
    Set(3, 0, w);
}
void Matrix::QuaternionProductDot(Matrix &a, Matrix &b) {
    QuaternionProductCross(b, a);
}
double Matrix::DotProduct(const Matrix &A, const Matrix &B) {
    double dot = 0;

    for (uint8_t i = 0; i < 3; i++) {
        dot += A.Get(i, 0) * B.Get(i, 0);
    }

    return dot;
}

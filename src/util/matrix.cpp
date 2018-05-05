#include <external/etl/exception.h>
#include <math.h>
#include <src/util/matrix.h>

void Matrix::Slice(uint8_t row_start, uint8_t row_end, uint8_t column_start,
                   uint8_t column_end, const Matrix &A) {
    if (row_start >= A.GetNRows() || row_end >= A.GetNRows() ||
        row_end < row_start || column_start >= A.GetNColumns() ||
        column_end >= A.GetNColumns() || column_end < column_start ||
        nrows != row_end - row_start + 1 ||
        ncolumns != column_end - column_start + 1) {
        etl::exception e("Matrix::Slice arguments outside bounds", __FILE__,
                         __LINE__);
        throw e;
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
        etl::exception e("Matrix::Get indices out of bounds", __FILE__,
                         __LINE__);
        throw e;
    }
    return data[(row * ncolumns) + column];
}

void Matrix::Set(uint8_t row, uint8_t column, double value) {
    if (row >= nrows || column >= ncolumns) {  // uint8_t always > 0
        etl::exception e("Matrix::Set indices out of bounds", __FILE__,
                         __LINE__);
        throw e;
    }
    data[(row * ncolumns) + column] = value;
}

//  Test equality using relative difference. Additive tolerance allows
//  comparison with zero, but it's set arbitrarily.
bool Matrix::DoubleIsEqual(double a, double b) {
    if (isinf(a) || isinf(b)) {
        etl::exception e("Matrix::DoubleIsEqual argument isinf", __FILE__,
                         __LINE__);
        throw e;
    }
    if (isnan(a) || isnan(b)) {
        etl::exception e("Matrix::DoubleIsEqual argument is nan", __FILE__,
                         __LINE__);
        throw e;
    }
    if (a > b) {
        return std::fabs(a - b) <= std::fabs(a * EPSILON_MULT + EPSILON_ADD);
    }
    return std::fabs(a - b) <= std::fabs(b * EPSILON_MULT + EPSILON_ADD);
}

bool Matrix::IsEqual(const Matrix &A) const {
    if (!SameSize(A)) {
        etl::exception e("Matrix::IsEqual arguments' sizes don't match",
                         __FILE__, __LINE__);
        throw e;
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
        etl::exception e("Matrix::Transpose arguments' sizes don't match",
                         __FILE__, __LINE__);
        throw e;
    }
    for (uint8_t i = 0; i < nrows; i++) {
        for (uint8_t j = 0; j < ncolumns; j++) {
            Set(i, j, A.Get(j, i));
        }
    }
}

double Matrix::VectorNorm(const Matrix &A) {
    if (A.GetNColumns() != 1) {
        etl::exception e("Matrix::VectorNorm must be a column vector", __FILE__,
                         __LINE__);
        throw e;
    }
    double sum_of_squares = 0;
    for (uint8_t i = 0; i < A.GetNRows(); i++) {
        sum_of_squares += A.Get(i, 0) * A.Get(i, 0);
    }
    return sqrt(sum_of_squares);
}

void Matrix::Add(const Matrix &A, const Matrix &B) {
    if (!SameSize(A) || !SameSize(B)) {
        etl::exception e("Matrix::Add arguments' sizes don't match", __FILE__,
                         __LINE__);
        throw e;
    }
    for (uint8_t i = 0; i < nrows; i++) {
        for (uint8_t j = 0; j < ncolumns; j++) {
            Set(i, j, A.Get(i, j) + B.Get(i, j));
        }
    }
}

void Matrix::Subtract(const Matrix &A, const Matrix &B) {
    if (!SameSize(A) || !SameSize(B)) {
        etl::exception e("Matrix::Subtract arguments' sizes don't match",
                         __FILE__, __LINE__);
        throw e;
    }
    for (uint8_t i = 0; i < nrows; i++) {
        for (uint8_t j = 0; j < ncolumns; j++) {
            Set(i, j, A.Get(i, j) - B.Get(i, j));
        }
    }
}

void Matrix::Multiply(const Matrix &A, const Matrix &B) {
    if (A.GetNColumns() != B.GetNRows() || !SameNRows(A) || !SameNColumns(B)) {
        etl::exception e("Matrix::Multiply dimensions don't match", __FILE__,
                         __LINE__);
        throw e;
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
        etl::exception e("Matrix::MultiplyScalar arguments' sizes don't match",
                         __FILE__, __LINE__);
        throw e;
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
        etl::exception e(
            "Matrix::CrossProduct arguments or 'this' aren't column vectors",
            __FILE__, __LINE__);
        throw e;
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
        etl::exception e(
            "Matrix::CopyInto arguments exceed the bounds of 'this'", __FILE__,
            __LINE__);
        throw e;
    }
    for (uint8_t i = 0; i < A.GetNRows(); i++) {
        for (uint8_t j = 0; j < A.GetNColumns(); j++) {
            Set(i + row_start, j + column_start, A.Get(i, j));
        }
    }
}

void Matrix::Identity() {
    if (!IsSquare()) {
        etl::exception e("Matrix::Identity argument not square", __FILE__,
                         __LINE__);
        throw e;
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
        etl::exception e("Matrix::QuaternionNormalise not a 4x1 matrix",
                         __FILE__, __LINE__);
        throw e;
    }
    MultiplyScalar(q, 1 / VectorNorm(q));
}

// Only valid for quaternions represented as 4x1 matrices
void Matrix::RotationMatrixFromQuaternion(const Matrix &q) {
    if (nrows != 3 || ncolumns != 3 || q.nrows != 4 || q.ncolumns != 1) {
        etl::exception e(
            "Matrix::RotationMatrixFromQuaternion incorrect dimensions",
            __FILE__, __LINE__);
        throw e;
    }
    double q_normed_data[4][1];
    Matrix q_normed(q_normed_data);

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
        etl::exception e("Matrix::SkewSymmetricFill incorrect dimensions",
                         __FILE__, __LINE__);
        throw e;
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
        etl::exception e(
            "Matrix::ConcatenateHorizontally arguments' dimensions don't match",
            __FILE__, __LINE__);
        throw e;
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
        etl::exception e(
            "Matrix::ConcatenateVertically arguments' dimensions don't match",
            __FILE__, __LINE__);
        throw e;
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
        etl::exception e(
            "Matrix::AddRows arguments are outside matrix dimensions", __FILE__,
            __LINE__);
        throw e;
    }
    for (uint8_t i = 0; i < ncolumns; i++) {
        Set(row_to, i, Get(row_to, i) + (Get(row_from, i) * scale));
    }
}

void Matrix::MultiplyRow(uint8_t row, double scale) {
    if (row >= nrows) {
        etl::exception e(
            "Matrix::MultiplyRow arguments are outside matrix dimensions",
            __FILE__, __LINE__);
        throw e;
    }
    for (uint8_t i = 0; i < ncolumns; i++) {
        Set(row, i, Get(row, i) * scale);
    }
}

void Matrix::SwitchRows(uint8_t row_a, uint8_t row_b) {
    if (row_a >= nrows || row_b >= nrows) {
        etl::exception e(
            "Matrix::SwitchRows arguments are outside matrix dimensions",
            __FILE__, __LINE__);
        throw e;
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
        double max_element = std::fabs(Get(i, i));
        for (uint8_t j = i + 1; j < nrows; j++) {
            if (std::fabs(Get(j, i)) > max_element) {
                max_element = std::fabs(Get(j, i));
                max_row = j;
            }
        }
        if (DoubleIsEqual(max_element, 0)) {
            etl::exception e("Matrix::RowReduce linear system has no solution",
                             __FILE__, __LINE__);
            throw e;
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

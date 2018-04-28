#ifndef SRC_UTIL_MATRIX_H_
#define SRC_UTIL_MATRIX_H_

#include <external/etl/exception.h>
#include <src/util/data_types.h>

template <typename T>
class Matrix {
   public:
    template <uint8_t rows, uint8_t columns>
    explicit Matrix(T (&data)[rows][columns])
        : data(&data[0][0]), nrows(rows), ncolumns(columns) {}

    // TODO(rskew) implement copy constructor in matrix.cpp
    template <uint8_t rows, uint8_t columns>
    Matrix(const Matrix &A, T (&init_data)[rows][columns] = NULL) {
        if (init_data == NULL) {
            etl::exception e("Must pass in data matrix", __FILE__, __LINE__);
            throw e;
        }
        if (rows != A.GetNRows() || columns != A.GetNColumns()) {
            etl::exception e("Arguments sizes don't match", __FILE__, __LINE__);
            throw e;
        }
        nrows = A.GetNRows();
        ncolumns = A.GetNColumns();
        data = &init_data[0][0];
        for (uint8_t i = 0; i < nrows; i++) {
            for (uint8_t j = 0; j < ncolumns; j++) {
                data[(i * ncolumns) + j] = A.Get(i, j);
            }
        }
    }

    void Slice(uint8_t row_start, uint8_t row_end, uint8_t column_start,
               uint8_t column_end, const Matrix &A);

    uint8_t GetNRows() const;
    uint8_t GetNColumns() const;
    bool IsSquare() const;

    T Get(uint8_t row, uint8_t column) const;
    void Set(uint8_t row, uint8_t column, T value);

    static bool TemplateTypeValueIsEqual(T a, T b);
    bool IsEqual(const Matrix &A) const;
    bool SameSize(const Matrix &A) const;
    bool SameNRows(const Matrix &A) const;
    bool SameNColumns(const Matrix &A) const;

    void Transpose(const Matrix &A);
    static T VectorNorm(const Matrix &A);

    void Add(const Matrix &A, const Matrix &B);
    void Subtract(const Matrix &A, const Matrix &B);
    void Multiply(const Matrix &A, const Matrix &B);
    void MultiplyScalar(const Matrix &A, T scale);
    void CrossProduct(const Matrix &A, const Matrix &B);
    static T DotProduct(const Matrix &A, const Matrix &B);

    void Fill(T value);
    void CopyInto(uint8_t row_start, uint8_t column_start, const Matrix &A);
    void Identity();

    void SkewSymmetricFill(const Matrix &V);

    void ConcatenateHorizontally(const Matrix &A, const Matrix &B);
    void ConcatenateVertically(const Matrix &A, const Matrix &B);

    void AddRows(uint8_t row_to, uint8_t row_from, T scale);
    void MultiplyRow(uint8_t row, T scale);
    void SwitchRows(uint8_t row_a, uint8_t row_b);
    void RowReduce();

    void QuaternionNormalise(const Matrix &q);
    void RotationMatrixFromQuaternion(const Matrix &q);
    void QuaternionConjugate();
    void QuaternionProductCross(Matrix &a, Matrix &b);
    void QuaternionProductDot(Matrix &a, Matrix &b);

   private:
    T *data;
    uint8_t nrows;
    uint8_t ncolumns;
    static const T EPSILON_MULT = 1E-6;  //  Comparison ratio
    static const T EPSILON_ADD = 1E-4;   //  Comparison ratio
};

//////////////////////////////////////////////////////////
// Moved all from cpp so they can be templated

//#include <external/etl/exception.h>
#include <math.h>
//#include <src/util/matrix.h>

template <typename T>
void Matrix<T>::Slice(uint8_t row_start, uint8_t row_end, uint8_t column_start,
                      uint8_t column_end, const Matrix &A) {
    if (row_start >= A.GetNRows() || row_end >= A.GetNRows() ||
        row_end < row_start || column_start >= A.GetNColumns() ||
        column_end >= A.GetNColumns() || column_end < column_start ||
        nrows != row_end - row_start + 1 ||
        ncolumns != column_end - column_start + 1) {
        etl::exception e("Arguments outside bounds", __FILE__, __LINE__);
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

template <typename T>
uint8_t Matrix<T>::GetNRows() const {
    return nrows;
}

template <typename T>
uint8_t Matrix<T>::GetNColumns() const { return ncolumns; }

template <typename T>
bool Matrix<T>::IsSquare() const {
    return nrows == ncolumns;
}

template <typename T>
T Matrix<T>::Get(uint8_t row, uint8_t column) const {
    if (row >= nrows || column >= ncolumns) {  // uint8_t always > 0
        etl::exception e("Indices out of bounds", __FILE__, __LINE__);
        throw e;
    }
    return data[(row * ncolumns) + column];
}

template <typename T>
void Matrix<T>::Set(uint8_t row, uint8_t column, T value) {
    if (row >= nrows || column >= ncolumns) {  // uint8_t always > 0
        etl::exception e("Indices out of bounds", __FILE__, __LINE__);
        throw e;
    }
    data[(row * ncolumns) + column] = value;
}

//  Test equality using relative difference. Additive tolerance allows
//  comparison with zero, but it's set arbitrarily.
template <typename T>
bool Matrix<T>::TemplateTypeValueIsEqual(T a, T b) {
    if (isinf(a) || isinf(b)) {
        etl::exception e("Argument isinf", __FILE__, __LINE__);
        throw e;
    }
    if (isnan(a) || isnan(b)) {
        etl::exception e("Argument is nan", __FILE__, __LINE__);
        throw e;
    }
    if (a > b) {
        return std::fabs(a - b) <= std::fabs(a * EPSILON_MULT + EPSILON_ADD);
    }
    return std::fabs(a - b) <= std::fabs(b * EPSILON_MULT + EPSILON_ADD);
}

template <typename T>
bool Matrix<T>::IsEqual(const Matrix &A) const {
    if (!SameSize(A)) {
        etl::exception e("Arguments' sizes don't match", __FILE__, __LINE__);
        throw e;
    }
    for (uint8_t i = 0; i < nrows; i++) {
        for (uint8_t j = 0; j < ncolumns; j++) {
            if (!TemplateTypeValueIsEqual(Get(i, j), A.Get(i, j))) {
                return false;
            }
        }
    }
    return true;
}

template <typename T>
bool Matrix<T>::SameSize(const Matrix &A) const {
    return SameNRows(A) && SameNColumns(A);
}

template <typename T>
bool Matrix<T>::SameNRows(const Matrix &A) const {
    return nrows == A.GetNRows();
}

template <typename T>
bool Matrix<T>::SameNColumns(const Matrix &A) const {
    return ncolumns == A.GetNColumns();
}

template <typename T>
void Matrix<T>::Transpose(const Matrix &A) {
    if (nrows != A.GetNColumns() || ncolumns != A.GetNRows()) {
        etl::exception e("Arguments' sizes don't match", __FILE__, __LINE__);
        throw e;
    }
    for (uint8_t i = 0; i < nrows; i++) {
        for (uint8_t j = 0; j < ncolumns; j++) {
            Set(i, j, A.Get(j, i));
        }
    }
}

template <typename T>
T Matrix<T>::VectorNorm(const Matrix &A) {
    if (A.GetNColumns() != 1) {
        etl::exception e("Must be a column vector", __FILE__, __LINE__);
        throw e;
    }
    T sum_of_squares = 0;
    for (uint8_t i = 0; i < A.GetNRows(); i++) {
        sum_of_squares += A.Get(i, 0) * A.Get(i, 0);
    }
    return sqrt(sum_of_squares);
}

template <typename T>
void Matrix<T>::Add(const Matrix &A, const Matrix &B) {
    if (!SameSize(A) || !SameSize(B)) {
        etl::exception e("Arguments' sizes don't match", __FILE__, __LINE__);
        throw e;
    }
    for (uint8_t i = 0; i < nrows; i++) {
        for (uint8_t j = 0; j < ncolumns; j++) {
            Set(i, j, A.Get(i, j) + B.Get(i, j));
        }
    }
}

template <typename T>
void Matrix<T>::Subtract(const Matrix &A, const Matrix &B) {
    if (!SameSize(A) || !SameSize(B)) {
        etl::exception e("Arguments' sizes don't match", __FILE__, __LINE__);
        throw e;
    }
    for (uint8_t i = 0; i < nrows; i++) {
        for (uint8_t j = 0; j < ncolumns; j++) {
            Set(i, j, A.Get(i, j) - B.Get(i, j));
        }
    }
}

template <typename T>
void Matrix<T>::Multiply(const Matrix &A, const Matrix &B) {
    if (A.GetNColumns() != B.GetNRows() || !SameNRows(A) || !SameNColumns(B)) {
        etl::exception e("Dimensions don't match", __FILE__, __LINE__);
        throw e;
    }
    for (uint8_t i = 0; i < nrows; i++) {
        for (uint8_t j = 0; j < ncolumns; j++) {
            T n = 0;
            for (uint8_t k = 0; k < A.GetNColumns(); k++) {
                n += A.Get(i, k) * B.Get(k, j);
            }
            Set(i, j, n);
        }
    }
}

template <typename T>
void Matrix<T>::MultiplyScalar(const Matrix &A, T scale) {
    if (!SameSize(A)) {
        etl::exception e("Arguments' sizes don't match", __FILE__, __LINE__);
        throw e;
    }
    for (uint8_t i = 0; i < nrows; i++) {
        for (uint8_t j = 0; j < ncolumns; j++) {
            Set(i, j, A.Get(i, j) * scale);
        }
    }
}

// Only valid for vectors represented as 3x1 matrices
template <typename T>
void Matrix<T>::CrossProduct(const Matrix &A, const Matrix &B) {
    if (A.GetNRows() != 3 || A.GetNColumns() != 1 || !B.SameSize(A) ||
        !SameSize(A)) {
        etl::exception e("Arguments or 'this' aren't column vectors", __FILE__,
                         __LINE__);
        throw e;
    }
    Set(0, 0, A.Get(1, 0) * B.Get(2, 0) - A.Get(2, 0) * B.Get(1, 0));
    Set(1, 0, A.Get(2, 0) * B.Get(0, 0) - A.Get(0, 0) * B.Get(2, 0));
    Set(2, 0, A.Get(0, 0) * B.Get(1, 0) - A.Get(1, 0) * B.Get(0, 0));
}

template <typename T>
void Matrix<T>::Fill(T value) {
    for (uint8_t i = 0; i < nrows; i++) {
        for (uint8_t j = 0; j < ncolumns; j++) {
            Set(i, j, value);
        }
    }
}

template <typename T>
void Matrix<T>::CopyInto(uint8_t row_start, uint8_t column_start,
                         const Matrix &A) {
    if (nrows < A.GetNRows() + row_start ||
        ncolumns < A.GetNColumns() + column_start) {
        etl::exception e("Arguments exceed the bounds of 'this'", __FILE__,
                         __LINE__);
        throw e;
    }
    for (uint8_t i = 0; i < A.GetNRows(); i++) {
        for (uint8_t j = 0; j < A.GetNColumns(); j++) {
            Set(i + row_start, j + column_start, A.Get(i, j));
        }
    }
}

template <typename T>
void Matrix<T>::Identity() {
    if (!IsSquare()) {
        etl::exception e("Argument not square", __FILE__, __LINE__);
        throw e;
    }
    Fill(0);

    for (uint8_t i = 0; i < nrows; i++) {
        Set(i, i, 1);
    }
}

// Only valid for quaternions represented as 4x1 matrices
template <typename T>
void Matrix<T>::QuaternionNormalise(const Matrix &q) {
    // TODO(rskew) break this out into a standalone quaternion library
    if (nrows != 4 || ncolumns != 1 || q.GetNRows() != 4 ||
        q.GetNColumns() != 1) {
        etl::exception e("Not a 4x1 matrix", __FILE__, __LINE__);
        throw e;
    }
    MultiplyScalar(q, 1 / VectorNorm(q));
}

// Only valid for quaternions represented as 4x1 matrices
template <typename T>
void Matrix<T>::RotationMatrixFromQuaternion(const Matrix &q) {
    if (nrows != 3 || ncolumns != 3 || q.nrows != 4 || q.ncolumns != 1) {
        etl::exception e("Incorrect dimensions", __FILE__, __LINE__);
        throw e;
    }
    T q_normed_data[4][1];
    Matrix q_normed(q_normed_data);

    q_normed.QuaternionNormalise(q);

    T qx = q_normed.Get(0, 0);
    T qy = q_normed.Get(1, 0);
    T qz = q_normed.Get(2, 0);
    T qw = q_normed.Get(3, 0);

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
template <typename T>
void Matrix<T>::SkewSymmetricFill(const Matrix &A) {
    if (A.GetNColumns() != 1 || A.GetNRows() != 3 || nrows != 3 ||
        ncolumns != 3) {
        etl::exception e("Incorrect dimensions", __FILE__, __LINE__);
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

template <typename T>
void Matrix<T>::ConcatenateHorizontally(const Matrix &A, const Matrix &B) {
    if (!SameNRows(A) || !SameNRows(B) ||
        ncolumns != A.GetNColumns() + B.GetNColumns()) {
        etl::exception e("Arguments' dimensions don't match", __FILE__,
                         __LINE__);
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

template <typename T>
void Matrix<T>::ConcatenateVertically(const Matrix &A, const Matrix &B) {
    if (!SameNColumns(A) || !SameNColumns(B) ||
        nrows != A.GetNRows() + B.GetNRows()) {
        etl::exception e("Arguments' dimensions don't match", __FILE__,
                         __LINE__);
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

template <typename T>
void Matrix<T>::AddRows(uint8_t row_to, uint8_t row_from, T scale) {
    if (row_to >= nrows || row_from >= nrows) {
        etl::exception e("Arguments are outside matrix dimensions", __FILE__,
                         __LINE__);
        throw e;
    }
    for (uint8_t i = 0; i < ncolumns; i++) {
        Set(row_to, i, Get(row_to, i) + (Get(row_from, i) * scale));
    }
}

template <typename T>
void Matrix<T>::MultiplyRow(uint8_t row, T scale) {
    if (row >= nrows) {
        etl::exception e("Arguments are outside matrix dimensions", __FILE__,
                         __LINE__);
        throw e;
    }
    for (uint8_t i = 0; i < ncolumns; i++) {
        Set(row, i, Get(row, i) * scale);
    }
}

template <typename T>
void Matrix<T>::SwitchRows(uint8_t row_a, uint8_t row_b) {
    if (row_a >= nrows || row_b >= nrows) {
        etl::exception e("Arguments are outside matrix dimensions", __FILE__,
                         __LINE__);
        throw e;
    }
    for (uint8_t i = 0; i < ncolumns; i++) {
        T temp = Get(row_a, i);
        Set(row_a, i, Get(row_b, i));
        Set(row_b, i, temp);
    }
}

template <typename T>
void Matrix<T>::RowReduce() {
    uint8_t square;

    if (nrows < ncolumns) {
        square = nrows;
    } else {
        square = ncolumns;
    }

    for (uint8_t i = 0; i < square; i++) {
        uint8_t max_row = i;
        T max_element = std::fabs(Get(i, i));
        for (uint8_t j = i + 1; j < nrows; j++) {
            if (std::fabs(Get(j, i)) > max_element) {
                max_element = std::fabs(Get(j, i));
                max_row = j;
            }
        }
        if (TemplateTypeValueIsEqual(max_element, 0)) {
            etl::exception e("Linear system has no solution", __FILE__,
                             __LINE__);
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
template <typename T>
void Matrix<T>::QuaternionConjugate() {
    Set(1, 0, -1 * Get(1, 0));
    Set(2, 0, -1 * Get(2, 0));
    Set(3, 0, -1 * Get(3, 0));
    return;
}

template <typename T>
void Matrix<T>::QuaternionProductCross(Matrix &a, Matrix &b) {
    T x = a.Get(0, 0) * b.Get(0, 0) - a.Get(1, 0) * b.Get(1, 0) -
               a.Get(2, 0) * b.Get(2, 0) - a.Get(3, 0) * b.Get(3, 0);
    T y = a.Get(0, 0) * b.Get(1, 0) + a.Get(1, 0) * b.Get(0, 0) -
               a.Get(2, 0) * b.Get(3, 0) + a.Get(3, 0) * b.Get(2, 0);
    T z = a.Get(0, 0) * b.Get(2, 0) + a.Get(2, 0) * b.Get(0, 0) +
               a.Get(1, 0) * b.Get(3, 0) - a.Get(3, 0) * b.Get(1, 0);
    T w = a.Get(0, 0) * b.Get(3, 0) - a.Get(1, 0) * b.Get(2, 0) +
               a.Get(2, 0) * b.Get(1, 0) + a.Get(3, 0) * b.Get(0, 0);
    Set(0, 0, x);
    Set(1, 0, y);
    Set(2, 0, z);
    Set(3, 0, w);
}

template <typename T>
void Matrix<T>::QuaternionProductDot(Matrix &a, Matrix &b) {
    QuaternionProductCross(b, a);
}

template <typename T>
T Matrix<T>::DotProduct(const Matrix &A, const Matrix &B) {
    T dot = 0;

    for (uint8_t i = 0; i < 3; i++) {
        dot += A.Get(i, 0) * B.Get(i, 0);
    }

    return dot;
}

#endif  // SRC_UTIL_MATRIX_H_

#ifndef SRC_UTIL_MATRIX_H_
#define SRC_UTIL_MATRIX_H_

#include <src/util/data_types.h>
#include <src/util/msp_exception.h>

#define NewStackMatrixMacro(name, n_rows, n_cols) \
    double name##_data[n_rows][n_cols];           \
    Matrix name(name##_data);

class Matrix {
   public:
    template <uint8_t rows, uint8_t columns>
    explicit Matrix(double (&data)[rows][columns])
        : data(&data[0][0]), nrows(rows), ncolumns(columns) {}

    // To instantiate a constant matrix, use this constructor (constant data),
    // a dummy 2D array of mutable data, but declare the object as 'const'
    // to disallow any modification of the inner array.
    template <uint8_t rows, uint8_t columns>
    explicit Matrix(const double (&const_data)[rows][columns],
                    double (&data)[rows][columns])
        : data(&data[0][0]), nrows(rows), ncolumns(columns) {
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < columns; j++) {
                data[i][j] = const_data[i][j];
            }
        }
    }

    template <uint8_t rows, uint8_t columns>
    Matrix(const Matrix &A, double (&init_data)[rows][columns] = NULL) {
        if (rows != A.GetNRows() || columns != A.GetNColumns()) {
            throw MspException(
                "Matrix::Matrix(const Matrix &A ...) arguments sizes don't "
                "match",
                kMatrixConstructSizeMismatchFail, __FILE__, __LINE__);
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

    void CopySlice(uint8_t row_start, uint8_t row_end, uint8_t column_start,
                   uint8_t column_end, const Matrix &A);

    uint8_t GetNRows() const;
    uint8_t GetNColumns() const;
    bool IsSquare() const;

    double Get(uint8_t row, uint8_t column) const;
    void Set(uint8_t row, uint8_t column, double value);

    static bool DoubleIsEqual(double a, double b);
    bool IsEqual(const Matrix &A) const;
    bool SameSize(const Matrix &A) const;
    bool SameNRows(const Matrix &A) const;
    bool SameNColumns(const Matrix &A) const;

    void Transpose(const Matrix &A);
    static double VectorNorm(const Matrix &A);

    void Add(const Matrix &A, const Matrix &B);
    void Subtract(const Matrix &A, const Matrix &B);
    void Multiply(const Matrix &A, const Matrix &B);
    void MultiplyScalar(const Matrix &A, double scale);
    void CrossProduct(const Matrix &A, const Matrix &B);
    static double DotProduct(const Matrix &A, const Matrix &B);

    void Fill(double value);
    void CopyInto(uint8_t row_start, uint8_t column_start, const Matrix &A);
    void Identity();

    void SkewSymmetricFill(const Matrix &V);

    void ConcatenateHorizontally(const Matrix &A, const Matrix &B);
    void ConcatenateVertically(const Matrix &A, const Matrix &B);

    void AddRows(uint8_t row_to, uint8_t row_from, double scale);
    void MultiplyRow(uint8_t row, double scale);
    void SwitchRows(uint8_t row_a, uint8_t row_b);
    void RowReduce();

    template <uint8_t size>
    void InvertMatrix(const Matrix &A) {
        NewStackMatrixMacro(augmented, size, 2 * size);
        augmented.CopyInto(0, 0, A);

        NewStackMatrixMacro(identity, size, size);
        identity.Identity();
        augmented.CopyInto(0, size, identity);

        augmented.RowReduce();

        CopySlice(0, size - 1, size, 2 * size - 1, augmented);
    }

    void QuaternionNormalise(const Matrix &q);
    void RotationMatrixFromQuaternion(const Matrix &q);
    void QuaternionConjugate();
    void QuaternionProductCross(Matrix &a, Matrix &b);
    void QuaternionProductDot(Matrix &a, Matrix &b);

   private:
    double *data;
    uint8_t nrows;
    uint8_t ncolumns;
    static constexpr double EPSILON_MULT = 1E-6;  //  Comparison ratio
    static constexpr double EPSILON_ADD = 1E-4;   //  Comparison ratio
};

#endif  // SRC_UTIL_MATRIX_H_

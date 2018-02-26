#ifndef SRC_UTIL_MATRIX_H_
#define SRC_UTIL_MATRIX_H_

#include <src/util/data_types.h>

class Matrix {
   public:
    template <uint8_t rows, uint8_t columns>
    explicit Matrix(double (&data)[rows][columns])
        : data(&data[0][0]), nrows(rows), ncolumns(columns) {}

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
    void Add(const Matrix &A, const Matrix &B);
    void Subtract(const Matrix &A, const Matrix &B);
    void Multiply(const Matrix &A, const Matrix &B);
    void MultiplyScalar(const Matrix &A, double scale);

    void Fill(double value);
    void Identity();

    void ConcatenateHorizontally(const Matrix &A, const Matrix &B);
    void ConcatenateVertically(const Matrix &A, const Matrix &B);

    void AddRows(uint8_t row_to, uint8_t row_from, double scale);
    void MultiplyRow(uint8_t row, double scale);
    void SwitchRows(uint8_t row_a, uint8_t row_b);

    void RowReduce();

   private:
    double *data;
    uint8_t nrows;
    uint8_t ncolumns;

    static const double EPSILON = 1E-6; //  Comparison ratio
};

#endif  // SRC_UTIL_MATRIX_H_

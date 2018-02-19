#include <cmath>
#include <src/util/matrix.h>

uint8_t Matrix::GetNRows() const { return nrows; }

uint8_t Matrix::GetNColumns() const { return ncolumns; }

bool Matrix::IsSquare() const { return nrows == ncolumns; }

double Matrix::Get(uint8_t row, uint8_t column) const {
    if (row >= nrows || column >= ncolumns) {  // uint8_t always > 0
        return 0;  // throw exception
    }
    return data[(row * ncolumns) + column];
}

void Matrix::Set(uint8_t row, uint8_t column, double value) {
    if (row >= nrows || column >= ncolumns) {  // uint8_t always > 0
        return;  // throw exception
    }
    data[(row * ncolumns) + column] = value;
}

//  Test equality using relative difference. Will fail comparing anything
//  non-zero with zero
bool Matrix::DoubleIsEqual(double a, double b) {
    if (isinf(a) && isinf(b) && ((a < 0) == (b < 0))) {
        return true;
    }
    if (isnan(a) || isnan(b)) {
        return false;  // throw exception
    }
    if (a > b) {
        return std::fabs(a - b) <= std::fabs(a * EPSILON);
    }
    return std::fabs(a - b) <= std::fabs(b * EPSILON);
}

bool Matrix::IsEqual(const Matrix &A) const {
    if (!SameSize(A)) {
        return false;  // throw exception
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

bool Matrix::SameNRows(const Matrix &A) const {
    return nrows == A.GetNRows();
}

bool Matrix::SameNColumns(const Matrix &A) const {
    return ncolumns == A.GetNColumns();
}

void Matrix::Transpose(const Matrix &A) {
    if (nrows != A.GetNColumns() || ncolumns != A.GetNRows()) {
        return;  // throw exception
    }
    for (uint8_t i = 0; i < nrows; i++) {
        for (uint8_t j = 0; j < ncolumns; j++) {
            Set(i, j, A.Get(j, i));
        }
    }
}

void Matrix::Add(const Matrix &A, const Matrix &B) {
    if (!SameSize(A) || !SameSize(B)) {
        return;  // throw exception
    }
    for (uint8_t i = 0; i < nrows; i++) {
        for (uint8_t j = 0; j < ncolumns; j++) {
            Set(i, j, A.Get(i, j) + B.Get(i, j));
        }
    }
}

void Matrix::Subtract(const Matrix &A, const Matrix &B) {
    if (!SameSize(A) || !SameSize(B)) {
        return;  // throw exception
    }
    for (uint8_t i = 0; i < nrows; i++) {
        for (uint8_t j = 0; j < ncolumns; j++) {
            Set(i, j, A.Get(i, j) - B.Get(i, j));
        }
    }
}

void Matrix::Multiply(const Matrix &A, const Matrix &B) {
    if (A.GetNColumns() != B.GetNRows() || !SameNRows(A) ||
        !SameNColumns(B)) {
        return;  // throw exception
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
        return; // throw exception
    }
    for (uint8_t i = 0; i < nrows; i++) {
        for (uint8_t j = 0; j < ncolumns; j++) {
            Set(i, j, A.Get(i, j) * scale);
        }
    }
}

void Matrix::Fill(double value) {
    for (uint8_t i = 0; i < nrows; i++) {
        for (uint8_t j = 0; j < ncolumns; j++) {
            Set(i, j, value);
        }
    }
}

void Matrix::Identity() {
    if (!IsSquare()) {
        return;  // throw exception
    }
    Fill(0);

    for (uint8_t i = 0; i < nrows; i++) {
        Set(i, i, 1);
    }
}

void Matrix::ConcatenateHorizontally(const Matrix &A, const Matrix &B) {
    if (!SameNRows(A) || !SameNRows(B) ||
        ncolumns != A.GetNColumns() + B.GetNColumns()) {
        return;  // throw exception
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
        return;  // throw exception
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
    for (uint8_t i = 0; i < ncolumns; i++) {
        Set(row_to, i, Get(row_to, i) + (Get(row_from, i) * scale));
    }
}

void Matrix::MultiplyRow(uint8_t row, double scale) {
    for (uint8_t i = 0; i < ncolumns; i++) {
        Set(row, i, Get(row, i) * scale);
    }
}

void Matrix::SwitchRows(uint8_t row_a, uint8_t row_b) {
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
            return;
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

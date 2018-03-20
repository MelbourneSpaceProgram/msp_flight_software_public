#include <src/util/data_types.h>
#include <src/util/matrix.h>
#include <test_runners/matrix_tests.h>
#include <test_runners/unity.h>

void TestGetNRows() {
    double m42_data[4][2];
    Matrix m42(m42_data);

    TEST_ASSERT_EQUAL_UINT8(4, m42.GetNRows());
}

void TestGetNColumns() {
    double m42_data[4][2];
    Matrix m42(m42_data);

    TEST_ASSERT_EQUAL_UINT8(2, m42.GetNColumns());
}

void TestIsSquare() {
    double m22_data[2][2];
    Matrix m22(m22_data);
    double m42_data[4][2];
    Matrix m42(m42_data);

    TEST_ASSERT_TRUE(m22.IsSquare());
    TEST_ASSERT_FALSE(m42.IsSquare());
}

void TestGet() {
    double m22_data[2][2] = {{0, -1E-100}, {1.234E100, 4}};
    Matrix m22(m22_data);

    TEST_ASSERT_EQUAL_DOUBLE(0, m22.Get(0, 0));
    TEST_ASSERT_EQUAL_DOUBLE(-1E-100, m22.Get(0, 1));
    TEST_ASSERT_EQUAL_DOUBLE(1.234E100, m22.Get(1, 0));
    TEST_ASSERT_EQUAL_DOUBLE(4, m22.Get(1, 1));
}

void TestSet() {
    double m22_data[2][2] = {{1, 2}, {3, 4}};
    Matrix m22(m22_data);

    m22.Set(0, 0, 0);
    m22.Set(0, 1, -1E-100);
    m22.Set(1, 0, 1.234E100);
    m22.Set(1, 1, 4);

    TEST_ASSERT_EQUAL_DOUBLE(0, m22.Get(0, 0));
    TEST_ASSERT_EQUAL_DOUBLE(-1E-100, m22.Get(0, 1));
    TEST_ASSERT_EQUAL_DOUBLE(1.234E100, m22.Get(1, 0));
    TEST_ASSERT_EQUAL_DOUBLE(4, m22.Get(1, 1));
}

void TestDoubleIsEqual() {
    TEST_ASSERT_TRUE(Matrix::DoubleIsEqual(0, -0));
    TEST_ASSERT_TRUE(Matrix::DoubleIsEqual(0, 1E-100));
    TEST_ASSERT_FALSE(Matrix::DoubleIsEqual(1, 2));
    TEST_ASSERT_TRUE(Matrix::DoubleIsEqual(-1, -1 + 1E-5));
    TEST_ASSERT_FALSE(Matrix::DoubleIsEqual(-1, 1 + 1E-7));
    TEST_ASSERT_TRUE(Matrix::DoubleIsEqual(-1, -1 + 1E-7));
}

void TestIsEqual() {
    double m22_data[2][2] = {{0, -1E-100}, {1.234E100, 4}};
    Matrix m22(m22_data);
    double m22_2_data[2][2] = {{0, -1E-100}, {1.234E100, 4}};
    Matrix m22_2(m22_2_data);
    double m22_3_data[2][2] = {{0, -1E-100}, {1.234E100, 4.1}};
    Matrix m22_3(m22_3_data);

    TEST_ASSERT_TRUE(m22.IsEqual(m22_2));
    TEST_ASSERT_FALSE(m22.IsEqual(m22_3));
}

void TestSameSize() {
    double m22_data[2][2];
    Matrix m22(m22_data);
    double m22_2_data[2][2];
    Matrix m22_2(m22_2_data);
    double m23_data[2][3];
    Matrix m23(m23_data);

    TEST_ASSERT_TRUE(m22.SameSize(m22_2));
    TEST_ASSERT_FALSE(m22.SameSize(m23));
}

void TestSameNRows() {
    double m22_data[2][2];
    Matrix m22(m22_data);
    double m22_2_data[2][2];
    Matrix m22_2(m22_2_data);
    double m32_data[3][2];
    Matrix m32(m32_data);

    TEST_ASSERT_TRUE(m22.SameSize(m22_2));
    TEST_ASSERT_FALSE(m22.SameSize(m32));
}

void TestSameNColumns() {
    double m22_data[2][2];
    Matrix m22(m22_data);
    double m22_2_data[2][2];
    Matrix m22_2(m22_2_data);
    double m23_data[2][3];
    Matrix m23(m23_data);

    TEST_ASSERT_TRUE(m22.SameSize(m22_2));
    TEST_ASSERT_FALSE(m22.SameSize(m23));
}

void TestTranspose() {
    double m22_data[2][2] = {{1, 2}, {3, 4}};
    Matrix m22(m22_data);
    double m22_2_data[2][2];
    Matrix m22_2(m22_2_data);
    double m22_expected_data[2][2] = {{1, 3}, {2, 4}};
    Matrix m22_expected(m22_expected_data);
    m22_2.Transpose(m22);
    TEST_ASSERT_TRUE(m22_2.IsEqual(m22_expected));
}

void TestAdd() {
    double m22_data[2][2];
    Matrix m22(m22_data);
    double m22_2_data[2][2] = {{5, 6}, {7, 8}};
    Matrix m22_2(m22_2_data);
    double m22_3_data[2][2] = {{1, 2}, {3, 4}};
    Matrix m22_3(m22_3_data);
    double m22_expected_data[2][2] = {{6, 8}, {10, 12}};
    Matrix m22_expected(m22_expected_data);

    m22.Add(m22_2, m22_3);

    TEST_ASSERT_TRUE(m22.IsEqual(m22_expected));
}

void TestSubtract() {
    double m22_data[2][2];
    Matrix m22(m22_data);
    double m22_2_data[2][2] = {{4, 3}, {2, 1}};
    Matrix m22_2(m22_2_data);
    double m22_3_data[2][2] = {{1, 2}, {3, 4}};
    Matrix m22_3(m22_3_data);
    double m22_expected_data[2][2] = {{-3, -1}, {1, 3}};
    Matrix m22_expected(m22_expected_data);

    m22.Subtract(m22_3, m22_2);

    TEST_ASSERT_TRUE(m22.IsEqual(m22_expected));
}

void TestMultiply() {
    double m32_data[3][2] = {{1.23, 2}, {0, 4}, {5, -6}};
    Matrix m32(m32_data);
    double m23_data[2][3] = {{-1.23, 2, 0}, {4, 5E-2, 6}};
    Matrix m23(m23_data);
    double m33_data[3][3];
    Matrix m33(m33_data);
    double m33_expected_data[3][3] = {
        {6.4871, 2.56, 12}, {16, 0.2, 24}, {-30.15, 9.7, -36}};
    Matrix m33_expected(m33_expected_data);

    m33.Multiply(m32, m23);

    TEST_ASSERT_TRUE(m33.IsEqual(m33_expected));
}

void TestMultiplyScalar() {
    double scale = -0.3456;
    double m22_data[2][2] = {{1, -2}, {1e-10, 0}};
    Matrix m22(m22_data);
    double m22_expected_data[2][2] = {{(1 * scale), (-2 * scale)},
                                      {(1e-10 * scale), (0 * scale)}};
    Matrix m22_expected(m22_expected_data);

    m22.MultiplyScalar(m22, scale);

    TEST_ASSERT_TRUE(m22.IsEqual(m22_expected));
}

void TestFill() {
    double m22_data[2][2] = {{1, 2}, {3, 4}};
    Matrix m22(m22_data);
    double m22_expected_data[2][2] = {{1.23, 1.23}, {1.23, 1.23}};
    Matrix m22_expected(m22_expected_data);

    m22.Fill(1.23);

    TEST_ASSERT_TRUE(m22.IsEqual(m22_expected));
}

void TestIdentity() {
    double m33_data[3][3];
    Matrix m33(m33_data);
    double m33_expected_data[3][3] = {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}};
    Matrix m33_expected(m33_expected_data);

    m33.Identity();

    TEST_ASSERT_TRUE(m33.IsEqual(m33_expected));
}

void TestConcatenateHorizontally() {
    double m22_data[2][2] = {{1, 2}, {3, 4}};
    Matrix m22(m22_data);
    double m23_data[2][3] = {{5, 6, 7}, {8, 9, 10}};
    Matrix m23(m23_data);
    double m25_data[2][5];
    Matrix m25(m25_data);
    double m25_expected_data[2][5] = {{1, 2, 5, 6, 7}, {3, 4, 8, 9, 10}};
    Matrix m25_expected(m25_expected_data);

    m25.ConcatenateHorizontally(m22, m23);

    TEST_ASSERT_TRUE(m25.IsEqual(m25_expected));
}

void TestConcatenateVertically() {
    double m22_data[2][2] = {{1, 2}, {3, 4}};
    Matrix m22(m22_data);
    double m32_data[3][2] = {{5, 6}, {7, 8}, {9, 10}};
    Matrix m32(m32_data);
    double m52_data[5][2];
    Matrix m52(m52_data);
    double m52_expected_data[5][2] = {{1, 2}, {3, 4}, {5, 6}, {7, 8}, {9, 10}};
    Matrix m52_expected(m52_expected_data);

    m52.ConcatenateVertically(m22, m32);

    TEST_ASSERT_TRUE(m52.IsEqual(m52_expected));
}

void TestAddRows() {
    double scale = -0.123;
    double m22_data[2][2] = {{1, 2}, {3, 4}};
    Matrix m22(m22_data);
    double m22_expected_data[2][2] = {{1 + (3 * scale), 2 + (4 * scale)},
                                      {3, 4}};
    Matrix m22_expected(m22_expected_data);

    m22.AddRows(0, 1, scale);

    TEST_ASSERT_TRUE(m22.IsEqual(m22_expected));
}

void TestMultiplyRow() {
    double scale = -0.123;
    double m22_data[2][2] = {{1, 2}, {3, 4}};
    Matrix m22(m22_data);
    double m22_expected_data[2][2] = {{1, 2}, {(3 * scale), (4 * scale)}};
    Matrix m22_expected(m22_expected_data);

    m22.MultiplyRow(1, scale);

    TEST_ASSERT_TRUE(m22.IsEqual(m22_expected));
}

void TestSwitchRows() {
    double m22_data[2][2] = {{1, 2}, {3, 4}};
    Matrix m22(m22_data);
    double m22_expected_data[2][2] = {{3, 4}, {1, 2}};
    Matrix m22_expected(m22_expected_data);

    m22.SwitchRows(0, 1);

    TEST_ASSERT_TRUE(m22.IsEqual(m22_expected));
}

void TestRowReduce() {
    double m34_data[3][4] = {{1, 2, 3, 1}, {1, 2, -3, 2}, {1, -2, 3, 3}};
    Matrix m34(m34_data);
    double m34_expected_data[3][4] = {
        {1, 0, 0, 5.0 / 2}, {0, 1, 0, -1.0 / 2}, {0, 0, 1, -1.0 / 6}};
    Matrix m34_expected(m34_expected_data);

    m34.RowReduce();

    TEST_ASSERT_TRUE(m34.IsEqual(m34_expected));
}

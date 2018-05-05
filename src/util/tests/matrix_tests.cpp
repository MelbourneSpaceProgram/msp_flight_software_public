#include <external/etl/exception.h>
#include <math.h>
#include <src/util/data_types.h>
#include <src/util/matrix.h>
#include <test_runners/matrix_tests.h>
#include <test_runners/unity.h>

void TestDotProduct() {
    // Tests Column vector input
    double aData[3][1] = {{2}, {3}, {4}};
    double bData[3][1] = {{2}, {3}, {4}};
    Matrix a(aData), b(bData);
    double c = Matrix::DotProduct(a, b);
    TEST_ASSERT_EQUAL_DOUBLE(c, 29);
    // tests row vector input
}

void TestQuaternionConjugate() {
    double q_data[4][1] = {{1.0}, {2}, {3}, {4}};
    Matrix q(q_data);
    q.QuaternionConjugate();
    TEST_ASSERT_EQUAL_DOUBLE(1.0, q.Get(0, 0));
    TEST_ASSERT_EQUAL_DOUBLE(-2.0, q.Get(1, 0));
    TEST_ASSERT_EQUAL_DOUBLE(-3.0, q.Get(2, 0));
    TEST_ASSERT_EQUAL_DOUBLE(-4.0, q.Get(3, 0));
}
void TestQuaternionDotProduct() {
    double q1[4][1] = {{0.060346}, {0.8606}, {0.23381}, {0.37978}};
    double q2[4][1] = {{-0.53115}, {-0.58631}, {-0.92527}, {0.32673}};

    Matrix qa(q1), qb(q2), qc(q1);
    qc.QuaternionProductDot(qa, qb);
    TEST_ASSERT_DOUBLE_WITHIN(0.001, 0.56477, qc.Get(0, 0));
    TEST_ASSERT_DOUBLE_WITHIN(0.001, -0.064697, qc.Get(1, 0));
    TEST_ASSERT_DOUBLE_WITHIN(0.001, -0.68388, qc.Get(2, 0));
    TEST_ASSERT_DOUBLE_WITHIN(0.001, -0.84121, qc.Get(3, 0));
}

void TestQuaternionCrossProduct() {
    double q1[4][1] = {{0.060346}, {0.8606}, {0.23381}, {0.37978}};
    double q2[4][1] = {{-0.53115}, {-0.58631}, {-0.92527}, {0.32673}};

    Matrix qa(q1), qb(q2), qc(q1);
    qc.QuaternionProductCross(qa, qb);
    TEST_ASSERT_DOUBLE_WITHIN(0.001, 0.56477, qc.Get(0, 0));
    TEST_ASSERT_DOUBLE_WITHIN(0.001, -0.92028, qc.Get(1, 0));
    TEST_ASSERT_DOUBLE_WITHIN(0.001, 0.32383, qc.Get(2, 0));
    TEST_ASSERT_DOUBLE_WITHIN(0.001, 0.4772, qc.Get(3, 0));
}

void TestCopyConstructor() {
    double m22_A_data[2][2] = {{1, 2}, {3, 4}};
    Matrix m22_A(m22_A_data);
    double m22_B_data[2][2];

    Matrix m22_B(m22_A, m22_B_data);

    TEST_ASSERT_TRUE(m22_A.IsEqual(m22_B));

    bool failed = false;
    try {
        double m32_data[3][2];
        Matrix m32(m22_A, m32_data);
    } catch (const etl::exception& e) {
        failed = true;
    }
    TEST_ASSERT_TRUE(failed);
}

void TestSlice() {
    double m44_data[4][4] = {
        {1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12}, {13, 14, 15, 16}};
    Matrix m44(m44_data);

    double m22_slice_data[2][2];
    Matrix m22_slice(m22_slice_data);
    m22_slice.Fill(0);
    m22_slice.Slice(1, 2, 1, 2, m44);

    double m22_expected_data[2][2] = {{6, 7}, {10, 11}};
    Matrix m22_expected(m22_expected_data);

    TEST_ASSERT_TRUE(m22_expected.IsEqual(m22_slice));

    bool failed = false;
    try {
        m22_slice.Get(2, 2);
    } catch (const etl::exception& e) {
        failed = true;
    }
    TEST_ASSERT_TRUE(failed);
}

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

    bool failed = false;
    try {
        m22.Get(255, 111);
    } catch (const etl::exception& e) {
        failed = true;
    }
    TEST_ASSERT_TRUE(failed);
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

    bool failed = false;
    try {
        m22.Set(255, 111, 0);
    } catch (const etl::exception& e) {
        failed = true;
    }
    TEST_ASSERT_TRUE(failed);
}

void TestDoubleIsEqual() {
    TEST_ASSERT_TRUE(Matrix::DoubleIsEqual(0, -0));
    TEST_ASSERT_TRUE(Matrix::DoubleIsEqual(0, 1E-100));
    TEST_ASSERT_FALSE(Matrix::DoubleIsEqual(1, 2));
    TEST_ASSERT_TRUE(Matrix::DoubleIsEqual(-1, -1 + 1E-5));
    TEST_ASSERT_FALSE(Matrix::DoubleIsEqual(-1, 1 + 1E-7));
    TEST_ASSERT_TRUE(Matrix::DoubleIsEqual(-1, -1 + 1E-7));

    bool failed = false;
    try {
        Matrix::DoubleIsEqual(NAN, 0);
    } catch (const etl::exception& e) {
        failed = true;
    }
    TEST_ASSERT_TRUE(failed);
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

    bool failed = false;
    try {
        double m33_data[3][3];
        Matrix m33(m33_data);
        m33.IsEqual(m22);
    } catch (const etl::exception& e) {
        failed = true;
    }
    TEST_ASSERT_TRUE(failed);
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

    bool failed = false;
    try {
        double m33_data[3][3];
        Matrix m33(m33_data);
        m33.Transpose(m22);
    } catch (const etl::exception& e) {
        failed = true;
    }
    TEST_ASSERT_TRUE(failed);
}

void TestVectorNorm() {
    double m41_data[4][1] = {{1}, {2}, {3}, {4}};
    Matrix m41(m41_data);

    TEST_ASSERT_EQUAL_DOUBLE(5.47722557505, Matrix::VectorNorm(m41));

    bool failed = false;
    try {
        double m33_data[3][3];
        Matrix m33(m33_data);
        Matrix::VectorNorm(m33);
    } catch (const etl::exception& e) {
        failed = true;
    }
    TEST_ASSERT_TRUE(failed);
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

    bool failed = false;
    try {
        double m33_data[3][3];
        Matrix m33(m33_data);
        m33.Add(m22, m22_2);
    } catch (const etl::exception& e) {
        failed = true;
    }
    TEST_ASSERT_TRUE(failed);
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

    bool failed = false;
    try {
        double m33_data[3][3];
        Matrix m33(m33_data);
        m33.Subtract(m22, m22_2);
    } catch (const etl::exception& e) {
        failed = true;
    }
    TEST_ASSERT_TRUE(failed);
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

    bool failed = false;
    try {
        double m33_data[3][3];
        Matrix m33(m33_data);
        m33.Multiply(m32, m32);
    } catch (const etl::exception& e) {
        failed = true;
    }
    TEST_ASSERT_TRUE(failed);
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

    bool failed = false;
    try {
        double m33_data[3][3];
        Matrix m33(m33_data);
        m33.MultiplyScalar(m22, 0);
    } catch (const etl::exception& e) {
        failed = true;
    }
    TEST_ASSERT_TRUE(failed);
}

void TestCrossProduct() {
    double m31_A_data[3][1] = {{1}, {2}, {3}};
    Matrix m31_A(m31_A_data);
    double m31_B_data[3][1] = {{4}, {1}, {9}};
    Matrix m31_B(m31_B_data);
    double m31_result_data[3][1];
    Matrix m31_result(m31_result_data);
    double m31_expected_data[3][1] = {{15}, {3}, {-7}};
    Matrix m31_expected(m31_expected_data);

    m31_result.CrossProduct(m31_A, m31_B);

    TEST_ASSERT_TRUE(m31_expected.IsEqual(m31_result));

    bool failed = false;
    try {
        double m33_data[3][3];
        Matrix m33(m33_data);
        m33.CrossProduct(m31_A, m31_B);
    } catch (const etl::exception& e) {
        failed = true;
    }
    TEST_ASSERT_TRUE(failed);
}

void TestFill() {
    double m22_data[2][2] = {{1, 2}, {3, 4}};
    Matrix m22(m22_data);
    double m22_expected_data[2][2] = {{1.23, 1.23}, {1.23, 1.23}};
    Matrix m22_expected(m22_expected_data);

    m22.Fill(1.23);

    TEST_ASSERT_TRUE(m22.IsEqual(m22_expected));
}

void TestCopyInto() {
    double m22_data[2][2] = {{1, 2}, {3, 4}};
    Matrix m22(m22_data);
    double m44_data[4][4];
    Matrix m44(m44_data);
    m44.Fill(0);

    m44.CopyInto(1, 1, m22);

    TEST_ASSERT_EQUAL_DOUBLE(m44.Get(1, 1), m22.Get(0, 0));
    TEST_ASSERT_EQUAL_DOUBLE(m44.Get(1, 2), m22.Get(0, 1));
    TEST_ASSERT_EQUAL_DOUBLE(m44.Get(2, 1), m22.Get(1, 0));
    TEST_ASSERT_EQUAL_DOUBLE(m44.Get(2, 2), m22.Get(1, 1));
    TEST_ASSERT_EQUAL_DOUBLE(m44.Get(3, 3), 0);

    bool failed = false;
    try {
        double m33_data[3][3];
        Matrix m33(m33_data);
        m33.CopyInto(0, 212, m44);
    } catch (const etl::exception& e) {
        failed = true;
    }
    TEST_ASSERT_TRUE(failed);

    failed = false;
    try {
        double m33_data[3][3];
        Matrix m33(m33_data);
        m33.CopyInto(0, 1, m44);
    } catch (const etl::exception& e) {
        failed = true;
    }
    TEST_ASSERT_TRUE(failed);
}

void TestIdentity() {
    double m33_identity_data[3][3];
    Matrix m33_identity(m33_identity_data);
    m33_identity.Identity();

    double m33_expected_data[3][3] = {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}};
    Matrix m33_expected(m33_expected_data);

    TEST_ASSERT_TRUE(m33_expected.IsEqual(m33_identity));

    bool failed = false;
    try {
        double m34_identity_data[3][4];
        Matrix m34_identity(m34_identity_data);
        m34_identity.Identity();
    } catch (const etl::exception& e) {
        failed = true;
    }
    TEST_ASSERT_TRUE(failed);
}

void TestQuaternionNormalise() {
    double q_data[4][1] = {{34}, {12}, {0}, {0.3}};
    Matrix q(q_data);
    double q_normalised_data[4][1];
    Matrix q_normalised(q_normalised_data);
    double q_normalised_expected_data[4][1] = {
        {0.942957693}, {0.3328085976}, {0}, {0.0083202149}};
    Matrix q_normalised_expected(q_normalised_expected_data);

    q_normalised.QuaternionNormalise(q);

    TEST_ASSERT_TRUE(q_normalised.IsEqual(q_normalised_expected));

    bool failed = false;
    try {
        double m33_data[3][3];
        Matrix m33(m33_data);
        m33.QuaternionNormalise(q);
    } catch (const etl::exception& e) {
        failed = true;
    }
    TEST_ASSERT_TRUE(failed);

    failed = false;
    try {
        double m33_data[3][3];
        Matrix m33(m33_data);
        q.QuaternionNormalise(m33);
    } catch (const etl::exception& e) {
        failed = true;
    }
    TEST_ASSERT_TRUE(failed);
}

void TestRotationMatrixFromQuaternion() {
    double q_data[4][1] = {{34}, {12}, {0}, {0.3}};
    Matrix q(q_data);
    double q_normalised_data[4][1];
    Matrix q_normalised(q_normalised_data);
    q_normalised.QuaternionNormalise(q);
    double rotation_matrix_data[3][3];
    Matrix rotation_matrix(rotation_matrix_data);

    double rotation_matrix_expected_data[3][3] = {
        {0.778476874677907, 0.627648855079264, 0.005538078133052},
        {0.627648855079264, -0.778338422724581, -0.015691221376982},
        {-0.005538078133052, 0.015691221376982, -0.999861548046674}};

    Matrix rotation_matrix_expected(rotation_matrix_expected_data);

    rotation_matrix.RotationMatrixFromQuaternion(q_normalised);

    TEST_ASSERT_TRUE(rotation_matrix_expected.IsEqual(rotation_matrix));

    bool failed = false;
    try {
        double m34_data[3][4];
        Matrix m34(m34_data);
        m34.RotationMatrixFromQuaternion(q);
    } catch (const etl::exception& e) {
        failed = true;
    }
    TEST_ASSERT_TRUE(failed);

    failed = false;
    try {
        double m34_data[3][4];
        Matrix m34(m34_data);
        rotation_matrix.RotationMatrixFromQuaternion(m34);
    } catch (const etl::exception& e) {
        failed = true;
    }
    TEST_ASSERT_TRUE(failed);
}

void TestSkewSymmetricFill() {
    // Random vectors. Test by applying the matrix as a cross product.
    double m31_input1_data[3][1] = {
        {0.814723686393179}, {0.905791937075619}, {0.126986816293506}};
    Matrix m31_input1(m31_input1_data);

    double m31_input2_data[3][1] = {
        {0.913375856139019}, {0.632359246225410}, {0.097540404999410}};
    Matrix m31_input2(m31_input2_data);

    double m31_expected_data[3][1] = {
        {0.008050024955630}, {0.036518213717047}, {-0.312130429800687}};
    Matrix m31_expected(m31_expected_data);

    double m33_skew_symmetric_data[3][3];
    Matrix m33_skew_symmetric(m33_skew_symmetric_data);
    m33_skew_symmetric.SkewSymmetricFill(m31_input1);

    double m31_result_data[3][1];
    Matrix m31_result(m31_result_data);
    m31_result.Multiply(m33_skew_symmetric, m31_input2);

    TEST_ASSERT_TRUE(m31_expected.IsEqual(m31_result));

    bool failed = false;
    try {
        double m34_data[3][4];
        Matrix m34(m34_data);
        m34.SkewSymmetricFill(m31_input1);
    } catch (const etl::exception& e) {
        failed = true;
    }
    TEST_ASSERT_TRUE(failed);

    failed = false;
    try {
        double m34_data[3][4];
        Matrix m34(m34_data);
        m33_skew_symmetric.SkewSymmetricFill(m34);
    } catch (const etl::exception& e) {
        failed = true;
    }
    TEST_ASSERT_TRUE(failed);
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

    bool failed = false;
    try {
        double m34_data[3][4];
        Matrix m34(m34_data);
        m34.ConcatenateHorizontally(m22, m23);
    } catch (const etl::exception& e) {
        failed = true;
    }
    TEST_ASSERT_TRUE(failed);

    failed = false;
    try {
        double m34_data[3][4];
        Matrix m34(m34_data);
        m25.ConcatenateHorizontally(m23, m34);
    } catch (const etl::exception& e) {
        failed = true;
    }
    TEST_ASSERT_TRUE(failed);
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

    bool failed = false;
    try {
        double m34_data[3][4];
        Matrix m34(m34_data);
        m34.ConcatenateVertically(m32, m52);
    } catch (const etl::exception& e) {
        failed = true;
    }
    TEST_ASSERT_TRUE(failed);

    failed = false;
    try {
        double m34_data[3][4];
        Matrix m34(m34_data);
        m52.ConcatenateVertically(m34, m32);
    } catch (const etl::exception& e) {
        failed = true;
    }
    TEST_ASSERT_TRUE(failed);
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

    bool failed = false;
    try {
        m22.AddRows(2, 100, 0);
    } catch (const etl::exception& e) {
        failed = true;
    }
    TEST_ASSERT_TRUE(failed);

    failed = false;
    try {
        m22.AddRows(255, 0, 0);
    } catch (const etl::exception& e) {
        failed = true;
    }
    TEST_ASSERT_TRUE(failed);
}

void TestMultiplyRow() {
    double scale = -0.123;
    double m22_data[2][2] = {{1, 2}, {3, 4}};
    Matrix m22(m22_data);
    double m22_expected_data[2][2] = {{1, 2}, {(3 * scale), (4 * scale)}};
    Matrix m22_expected(m22_expected_data);

    m22.MultiplyRow(1, scale);

    TEST_ASSERT_TRUE(m22.IsEqual(m22_expected));

    bool failed = false;
    try {
        m22.MultiplyRow(100, 0);
    } catch (const etl::exception& e) {
        failed = true;
    }
    TEST_ASSERT_TRUE(failed);
}

void TestSwitchRows() {
    double m22_data[2][2] = {{1, 2}, {3, 4}};

    Matrix m22(m22_data);
    double m22_expected_data[2][2] = {{3, 4}, {1, 2}};
    Matrix m22_expected(m22_expected_data);

    m22.SwitchRows(0, 1);

    TEST_ASSERT_TRUE(m22.IsEqual(m22_expected));

    bool failed = false;
    try {
        m22.SwitchRows(2, 100);
    } catch (const etl::exception& e) {
        failed = true;
    }
    TEST_ASSERT_TRUE(failed);

    failed = false;
    try {
        m22.SwitchRows(255, 1);
    } catch (const etl::exception& e) {
        failed = true;
    }
    TEST_ASSERT_TRUE(failed);
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

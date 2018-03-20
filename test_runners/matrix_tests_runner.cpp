/* AUTOGENERATED FILE. DO NOT EDIT. */

/*=======Test Runner Used To Run Each Test Below=====*/
#define RUN_TEST(TestFunc, TestLineNum) \
{ \
  Unity.CurrentTestName = #TestFunc; \
  Unity.CurrentTestLineNumber = TestLineNum; \
  Unity.NumberOfTests++; \
  if (TEST_PROTECT()) \
  { \
      SetUp(); \
      TestFunc(); \
  } \
  if (TEST_PROTECT()) \
  { \
    TearDown(); \
  } \
  UnityConcludeTest(); \
}

/*=======Automagically Detected Files To Include=====*/
#ifdef __WIN32__
#define UNITY_INCLUDE_SETUP_STUBS
#endif
#include "unity.h"
#ifndef UNITY_EXCLUDE_SETJMP_H
#include <setjmp.h>
#endif
#include <stdio.h>
#include "matrix_tests.h"

/*=======External Functions This Runner Calls=====*/
extern void SetUp(void);
extern void TearDown(void);
extern void TestCopyConstructor();
extern void TestSlice();
extern void TestGetNRows();
extern void TestGetNColumns();
extern void TestIsSquare();
extern void TestGet();
extern void TestSet();
extern void TestDoubleIsEqual();
extern void TestIsEqual();
extern void TestSameSize();
extern void TestSameNRows();
extern void TestSameNColumns();
extern void TestTranspose();
extern void TestVectorNorm();
extern void TestAdd();
extern void TestSubtract();
extern void TestMultiply();
extern void TestMultiplyScalar();
extern void TestCrossProduct();
extern void TestFill();
extern void TestCopyInto();
extern void TestIdentity();
extern void TestQuaternionNormalise();
extern void TestRotationMatrixFromQuaternion();
extern void TestSkewSymmetricFill();
extern void TestConcatenateHorizontally();
extern void TestConcatenateVertically();
extern void TestAddRows();
extern void TestMultiplyRow();
extern void TestSwitchRows();
extern void TestRowReduce();


/*=======Suite Setup=====*/
static void suite_setup(void)
{
#if defined(UNITY_WEAK_ATTRIBUTE) || defined(UNITY_WEAK_PRAGMA)
  suiteSetUp();
#endif
}

/*=======Suite Teardown=====*/
static int suite_teardown(int num_failures)
{
#if defined(UNITY_WEAK_ATTRIBUTE) || defined(UNITY_WEAK_PRAGMA)
  return suiteTearDown(num_failures);
#else
  return num_failures;
#endif
}

/*=======Test Reset Option=====*/
void resetTest_matrix_tests_runner(void);
void resetTest_matrix_tests_runner(void)
{
  TearDown();
  SetUp();
}


/*=======MAIN=====*/
 int matrix_tests_runner(void);
int matrix_tests_runner(void)
{
  suite_setup();
  UnityBegin("src/util/tests/matrix_tests.cpp");
  RUN_TEST(TestCopyConstructor, 8);
  RUN_TEST(TestSlice, 27);
  RUN_TEST(TestGetNRows, 51);
  RUN_TEST(TestGetNColumns, 58);
  RUN_TEST(TestIsSquare, 65);
  RUN_TEST(TestGet, 75);
  RUN_TEST(TestSet, 93);
  RUN_TEST(TestDoubleIsEqual, 116);
  RUN_TEST(TestIsEqual, 133);
  RUN_TEST(TestSameSize, 155);
  RUN_TEST(TestSameNRows, 167);
  RUN_TEST(TestSameNColumns, 179);
  RUN_TEST(TestTranspose, 191);
  RUN_TEST(TestVectorNorm, 212);
  RUN_TEST(TestAdd, 229);
  RUN_TEST(TestSubtract, 254);
  RUN_TEST(TestMultiply, 279);
  RUN_TEST(TestMultiplyScalar, 305);
  RUN_TEST(TestCrossProduct, 328);
  RUN_TEST(TestFill, 353);
  RUN_TEST(TestCopyInto, 364);
  RUN_TEST(TestIdentity, 400);
  RUN_TEST(TestQuaternionNormalise, 421);
  RUN_TEST(TestRotationMatrixFromQuaternion, 455);
  RUN_TEST(TestSkewSymmetricFill, 496);
  RUN_TEST(TestConcatenateHorizontally, 541);
  RUN_TEST(TestConcatenateVertically, 576);
  RUN_TEST(TestAddRows, 611);
  RUN_TEST(TestMultiplyRow, 640);
  RUN_TEST(TestSwitchRows, 660);
  RUN_TEST(TestRowReduce, 688);

  return suite_teardown(UnityEnd());
}

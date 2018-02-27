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
#include "sensor_state_logic_tests.h"

/*=======External Functions This Runner Calls=====*/
extern void SetUp(void);
extern void TearDown(void);
extern void TestBatteryChargeStateFlow(void);
extern void TestBatteryTempStateFlow(void);
extern void TestTelecomsTempStateFlow(void);


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
void resetTest_sensor_state_logic_tests_runner(void);
void resetTest_sensor_state_logic_tests_runner(void)
{
  TearDown();
  SetUp();
}


/*=======MAIN=====*/
 int sensor_state_logic_tests_runner(void);
int sensor_state_logic_tests_runner(void)
{
  suite_setup();
  UnityBegin("src/system/tests/sensor_state_logic_tests.cpp");
  RUN_TEST(TestBatteryChargeStateFlow, 84);
  RUN_TEST(TestBatteryTempStateFlow, 118);
  RUN_TEST(TestTelecomsTempStateFlow, 152);

  return suite_teardown(UnityEnd());
}

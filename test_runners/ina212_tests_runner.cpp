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
#include <src/util/memory_troubleshooter.h>
#include <external/etl/exception.h>
#include <src/board/board.h>
#include <src/board/i2c/i2c.h>
#include <src/board/i2c/multiplexers/i2c_multiplexer.h>
#include <src/config/unit_tests.h>
#include <src/sensors/i2c_sensors/current_sensors/ina212.h>
#include <src/sensors/i2c_sensors/measurables/ina212_current_measurable.h>

/*=======External Functions This Runner Calls=====*/
extern void SetUp(void);
extern void TearDown(void);
extern void TestIna212CurrentRead();


/*=======Suite Setup=====*/
static MemoryTroubleshooter *suite_setup(void)
{
  MemoryTroubleshooter *mem_test = new MemoryTroubleshooter();
#if defined(UNITY_WEAK_ATTRIBUTE) || defined(UNITY_WEAK_PRAGMA)
  suiteSetUp();
#endif
  return mem_test;
}

/*=======Suite Teardown=====*/
static int suite_teardown(int num_failures, MemoryTroubleshooter *mem_test)
{
    if(mem_test->MemoryLeakTest()){
        UNITY_PRINT_EOL();
        UnityPrint(memoryLeakMessage);
        UNITY_PRINT_EOL();
    }
    mem_test->~MemoryTroubleshooter();
#if defined(UNITY_WEAK_ATTRIBUTE) || defined(UNITY_WEAK_PRAGMA)
  return suiteTearDown(num_failures);
#else
  return num_failures;
#endif
}

/*=======Test Reset Option=====*/
void resetTest_ina212_tests_runner(void);
void resetTest_ina212_tests_runner(void)
{
  TearDown();
  SetUp();
}


/*=======MAIN=====*/
 int ina212_tests_runner(void);
int ina212_tests_runner(void)
{
  MemoryTroubleshooter *mem_test = suite_setup();
  try {
  UnityBegin("src/sensors/i2c_sensors/tests/ina212_tests.cpp");
    RUN_TEST(TestIna212CurrentRead, 12);
  } catch (etl::exception &e) {
    const char* message = e.what();
    TEST_FAIL_MESSAGE(message);
  }

  return suite_teardown(UnityEnd(), mem_test);
}

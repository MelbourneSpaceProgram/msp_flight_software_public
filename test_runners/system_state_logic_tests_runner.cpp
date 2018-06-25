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
#include <src/config/unit_tests.h>
#include <src/system/sensor_state_machines/detumbled_state_machine.h>
#include <src/system/sensor_state_machines/telecoms_temp_state_machine.h>
#include <src/system/sensor_state_machines/tle_state_machine.h>
#include <src/system/state_definitions.h>
#include <src/system/state_machine.h>
#include <src/system/state_manager.h>
#include <src/system/system_state_machines/adcs_state_machine.h>
#include <src/system/system_state_machines/power_state_machine.h>
#include <src/system/system_state_machines/telecoms_state_machine.h>

/*=======External Functions This Runner Calls=====*/
extern void SetUp(void);
extern void TearDown(void);
extern void TestTelecomsStateLogic();
extern void TestAdcsStateLogic();


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
void resetTest_system_state_logic_tests_runner(void);
void resetTest_system_state_logic_tests_runner(void)
{
  TearDown();
  SetUp();
}


/*=======MAIN=====*/
 int system_state_logic_tests_runner(void);
int system_state_logic_tests_runner(void)
{
  MemoryTroubleshooter *mem_test = suite_setup();
  try {
  UnityBegin("src/system/tests/system_state_logic_tests.cpp");
    RUN_TEST(TestTelecomsStateLogic, 34);
    RUN_TEST(TestAdcsStateLogic, 76);
  } catch (etl::exception &e) {
    const char* message = e.what();
    TEST_FAIL_MESSAGE(message);
  }

  return suite_teardown(UnityEnd(), mem_test);
}

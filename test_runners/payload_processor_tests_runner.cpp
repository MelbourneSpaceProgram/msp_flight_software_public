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
#include <external/nanopb/pb_encode.h>
#include <external/sgp4/sgp4.h>
#include <src/adcs/state_estimators/location_estimator.h>
#include <src/config/unit_tests.h>
#include <src/messages/Tle.pb.h>
#include <src/payload_processor/commands/test_command.h>
#include <src/payload_processor/commands/tle_update_command.h>
#include <src/payload_processor/payload_processor.h>
#include <src/telecomms/lithium.h>
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Mailbox.h>

/*=======External Functions This Runner Calls=====*/
extern void SetUp(void);
extern void TearDown(void);
extern void TestPayloadProcessor(void);
extern void TestForceResetCommand(void);
extern void TestTleUpdateCommand(void);


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
void resetTest_payload_processor_tests_runner(void);
void resetTest_payload_processor_tests_runner(void)
{
  TearDown();
  SetUp();
}


/*=======MAIN=====*/
 int payload_processor_tests_runner(void);
int payload_processor_tests_runner(void)
{
  MemoryTroubleshooter *mem_test = suite_setup();
  try {
  UnityBegin("src/payload_processor/tests/payload_processor_tests.cpp");
    RUN_TEST(TestPayloadProcessor, 14);
    RUN_TEST(TestForceResetCommand, 33);
    RUN_TEST(TestTleUpdateCommand, 51);
  } catch (etl::exception &e) {
    TEST_FAIL_MESSAGE("Uncaught exception in test");
  }

  return suite_teardown(UnityEnd(), mem_test);
}

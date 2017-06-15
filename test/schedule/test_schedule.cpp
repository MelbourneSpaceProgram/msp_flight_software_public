#include <test/unity/unity.h>

#include <src/public_headers/arch/System.h>
#include <src/public_headers/arch/Task.h>

#include <src/public_headers/systems.hpp>
#include <src/public_headers/request.hpp>
#include <src/public_headers/schedule.hpp>


static void test_get_size(void){
    // Make the new schedule.
    Schedule dut;
    TEST_ASSERT_EQUAL_INT(0, dut.get_size());

    request_long_type abc;
    // Now add an item
    dut.add(abc);

    TEST_ASSERT_EQUAL_INT(1, dut.get_size());

    // Clear it out
    dut.clear();

    TEST_ASSERT_EQUAL_INT(0, dut.get_size());
}

static void test_schedule_clear(void){
    Schedule dut;
    // Make sure the schedule is empty initially.
    TEST_ASSERT_EQUAL_INT(0, dut.get_size());

    // Add some items
    request_long_type new_item;
    dut.add(new_item);

    // Verify the item was added correctly
    TEST_ASSERT_EQUAL_INT(1, dut.get_size());

    // Now we test the clear functionality
    dut.clear();

    // Verify that all items were removed
    TEST_ASSERT_EQUAL_INT(0, dut.get_size());
}

static void test_schedule_request_retrieval(void){
    Schedule schedule;

    request_long_type new_item;
    // TODO
}

static void test_schedule_add_request(void) {
    Schedule schedule;

    request_long_type new_item;

    schedule.add(new_item);

    request_long_type returned_item = schedule.get_request(0);

    TEST_ASSERT_TRUE(requests_equal(new_item, returned_item));
}



void tests_schedule(){
    UNITY_BEGIN();

    RUN_TEST(test_get_size);
    RUN_TEST(test_schedule_clear);
    RUN_TEST(test_schedule_add_request);

    UNITY_END();
}


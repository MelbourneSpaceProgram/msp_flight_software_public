#include <test/unity/unity.h>

#include <src/public_headers/arch/System.h>
#include <src/public_headers/arch/Task.h>

#include <src/public_headers/systems.hpp>
#include <src/public_headers/request.hpp>

static void test_requests_equal(void){
	// Make some requests
	request_long request_a;
	request_long request_b;

	// Same ID implies equal
	request_a.request_id = 10;
	request_b.request_id = 10;
	TEST_ASSERT_TRUE(requests_equal(request_a, request_b));

	request_a.request_id = 10;
	request_b.request_id = 20;
	TEST_ASSERT_FALSE(requests_equal(request_a, request_b));
}

static void test_requests_overlap(void){
	// Make some requests
	request_long request_a;
	request_long request_b;

	request_a.start_time = 10;
	request_a.end_time = 20;
	request_b.start_time = 11;
	request_b.end_time = 21;
	TEST_ASSERT_TRUE(requests_overlap(request_a, request_b));

	request_a.start_time = 10;
	request_a.end_time = 20;
	request_b.start_time = 10;
	request_b.end_time = 20;
	TEST_ASSERT_TRUE(requests_overlap(request_a, request_b));

	request_a.start_time = 10;
	request_a.end_time = 20;
	request_b.start_time = 21;
	request_b.end_time = 30;
	TEST_ASSERT_FALSE(requests_overlap(request_a, request_b));

	request_a.start_time = 30;
	request_a.end_time = 40;
	request_b.start_time = 21;
	request_b.end_time = 29;
	TEST_ASSERT_FALSE(requests_overlap(request_a, request_b));

	// How does it deal with poorly ordered time? 
	// TODO Is this the right behaviour to be seeing here?
	// Currently will say they don't overlap
	request_a.start_time = 10;
	request_a.end_time = 1;
	request_b.start_time = 21;
	request_b.end_time = 30;
	TEST_ASSERT_FALSE(requests_overlap(request_a, request_b));
}

void tests_request(){
    UNITY_BEGIN();

    RUN_TEST(test_requests_equal);
	RUN_TEST(test_requests_overlap);

    UNITY_END();
}


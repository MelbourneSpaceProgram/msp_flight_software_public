

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include "cmocka.h"

static void test_nothing(void **state){

	assert_true(1 == 1);
}

static void test_request_power_available(void **state){
    float power_available = request_power_available();
    assert_true(power_available < 20); // We can never have more than 20 Watts of power available, physically impossible
    assert_true(power_available >= 0); // Negative power is also something that cannot happen
}

int main(void){
	const struct CMUnitTest tests[] = {
		cmocka_unit_test(test_nothing),
        cmocka_unit_test(test_request_power_available)
    };

	return cmocka_run_group_tests(tests, NULL, NULL);
}

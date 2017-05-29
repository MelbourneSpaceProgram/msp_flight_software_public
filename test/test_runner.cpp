#include <test/schedule/test_schedule.cpp>
#include <test/allocator/test_allocator.cpp>
#include <test/request/test_request.cpp>

int main(){
	tests_schedule();
	tests_allocator();
	tests_request();
	return 0;
}
# CppUTest
CppUTest requires no manual or automated work other than writing the test file. There is no need to generate additional files (i.e `generate_tests.py`). It also gives a cleaner output which makes it easier to find failing tests. Additionally, only specific tests can be configured to run so there is no need to run all tests when you only care about one.

## How to switch

### Includes

Before: `#include <test_runners/unity.h>`
After: `#include <CppUTest/TestHarness.h>`

### Quick demonstration

```
#include <CppUTest/TestHarness.h>
// Other includes here

TEST_GROUP(NameOfTestGroup) {
    void setup() {
        // If this test relies on special hardware to run, include a 
        // conditional here. Else the entire void setup() can be deleted,
        // leaving just the TEST_GROUP.
        if (!imu_available) {
            TEST_EXIT;
        }
    };
};

// Ensure the first string matches the TEST_GROUP. The second string should be
// a descriptive name of what is being tested (specifically).
TEST(NameOfTestGroup, OneIsOne) {
    CHECK(1 == 1);
}

// A test group can have multiple tests
TEST(NameOfTestGroup, OneIsNotTwo) {
    CHECK(1 != 2);
}

TEST(NameOfTestGroup, ThreeIsThree) {
    CHECK_EQUAL(3, 3);
}

TEST(NameOfTestGroup, OneIsOneDoubles) {
    // The first argument is the tolerance
    DOUBLES_EQUAL(0.001, 1, 1);
}
```

### Unity -> CppUTest equivalents

```
TEST_ASSERT_EQUAL_* -> CHECK_EQUAL
TEST_ASSERT_FLOAT_WITHIN -> DOUBLES_EQUAL(tolerance, expected, actual)
TEST_ASSERT_DOUBLE_WITHIN -> DOUBLES_EQUAL(tolerance, expected, actual)
TEST_ASSERT_DOUBLE_EQUAL -> DOUBLES_EQUAL(tolerance, expected, actual)
TEST_ASSERT_TRUE -> CHECK
TEST_ASSERT_FALSE -> CHECK_FALSE
TEST_ASSERT -> CHECK
```

## Configuring which tests are run

In `src/init/test_initialiser.cpp` there is one variables that needs to be modified: `test_args`.

### Verbose mode

By default CppUTest only prints a `.` or a `!` for each successful test. To have it print the names of the tests it is running add `-v` to the array.

### Exclude a group of tests

Add `-xg` as one element followed by `NameOfTestGroup` as the next element. This will skip all tests in the group specified.

### Run only a group of tests

Add `-sg` as one element followed by `NameOfTestGroup` as the next element. This will run only tests in the group specified.
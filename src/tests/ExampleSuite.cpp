#include <CppUTest/TestHarness.h>

TEST_GROUP(ExampleSuite)
{
};


TEST(ExampleSuite, testInt)
{
    int i = 3;

    CHECK_EQUAL(3, i);
}

TEST(ExampleSuite, testString)
{
    const char* str = "example";

    STRCMP_EQUAL("example", str);
    STRCMP_NOCASE_EQUAL("EXAMPLE", str);
}

TEST(ExampleSuite, testDouble)
{
    double d = 3.67;

    DOUBLES_EQUAL(3.64, d, 0.1);
}

TEST(ExampleSuite, testPointer)
{
    int i = 3;
    int *p2 = &i;

    POINTERS_EQUAL(&i, p2);
}

TEST(ExampleSuite, testWillFail)
{
    int wrong = 3;

    CHECK_EQUAL(762, wrong); // This will fail!
}

#include "CppUTest/TestHarness.h"
#include "../inc/main.hpp"

TEST_GROUP(FirstTestGroup)
{
};

TEST(FirstTestGroup, FirstTest)
{
   FAIL("Fail me!");
}

TEST(FirstTestGroup, SecondTest) {
    CHECK_EQUAL(add_function(4,5), 9);
}

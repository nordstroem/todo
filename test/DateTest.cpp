#include "Date.hpp"
#include <doctest.h>

using namespace todo;

TEST_CASE("to string")
{
    Date d{.year = 2019, .month = 12, .day = 30};
    REQUIRE(d.toString() == "2019-12-30");
}

TEST_CASE("valid")
{
    Date d1{.year = 2020, .month = 10, .day = 1};
    REQUIRE(d1.valid());
    Date d2{.year = 2020, .month = 2, .day = 31};
    REQUIRE_FALSE(d2.valid());
}

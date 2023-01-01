#include "Date.hpp"
#include <doctest/doctest.h>

using namespace todo;

TEST_CASE("to string")
{
    const Date d{.year = 2019, .month = 12, .day = 30};
    REQUIRE(d.toString() == "2019-12-30");
}

TEST_CASE("valid")
{
    const Date d1{.year = 2020, .month = 10, .day = 1};
    REQUIRE(d1.valid());
    const Date d2{.year = 2020, .month = 2, .day = 31};
    REQUIRE_FALSE(d2.valid());
}

TEST_CASE("from string")
{
    REQUIRE(Date::fromString("2019-12-30") == Date{.year = 2019, .month = 12, .day = 30});
    REQUIRE(Date::fromString("today") == Date::today());
    REQUIRE(Date::fromString("tomorrow") == Date::tomorrow());
    REQUIRE(Date::fromString("yesterday") == Date::yesterday());
    REQUIRE_THROWS(Date::fromString("random string"));
    REQUIRE_THROWS(Date::fromString("2019012030"));
}

TEST_CASE("ordering")
{
    const Date d1{.year = 2022, .month = 11, .day = 30};
    REQUIRE(d1.valid());
    const Date d2{.year = 2023, .month = 2, .day = 0};
    REQUIRE_GE(d2, d1);
}

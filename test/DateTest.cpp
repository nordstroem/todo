#include "Date.hpp"
#include <doctest.h>
#include <fmt/core.h>

using namespace todo;

TEST_CASE("to string")
{
    Date d{.year = 2019, .month = 12, .day = 30};
    REQUIRE(d.toString() == "2019-12-30");
}

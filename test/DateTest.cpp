#include "todo/Date.hpp"
#include <date/date.h>
#include <doctest.h>
#include <fmt/core.h>

using namespace todo;

TEST_CASE("date")
{
    date::year_month_day firstOfJanuary(date::year(2020), date::month(1), date::day(1));
    Date d("2020-01-01");
    REQUIRE(d.daysSinceEpoch() == static_cast<date::sys_days>(firstOfJanuary).time_since_epoch().count());
    Date d2("today");
    REQUIRE(d2.daysSinceEpoch() == date::floor<date::days>(std::chrono::system_clock::now()).time_since_epoch().count());
}

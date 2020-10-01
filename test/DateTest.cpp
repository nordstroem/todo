#include "todo/Date.hpp"
#include <date/date.h>
#include <doctest.h>
#include <fmt/core.h>

using namespace todo;

TEST_CASE("create date")
{
    Date d("0000-00-01");
    REQUIRE(d.days() == 1);
    date::year_month_day firstOfJanuary(date::year(2020), date::month(1), date::day(1));
    Date d2("2020-01-01");
    REQUIRE(d2.days() == static_cast<date::sys_days>(firstOfJanuary).time_since_epoch().count());
    Date d3("today");
    REQUIRE(d2.days() == date::floor<date::days>(std::chrono::system_clock::now()).time_since_epoch().count());
}

TEST_CASE("date to string")
{
    Date d("2020-01-01");
}

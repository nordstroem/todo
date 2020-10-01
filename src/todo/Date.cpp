#include "Date.hpp"
#include <date/date.h>
#include <fmt/core.h>

namespace todo {

Date Date::today()
{
    auto ymd = date::year_month_day{date::floor<date::days>(std::chrono::system_clock::now())};
    return {.year = static_cast<int>(ymd.year()), .month = static_cast<unsigned>(ymd.month()), .day = static_cast<unsigned>(ymd.day())};
}

std::string Date::toString() const
{
    return fmt::format("{}-{}-{}", this->year, this->month, this->day);
}

bool Date::valid() const
{
    return date::year_month_day(date::year{this->year} / month / day).ok();
}

bool Date::operator==(const Date& other) const
{
    return this->year == other.year && this->month == other.month && this->day == other.day;
}

size_t Date::hash() const noexcept
{
    return std::hash<int>{}(date::sys_days{date::year{this->year} / this->month / this->day}.time_since_epoch().count());
}

} // namespace todo

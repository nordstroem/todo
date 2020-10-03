#include "Date.hpp"
#include <algorithm>
#include <array>
#include <date/date.h>
#include <fmt/core.h>

namespace todo {

static bool validDateString(const std::string& string)
{
    if (string.length() != 10)
        return false;
    constexpr auto digitPositions = std::array<int, 8>{0, 1, 2, 3, 5, 6, 8, 9};
    if (std::any_of(digitPositions.begin(), digitPositions.end(), [&](const auto& p) { return !std::isdigit(string.at(p)); }))
        return false;
    if (string.at(4) != '-' || string.at(7) != '-')
        return false;
    return true;
}

Date Date::today()
{
    auto ymd = date::year_month_day{date::floor<date::days>(std::chrono::system_clock::now())};
    return {.year = static_cast<int>(ymd.year()), .month = static_cast<unsigned>(ymd.month()), .day = static_cast<unsigned>(ymd.day())};
}

std::optional<Date> Date::fromString(const std::string& string)
{
    if (string == "today")
        return Date::today();

    if (validDateString(string)) {
        int year = std::stoi(string.substr(0, 4));
        unsigned month = std::stoull(string.substr(5, 2));
        unsigned day = std::stoull(string.substr(8, 2));
        return Date{.year = year, .month = month, .day = day};
    } else {
        return std::nullopt;
    }
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

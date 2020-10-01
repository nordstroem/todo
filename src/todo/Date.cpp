#include "Date.hpp"
#include <date/date.h>
#include <regex>
#include <spdlog/spdlog.h>

namespace todo {

constexpr date::year_month_day firstDay(date::year(0), date::month(0), date::day(0));

Date::Date(const std::string& date)
{
    std::regex exp(R"_((\d{4})-(\d{2})-(\d{2}))_");
    std::smatch matches;
    constexpr date::sys_days sysFirstDay = static_cast<date::sys_days>(firstDay);

    if (date == "today") {
        auto t = std::chrono::system_clock::now() - sysFirstDay;
        this->_days = date::floor<date::days>().count();
    } else if (bool match = std::regex_match(date, matches, exp)) {
        date::year_month_day d(date::year(std::stoi(matches[1])), date::month(std::stoi(matches[2])), date::day(std::stoi(matches[3])));
        this->_days = (static_cast<date::sys_days>(d) - sysFirstDay).count();
    } else {
        spdlog::error("Invalid date string");
    }
}

int Date::days() const noexcept
{
    return this->_days;
}

std::string Date::toString() const noexcept
{
    return "";
}

} // namespace todo

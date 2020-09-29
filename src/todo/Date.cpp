#include "Date.hpp"
#include <date/date.h>
#include <regex>
#include <spdlog/spdlog.h>

namespace todo {

Date::Date(std::string_view date)
{
    std::regex exp(R"_((\d{4})-(\d{2})-(\d{2}))_");
    std::smatch matches;
    std::string string = date.data();

    if (string == "today") {
        auto t = date::floor<date::days>(std::chrono::system_clock::now());
        this->_daysSinceEpoch = date::floor<date::days>(std::chrono::system_clock::now()).time_since_epoch().count();
    } else if (bool match = std::regex_match(string, matches, exp)) {
        date::year_month_day d(date::year(std::stoi(matches[1])), date::month(std::stoi(matches[2])), date::day(std::stoi(matches[3])));
        this->_daysSinceEpoch = static_cast<date::sys_days>(d).time_since_epoch().count();
    } else {
        spdlog::error("Invalid date string");
    }
}

int Date::daysSinceEpoch() const noexcept
{
    return this->_daysSinceEpoch;
}

} // namespace todo

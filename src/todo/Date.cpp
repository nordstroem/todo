#include "Date.hpp"
#include <date/date.h>

namespace todo {

bool Date::operator==(const Date& other) const
{
    return year == other.year && month == other.month && day == other.day;
}

size_t Date::hash() const noexcept
{
    return std::hash<int>{}(date::sys_days{date::year{year} / month / day}.time_since_epoch().count());
}

} // namespace todo

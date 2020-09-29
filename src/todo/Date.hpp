#pragma once
#include <string_view>

namespace todo {

/// Class representing a specific day in time (year-month-day)
class Date
{
public:
    explicit Date(std::string_view date);
    int daysSinceEpoch() const noexcept;

private:
    int _daysSinceEpoch = 0;
};

} // namespace todo

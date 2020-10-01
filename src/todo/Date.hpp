#pragma once
#include <string>

namespace todo {

/// Class representing a specific day in time (year-month-day)
class Date
{
public:
    explicit Date(const std::string& date);
    int days() const noexcept;
    std::string toString() const noexcept;

private:
    int _days = 0;
};

} // namespace todo

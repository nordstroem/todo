#pragma once
#include <functional>
#include <string>

namespace todo {

struct Date
{
    int year = 0;
    unsigned month = 0;
    unsigned day = 0;

public:
    static Date today();

public:
    /// @return string representation of the date in the format yyyy-mm-dd
    std::string toString() const;
    /// @return whether this is a valid date in the Gregorian calendar
    bool valid() const;
    /// @return hash of this date
    size_t hash() const noexcept;

    bool operator==(const Date& other) const;
};

} // namespace todo

namespace std {
template <>
struct hash<todo::Date>
{
    std::size_t operator()(const todo::Date& date) const noexcept
    {
        return std::hash<int>{}(date.hash());
    }
};
} // namespace std

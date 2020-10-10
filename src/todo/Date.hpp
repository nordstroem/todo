#pragma once
#include <functional>
#include <string>

namespace todo {

struct Date
{
    int32_t year = 0;
    uint32_t month = 0;
    uint32_t day = 0;

public:
    /// @return todays date
    static Date today();
    /// @return tomorrows date
    static Date tomorrow();
    /// @return yesterdays date
    static Date yesterday();
    /**
     * Returns a date from a string in the format yyyy-mm-dd. If the string is ill-formed, std::invalid_argument is thrown
     * @param string the date as a string 
     * @return the date as an optional
     */
    static Date fromString(const std::string& string) noexcept(false);

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

#pragma once
#include <functional>

namespace todo {

struct Date
{
    int year = 0;
    int month = 0;
    int day = 0;

    size_t hash() const noexcept;
    bool operator==(const Date& other) const;

    template <class Archive>
    void serialize(Archive& archive)
    {
        archive(year, month, day);
    }
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
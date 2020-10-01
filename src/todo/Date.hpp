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
    size_t hash() const noexcept;
    std::string toString() const;
    bool operator==(const Date& other) const;

    template <class Archive>
    void serialize(Archive& archive)
    {
        archive(this->year, this->month, this->day);
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
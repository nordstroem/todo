#include "DatabaseCommandVisitor.hpp"
#include <algorithm>
#include <fmt/color.h>
#include <fmt/core.h>
#include <functional>

namespace {
template <typename T>
std::string toString(const T& t)
{
    if constexpr (std::is_same_v<T, std::string>)
        return t;
    else
        return std::to_string(t);
}

template <typename Container>
class MaxLengthHelper
{
public:
    explicit MaxLengthHelper(const Container& c)
        : _c(c)
    {}
    template <typename Transform>
    auto operator()(const Transform& t) const
    {
        auto compare = [&](const auto& a, const auto& b) { return toString(t(a)).length() < toString(t(b)).length(); };
        return toString(t(*std::max_element(this->_c.begin(), this->_c.end(), compare))).length();
    }

private:
    const Container& _c;
};
} // namespace

namespace todo {

DatabaseCommandVisitor::DatabaseCommandVisitor(std::string_view databasePath)
    : _database(Database(databasePath))
{
}

void DatabaseCommandVisitor::operator()(ShowMessage&& cmd) const
{
    fmt::print("{}\n", cmd.message);
}

void DatabaseCommandVisitor::operator()(ShowTasks&& cmd) const
{
    const auto& tasks = this->_database.at(cmd.date);
    if (tasks.size() > 0) {
        MaxLengthHelper maxLength(tasks);
        auto maxHash = maxLength([](const auto& e) { return e.hash; });
        auto maxDescription = maxLength([](const auto& e) { return e.description; });
        auto maxPriority = maxLength([](const auto& e) { return e.priority; });

        auto hashString = std::string("Hash");
        int hashPadding = std::max(maxHash, hashString.length());
        auto descriptionString = std::string("Task");
        int descriptionPadding = std::max(maxDescription + 2, descriptionString.length());
        auto priorityString = std::string("Priority");
        int priorityPadding = std::max(maxPriority, priorityString.length());
        auto doneString = std::string("Done");

        fmt::print(fg(fmt::color::indian_red), "To do at {}:\n\n", cmd.date.toString());
        fmt::print("{:>{}}  {:<{}}  {:<{}}  {}\n", hashString, hashPadding, descriptionString, descriptionPadding, priorityString, priorityPadding, doneString);
        for (const auto& task : tasks) {
            auto checked = task.done ? fmt::format(fg(fmt::color::green), "V") : fmt::format(" ");
            fmt::print("{:>{}}  {:<{}}  {:<{}}  [{}]\n", task.hash, hashPadding, fmt::format(R"("{}")", task.description), descriptionPadding, task.priority, priorityPadding, checked);
        }
        fmt::print("\n");
    } else {
        fmt::print(fg(fmt::color::indian_red), "Nothing to do {}\n", cmd.date == Date::today() ? fmt::format("today") : fmt::format("at {}", cmd.date.toString()));
    }
}

void DatabaseCommandVisitor::operator()(AddTask&& cmd)
{
    this->_database.add(std::move(cmd.task), cmd.date);
}

void DatabaseCommandVisitor::operator()(RemoveTask&& cmd)
{
    this->_database.remove(cmd.hash);
}

void DatabaseCommandVisitor::operator()(CheckTask&& cmd)
{
    this->_database.check(cmd.hash);
}

void DatabaseCommandVisitor::operator()([[maybe_unused]] DoNothing&& cmd) const
{
}

const Database& DatabaseCommandVisitor::database() const noexcept
{
    return this->_database;
}

} // namespace todo

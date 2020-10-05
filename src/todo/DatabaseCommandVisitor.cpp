#include "DatabaseCommandVisitor.hpp"
#include <algorithm>
#include <fmt/color.h>
#include <fmt/core.h>

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
        auto maxHash = std::to_string(std::max_element(tasks.begin(), tasks.end(), [](const auto& a, const auto& b) { return std::to_string(a.hash).length() < std::to_string(b.hash).length(); })->hash).length();
        auto maxDescription = std::max_element(tasks.begin(), tasks.end(), [](const auto& a, const auto& b) { return a.description.length() < b.description.length(); })->description.size();
        auto maxPriority = std::to_string(std::max_element(tasks.begin(), tasks.end(), [](const auto& a, const auto& b) { return std::to_string(a.priority).length() < std::to_string(b.priority).length(); })->priority).length();

        auto hashString = std::string("Hash");
        int hashPadding = std::max(maxHash, hashString.length()) + 1;
        auto descriptionString = std::string("Description");
        int descriptionPadding = std::max(maxDescription, descriptionString.length());
        auto priorityString = std::string("Priority");
        int priorityPadding = std::max(maxPriority, priorityString.length());
        auto doneString = std::string("Done");

        fmt::print("{:>{}}  {:<{}}  {:<{}}  {}\n", hashString, hashPadding, descriptionString, descriptionPadding, priorityString, priorityPadding, doneString);
        for (const auto& task : tasks) {
            auto checked = task.done ? fmt::format(fg(fmt::color::green), "V") : fmt::format(" ");
            fmt::print("{:>{}}  {:<{}}  {:<{}}  [{}]\n", task.hash, hashPadding, task.description, descriptionPadding, task.priority, priorityPadding, checked);
        }
    } else {
        fmt::print("Nothing to do {}\n", cmd.date == Date::today() ? "today" : "at this date");
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

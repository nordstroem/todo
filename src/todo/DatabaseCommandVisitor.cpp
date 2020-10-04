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
        auto maxLength = std::max_element(tasks.begin(), tasks.end(), [](const auto& a, const auto& b) { return a.description.length() < b.description.length(); })->description.size();
        for (const auto& task : tasks) {
            auto checked = task.done ? fmt::format(fg(fmt::color::green), "V") : fmt::format(" ");
            fmt::print("{:<{}} [{}] {} {:x}\n", task.description, maxLength + 5, checked, task.priority, task.hash);
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

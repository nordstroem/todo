#include "DatabaseCommandVisitor.hpp"
#include <algorithm>
#include <fmt/core.h>

namespace todo {

DatabaseCommandVisitor::DatabaseCommandVisitor(const std::string& databasePath)
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
        auto maxLength = std::max_element(tasks.begin(), tasks.end(), [](const auto& a, const auto& b) { return a.task.description.length() < b.task.description.length(); })->task.description.size();
        for (const auto& [task, hash] : tasks)
            fmt::print("{:<{}} [{}] {:x}\n", task.description, maxLength + 5, task.priority, hash);
    } else {
        fmt::print("Nothing to do {}\n", cmd.date == Date::today() ? "today" : "at this date");
    }
}

void DatabaseCommandVisitor::operator()(AddTask&& cmd)
{
    this->_database.add(std::move(cmd.task), cmd.date);
}

void DatabaseCommandVisitor::operator()([[maybe_unused]] DoNothing&& cmd) const
{
}

const Database& DatabaseCommandVisitor::database() const noexcept
{
    return this->_database;
}

} // namespace todo

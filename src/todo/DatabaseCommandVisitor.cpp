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
        auto maxLength = std::max_element(tasks.begin(), tasks.end(), [](const Task& a, const Task& b) { return a.description.length() < b.description.length(); })->description.size();
        for (const auto& [description, priority] : tasks)
            fmt::print("{:<{}} [{}]\n", description, maxLength + 5, priority);
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

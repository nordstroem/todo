#include "DatabaseCommandVisitor.hpp"
#include <fmt/core.h>

namespace todo {
DatabaseCommandVisitor::DatabaseCommandVisitor(const std::string& databasePath)
    : _database(Database(databasePath))
{
}

void DatabaseCommandVisitor::operator()(ShowMessage&& cmd) const
{
    fmt::print("{}", cmd.message);
}

void DatabaseCommandVisitor::operator()([[maybe_unused]] ShowTasks&& cmd) const
{
    for (const auto& task : this->_database.at(cmd.date))
        fmt::print("{} [{}]\n", task.task, task.priority);
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

#pragma once
#include "Database.hpp"
#include "DatabaseCommand.hpp"
#include <string_view>

namespace todo {
/**
 * Applies a DatabaseCommand to a database
 */
class DatabaseCommandVisitor
{
public:
    DatabaseCommandVisitor() = default;
    explicit DatabaseCommandVisitor(std::string_view databasePath);

public:
    void operator()(ShowMessage&& cmd) const;
    void operator()(ShowTasks&& cmd) const;
    void operator()(ShowUndoneTasks&& cmd) const;
    void operator()(AddTask&& cmd);
    void operator()(RemoveTask&& cmd);
    void operator()(CheckTask&& cmd);
    void operator()(MoveTask&& cmd);
    void operator()(DoNothing&& cmd) const;
    const Database& database() const noexcept;

private:
    Database _database;
};

} // namespace todo

#pragma once
#include "Database.hpp"
#include "DatabaseCommand.hpp"
#include <string>

namespace todo {
/**
 * 
 */
class DatabaseCommandVisitor
{
public:
    DatabaseCommandVisitor() = default;
    explicit DatabaseCommandVisitor(const std::string& databasePath);

public:
    void operator()(ShowMessage&& cmd) const;
    void operator()(ShowTasks&& cmd) const;
    void operator()(AddTask&& cmd);
    void operator()(DoNothing&& cmd) const;
    const Database& database() const noexcept;

private:
    Database _database;
};

} // namespace todo

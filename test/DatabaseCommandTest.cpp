#include "DatabaseCommandVisitor.hpp"
#include "Date.hpp"
#include <doctest/doctest.h>

using namespace todo;

TEST_CASE("help message")
{
    std::vector<const char*> args = {"todo", "--help"};
    auto command = todo::parse(args);
    REQUIRE(std::holds_alternative<ShowMessage>(command));
    REQUIRE(std::get<ShowMessage>(command).message.size() > 0);
}

TEST_CASE("show task")
{
    std::vector<const char*> args = {"todo", "--show"};
    auto command = todo::parse(args);
    REQUIRE(std::holds_alternative<ShowTasks>(command));
}

TEST_CASE("add task")
{
    std::vector<const char*> args = {{"todo", "--add", "do something", "--date", "2020-10-01"}};
    auto command = todo::parse(args);
    REQUIRE(std::holds_alternative<AddTask>(command));
    auto [task, date] = std::get<AddTask>(command);
    REQUIRE(task.description == "do something");
    REQUIRE(date == Date{.year = 2020, .month = 10, .day = 1});
}

TEST_CASE("remove task")
{
    std::vector<const char*> args = {{"todo", "--remove", "1"}};
    auto command = todo::parse(args);
    REQUIRE(std::holds_alternative<RemoveTask>(command));
    auto [hash] = std::get<RemoveTask>(command);
    REQUIRE(hash == 1);
}

TEST_CASE("check task")
{
    std::vector<const char*> args = {{"todo", "--check", "1"}};
    auto command = todo::parse(args);
    REQUIRE(std::holds_alternative<CheckTask>(command));
    auto [hash] = std::get<CheckTask>(command);
    REQUIRE(hash == 1);
}

TEST_CASE("move task")
{
    std::vector<const char*> args = {{"todo", "--move", "1"}};
    auto command = todo::parse(args);
    REQUIRE(std::holds_alternative<MoveTask>(command));
    auto [hash, date] = std::get<MoveTask>(command);
    REQUIRE(hash == 1);
    REQUIRE(date == Date::today());
}

TEST_CASE("show undone tasks")
{
    std::vector<const char*> args = {{"todo", "--show_undone"}};
    auto command = todo::parse(args);
    REQUIRE(std::holds_alternative<ShowUndoneTasks>(command));
}

TEST_CASE("apply add task command")
{
    const auto date = Date::fromString("2012-12-12");
    DatabaseCommandVisitor visitor;
    DatabaseCommand command = AddTask{.task = {"task to do"}, .date = date};
    std::visit(visitor, std::move(command));

    const auto& database = visitor.database();
    const auto tasks = database.at(date);
    REQUIRE(tasks.size() == 1);
    REQUIRE(tasks.front().description == "task to do");
}

#include "DatabaseCommandVisitor.hpp"
#include "Date.hpp"
#include <doctest.h>

using namespace todo;

TEST_CASE("help message")
{
    std::vector<const char*> args = {"todo", "--help"};
    auto command = todo::parse(args.size(), &args[0]);
    REQUIRE(std::holds_alternative<ShowMessage>(command));
    REQUIRE(std::get<ShowMessage>(command).message.size() > 0);
}

TEST_CASE("add task")
{
    std::vector<const char*> args = {{"todo", "--add", "do something", "--date", "2020-10-01"}};
    auto command = todo::parse(args.size(), &args[0]);
    REQUIRE(std::holds_alternative<AddTask>(command));
    auto [task, date] = std::get<AddTask>(command);
    REQUIRE(task.task == "do something");
    REQUIRE(date == Date{.year = 2020, .month = 10, .day = 1});
}

TEST_CASE("apply add task command")
{
    const auto date = *Date::fromString("2012-12-12");
    DatabaseCommandVisitor visitor;
    DatabaseCommand command = AddTask{.task = {"task to do"}, .date = date};
    std::visit(visitor, std::move(command));

    const auto& database = visitor.database();
    const auto tasks = database.at(date);
    REQUIRE(tasks.size() == 1);
    REQUIRE(tasks.at(0).task == "task to do");
}
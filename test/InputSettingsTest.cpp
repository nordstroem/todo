#include "InputSettings.hpp"
#include "Date.hpp"
#include <doctest.h>
#include <fmt/core.h>
#include <string>
#include <vector>

using namespace todo;

TEST_CASE("help message")
{
    std::vector<const char*> args = {"todo", "--help"};
    auto input = todo::parse(args.size(), &args[0]);
    REQUIRE(input.message);

    args = {"todo", "--add", "do something"};
    input = todo::parse(args.size(), &args[0]);
    REQUIRE_FALSE(input.message);
}

TEST_CASE("input settings")
{
    std::vector<const char*> args = {"todo", "--add", "do something", "--date", "2020-10-01"};
    auto settings = todo::parse(args.size(), &args[0]);

    REQUIRE(settings.task);
    REQUIRE(settings.task->task == "do something");

    REQUIRE(settings.date);
    REQUIRE(*settings.date == Date{.year = 2020, .month = 10, .day = 1});
}

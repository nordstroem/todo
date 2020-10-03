#include "Database.hpp"
#include <algorithm>
#include <doctest.h>
#include <filesystem>
#include <fmt/core.h>

using namespace todo;

TEST_CASE("add and at")
{
    const Date date = {.year = 2020, .month = 1, .day = 1};
    Database database;
    database.add({.task = "sample task", .priority = 1}, date);

    auto tasks = database.at(date);
    REQUIRE(tasks.size() == 1);

    const auto& [task, priority] = tasks.at(0);
    REQUIRE(task == "sample task");
    REQUIRE(priority == 1);
}

TEST_CASE("sorted")
{
    const Date date = {.year = 2020, .month = 1, .day = 1};
    Database database;
    database.add({.task = "low prio", .priority = 1}, date);
    database.add({.task = "high prio", .priority = 10}, date);
    database.add({.task = "medium prio", .priority = 5}, date);

    auto tasks = database.at(date);
    REQUIRE(std::is_sorted(tasks.begin(), tasks.end(), std::greater<Task>()));
}

TEST_CASE("input file")
{
    auto path = std::filesystem::temp_directory_path().append("database.bin");
    if (std::filesystem::exists(path))
        std::filesystem::remove(path);

    const Date date = {.year = 2022, .month = 3, .day = 4};
    {
        Database database(path.c_str());
        database.add({.task = "test task"}, date);
    }
    auto database = Database(path.c_str());

    auto tasks = database.at(date);
    REQUIRE(tasks.size() == 1);

    const auto& [task, priority] = tasks.at(0);
    REQUIRE(task == "test task");
    REQUIRE(priority == 0);
}

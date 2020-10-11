#include "Database.hpp"
#include <algorithm>
#include <doctest.h>
#include <filesystem>
#include <fmt/core.h>
#include <ranges>

using namespace todo;

TEST_CASE("add and at")
{
    const Date date = {.year = 2020, .month = 1, .day = 1};
    Database database;
    database.add({.description = "sample task", .priority = 1}, date);

    auto tasks = database.at(date);
    REQUIRE(tasks.size() == 1);

    const auto& task = tasks.front();
    REQUIRE(task.description == "sample task");
    REQUIRE(task.priority == 1);
}

TEST_CASE("sorted")
{
    const Date date = {.year = 2020, .month = 1, .day = 1};
    Database database;
    database.add({.description = "low prio", .priority = 1}, date);
    database.add({.description = "high prio", .priority = 10}, date);
    database.add({.description = "medium prio", .priority = 5}, date);

    auto tasks = database.at(date);
    REQUIRE(std::is_sorted(tasks.begin(), tasks.end(), std::greater<>()));
}

TEST_CASE("remove task")
{
    const Date date = {.year = 2020, .month = 2, .day = 2};
    Database database;
    database.add({.description = "some task", .priority = 1}, date);
    database.add({.description = "some other task", .priority = 2}, date);
    database.remove(database.at(date).at(0).hash);
    auto tasks = database.at(date);
    REQUIRE(tasks.size() == 1);
    REQUIRE(tasks.front().description == "some task");
}

TEST_CASE("check task")
{
    const Date date = {.year = 2020, .month = 2, .day = 2};
    Database database;
    database.add({.description = "some task", .priority = 1}, date);
    database.add({.description = "some other task", .priority = 2}, date);
    database.check(database.at(date).front().hash);
    auto tasks = database.at(date);
    REQUIRE(tasks.front().description == "some other task");
    REQUIRE(tasks.front().done);
    REQUIRE_FALSE(tasks.at(1).done);
}

TEST_CASE("get task")
{
    const Date date = {.year = 2020, .month = 2, .day = 2};
    Database database;
    database.add({.description = "some task", .priority = 1}, date);
    auto tasks = database.at(date);

    REQUIRE(database.get(tasks.front().hash)->description == "some task");
    REQUIRE_FALSE(database.get(42).has_value());
}

TEST_CASE("move task")
{
    const Date initialDate = {.year = 2020, .month = 2, .day = 2};
    const Date newDate = {.year = 2020, .month = 2, .day = 3};
    Database database;
    database.add({.description = "some task", .priority = 1}, initialDate);
    uint32_t hash = database.at(initialDate).front().hash;
    database.move(hash, newDate);
    REQUIRE(database.at(initialDate).empty());
    REQUIRE(database.at(newDate).front().description == "some task");
}

TEST_CASE("input file")
{
    auto path = std::filesystem::temp_directory_path().append("database.bin");
    if (std::filesystem::exists(path))
        std::filesystem::remove(path);

    const Date date = {.year = 2022, .month = 3, .day = 4};
    {
        Database database(path.c_str());
        database.add({.description = "test task"}, date);
    }
    auto database = Database(path.c_str());

    auto tasks = database.at(date);
    REQUIRE(tasks.size() == 1);

    const auto& task = tasks.front();
    REQUIRE(task.description == "test task");
    REQUIRE(task.priority == 0);
}

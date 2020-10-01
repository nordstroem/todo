#include "Database.hpp"
#include <doctest.h>
#include <filesystem>

using namespace todo;

TEST_CASE("add and at")
{
    const Date date = {.year = 2020, .month = 1, .day = 1};
    Database database;
    database.add({.task = "sample task", .priority = 1}, date);

    auto tasks = database.at(date);
    REQUIRE(tasks.size() == 1);

    const auto& [task, priority] = tasks[0];
    REQUIRE(task == "sample task");
    REQUIRE(priority == 1);
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

    const auto& [task, priority] = tasks[0];
    REQUIRE(task == "test task");
    REQUIRE(priority == 0);
}
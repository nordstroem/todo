#include "Database.hpp"
#include <algorithm>
#include <doctest/doctest.h>
#include <filesystem>
#include <fmt/core.h>

using namespace todo;

TEST_CASE("add and at")
{
    const Date date = {.year = 2020, .month = 1, .day = 1};
    Database database;
    database.add("sample task", date, 1);

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
    database.add("low prio", date, 1);
    database.add("high prio", date, 10);
    database.add("medium prio", date, 5);

    auto tasks = database.at(date);
    REQUIRE(std::is_sorted(tasks.begin(), tasks.end(), [](const auto& a, const auto& b) { return a.priority > b.priority; }));
}

TEST_CASE("remove task")
{
    const Date date = {.year = 2020, .month = 2, .day = 2};
    Database database;
    database.add("some task", date, 1);
    database.add("some other task", date, 2);
    database.remove(database.at(date).at(0).hash);
    auto tasks = database.at(date);
    REQUIRE(tasks.size() == 1);
    REQUIRE(tasks.front().description == "some task");
}

TEST_CASE("check task")
{
    const Date date = {.year = 2020, .month = 2, .day = 2};
    Database database;
    database.add("some task", date, 1);
    database.add("some other task", date, 2);
    database.check(database.at(date).front().hash);
    auto tasks = database.at(date);
    REQUIRE(tasks.front().description == "some other task");
    REQUIRE(tasks.front().done());
    REQUIRE_FALSE(tasks.at(1).done());
}

TEST_CASE("check task on other")
{
    const Date date = {.year = 2020, .month = 2, .day = 2};
    Database database;
    database.add("some task", date, 1);
    database.add("some other task", date, 2);
    database.check(database.at(date).front().hash);
    auto tasks = database.at(date);
    REQUIRE(tasks.front().description == "some other task");
    REQUIRE(tasks.front().done());
    REQUIRE_FALSE(tasks.at(1).done());
}

TEST_CASE("at also returns unfinished older tasks")
{
    const Date firstDate = {.year = 2020, .month = 2, .day = 2};
    const Date secondDate = {.year = 2020, .month = 2, .day = 3};
    const Date thirdDate = {.year = 2020, .month = 2, .day = 4};
    Database database;
    const uint32_t firstHash = database.add("first task", firstDate, 4);
    const uint32_t secondHash = database.add("second task", secondDate, 3);
    const uint32_t thirdHash = database.add("third task", thirdDate, 1);

    database.check(secondHash);

    const auto tasks = database.at(thirdDate);
    REQUIRE(tasks.size() == 2);
    REQUIRE(tasks.at(0).hash == firstHash);
    REQUIRE(tasks.at(1).hash == thirdHash);
}

TEST_CASE("get task")
{
    const Date date = {.year = 2020, .month = 2, .day = 2};
    Database database;
    database.add("some task", date, 1);
    auto tasks = database.at(date);

    REQUIRE(database.get(tasks.front().hash)->description == "some task");
    REQUIRE_FALSE(database.get(42).has_value());
}

TEST_CASE("get undone tasks")
{
    const Date firstDate = {.year = 2020, .month = 2, .day = 1};
    const Date secondDate = {.year = 2020, .month = 2, .day = 2};
    Database database;
    database.add("first task", firstDate, 3);
    const uint32_t firstHash = database.at(firstDate).front().hash;
    database.check(firstHash);
    database.add("second task", firstDate, 5);
    database.add("third task", secondDate, 3);
    database.add("fourth task", secondDate, 4);

    const auto undone = database.undone();
    REQUIRE(undone.size() == 3);
    REQUIRE(undone.at(0).description == "second task");
    REQUIRE(undone.at(1).description == "fourth task");
    REQUIRE(undone.at(2).description == "third task");
}

TEST_CASE("move task")
{
    const Date initialDate = {.year = 2020, .month = 2, .day = 2};
    const Date newDate = {.year = 2020, .month = 2, .day = 3};
    Database database;
    database.add("some task", initialDate, 1);
    const uint32_t hash = database.at(initialDate).front().hash;
    database.move(hash, newDate);
    REQUIRE(database.at(initialDate).empty());
    REQUIRE(database.at(newDate).front().description == "some task");
}

TEST_CASE("input file")
{
    const auto path = std::filesystem::temp_directory_path().append("database.bin");
    if (std::filesystem::exists(path))
        std::filesystem::remove(path);

    const Date date = {.year = 2022, .month = 3, .day = 4};
    {
        Database database(path.c_str());
        database.add("test task", date, 0);
    }
    auto database = Database(path.c_str());

    const auto tasks = database.at(date);
    REQUIRE(tasks.size() == 1);

    const auto& task = tasks.front();
    REQUIRE(task.description == "test task");
    REQUIRE(task.priority == 0);
}

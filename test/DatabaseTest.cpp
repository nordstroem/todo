#include "Database.hpp"
#include <algorithm>
#include <doctest/doctest.h>
#include <filesystem>
#include <fmt/core.h>

using namespace todo;

TEST_CASE("add and withDueDate")
{
    const Date date = {.year = 2020, .month = 1, .day = 1};
    Database database;
    database.add("sample task", date, Priority::Medium);

    auto tasks = database.withDueDate(date);
    REQUIRE(tasks.size() == 1);

    const auto& task = tasks.front();
    REQUIRE(task.description == "sample task");
    REQUIRE(task.priority == Priority::Medium);
}

TEST_CASE("sorted")
{
    const Date date = {.year = 2020, .month = 1, .day = 1};
    Database database;
    database.add("low prio", date, Priority::Low);
    database.add("high prio", date, Priority::High);
    database.add("medium prio", date, Priority::Medium);

    auto tasks = database.withDueDate(date);
    REQUIRE(std::is_sorted(tasks.begin(), tasks.end(), [](const auto& a, const auto& b) { return a.priority > b.priority; }));
}

TEST_CASE("remove task")
{
    const Date date = {.year = 2020, .month = 2, .day = 2};
    Database database;
    database.add("some task", date, Priority::Medium);
    database.add("some other task", date, Priority::High);
    database.remove(database.withDueDate(date).at(0).hash);
    auto tasks = database.withDueDate(date);
    REQUIRE(tasks.size() == 1);
    REQUIRE(tasks.front().description == "some task");
}

TEST_CASE("check task")
{
    const Date date = {.year = 2020, .month = 2, .day = 2};
    Database database;
    database.add("some task", date, Priority::Medium);
    database.add("some other task", date, Priority::High);
    database.check(database.withDueDate(date).front().hash);
    auto tasks = database.withDueDate(date);
    REQUIRE(tasks.front().description == "some other task");
    REQUIRE(tasks.front().done());
    REQUIRE_FALSE(tasks.at(1).done());
}

TEST_CASE("check task on other")
{
    const Date date = {.year = 2020, .month = 2, .day = 2};
    Database database;
    database.add("some task", date, Priority::Medium);
    database.add("some other task", date, Priority::High);
    database.check(database.withDueDate(date).front().hash);
    auto tasks = database.withDueDate(date);
    REQUIRE(tasks.front().description == "some other task");
    REQUIRE(tasks.front().done());
    REQUIRE_FALSE(tasks.at(1).done());
}

TEST_CASE("with done date")
{
    const Date firstDate = {.year = 2020, .month = 2, .day = 2};
    const Date secondDate = {.year = 2020, .month = 2, .day = 3};
    const Date thirdDate = {.year = 2020, .month = 2, .day = 4};
    Database database;
    database.add("first task", firstDate, Priority::Medium);
    database.add("second task", secondDate, Priority::Medium);
    database.add("third task", thirdDate, Priority::Medium);
    database.check(database.withDueDate(secondDate).front().hash);

    const auto tasks = database.withDoneDate(Date::today());
    REQUIRE(tasks.size() == 1);
    REQUIRE(tasks.at(0).description == "second task");
}

TEST_CASE("get task")
{
    const Date date = {.year = 2020, .month = 2, .day = 2};
    Database database;
    database.add("some task", date, Priority::Medium);
    auto tasks = database.withDueDate(date);

    REQUIRE(database.get(tasks.front().hash)->description == "some task");
    REQUIRE_FALSE(database.get(42).has_value());
}

TEST_CASE("get undone tasks")
{
    const Date firstDate = {.year = 2020, .month = 2, .day = 1};
    const Date secondDate = {.year = 2020, .month = 2, .day = 2};
    Database database;
    database.add("first task", firstDate, Priority::Medium);
    const uint32_t firstHash = database.withDueDate(firstDate).front().hash;
    database.check(firstHash);
    database.add("second task", firstDate, Priority::High);
    database.add("third task", secondDate, Priority::Low);
    database.add("fourth task", secondDate, Priority::Medium);

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
    database.add("some task", initialDate, Priority::Medium);
    const uint32_t hash = database.withDueDate(initialDate).front().hash;
    database.move(hash, newDate);
    REQUIRE(database.withDueDate(initialDate).empty());
    REQUIRE(database.withDueDate(newDate).front().description == "some task");
}

TEST_CASE("input file")
{
    const auto path = std::filesystem::temp_directory_path().append("database.bin");
    if (std::filesystem::exists(path))
        std::filesystem::remove(path);

    const Date date = {.year = 2022, .month = 3, .day = 4};
    {
        Database database(path.string().c_str());
        database.add("test task", date, Priority::Medium);
    }
    auto database = Database(path.string().c_str());

    const auto tasks = database.withDueDate(date);
    REQUIRE(tasks.size() == 1);

    const auto& task = tasks.front();
    REQUIRE(task.description == "test task");
    REQUIRE(task.priority == Priority::Medium);
}

TEST_CASE("get undone tasks up to date")
{
    const Date firstDate = {.year = 2020, .month = 2, .day = 1};
    const Date secondDate = {.year = 2020, .month = 2, .day = 2};
    const Date thirdDate = {.year = 2020, .month = 2, .day = 3};

    Database database;
    database.add("first task", firstDate, Priority::Medium);
    database.add("second task", secondDate, Priority::Medium);
    database.add("third task", secondDate, Priority::High);
    database.add("fourth task", thirdDate, Priority::Medium);
    database.check(1);

    const auto undone = database.undoneUpToDueDate(secondDate);
    REQUIRE(undone.size() == 2);
    REQUIRE(undone.at(0).description == "third task");
    REQUIRE(undone.at(1).description == "first task");
}

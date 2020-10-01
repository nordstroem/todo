#include "Database.hpp"
#include <cereal/archives/binary.hpp>
#include <doctest.h>
#include <fmt/core.h>
#include <sstream>

using namespace todo;

TEST_CASE("add and at")
{
    const Date date = {.year = 2020, .month = 1, .day = 1};
    Database database;
    database.add({.task = "sample task", .priority = 1}, date);
    for (const auto& [task, priority] : database.at(date)) {
        REQUIRE(task == "sample task");
        REQUIRE(priority == 1);
    }
}

TEST_CASE("serialize")
{
    const Date date = {.year = 2020, .month = 1, .day = 1};
    Database serializedDatabase;
    std::stringstream s;
    {
        Database database;
        database.add({.task = "sample task", .priority = 2}, date);
        cereal::BinaryOutputArchive oarchive(s);
        oarchive(database);
    }
    {
        cereal::BinaryInputArchive iarchive(s);
        iarchive(serializedDatabase);
    }
    for (const auto& [task, priority] : serializedDatabase.at(date)) {
        REQUIRE(task == "sample task");
        REQUIRE(priority == 2);
    }
}

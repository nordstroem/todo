#include "todo/Database.hpp"
#include <doctest.h>
#include <fmt/core.h>
#include <sstream>

using namespace todo;

TEST_CASE("add and at")
{
    const auto date = date::year{2020} / 1 / 1;
    Database database;
    database.add({.task = "sample task", .priority = 1}, date);
    for (const auto& [task, priority] : database.at(date)) {
        REQUIRE(task == "sample task");
        REQUIRE(priority == 1);
    }
}

TEST_CASE("serialize")
{
    /*std::stringstream s;
    const auto date = date::year{2020} / 2 / 2;
    Database serializedDatabase;
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
        REQUIRE(priority == 1);
    }
    */
}

#include "todo/Database.hpp"
#include <doctest.h>
#include <fmt/core.h>

using namespace todo;

TEST_CASE("database add and at")
{
    Database database;
    database.add({.task = "hejsan", .priority = 1}, Date("123"));

    for (const auto& val : database.at(Date("123"))) {
        fmt::print("{}\n", val.task);
    }
}

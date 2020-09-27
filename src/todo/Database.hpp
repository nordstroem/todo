#pragma once
#include "Date.hpp"
#include <queue>
#include <string>
#include <string_view>
#include <unordered_map>

namespace todo {

class Database
{
public:
    /// Constructs an empty database
    Database() = default;
    /**
     * Constructs a database that is linked to a database file
     * @param file the path to the database file 
     */
    Database(std::string_view file);
    /**
     * Adds a task to the database
     * @param task 
     * @param date 
     * @param priority 
     */
    void add(std::string_view task, Date date, int priority);

private:
    std::unordered_map<int, std::priority_queue<std::string>> _tasks;
};

} // namespace todo

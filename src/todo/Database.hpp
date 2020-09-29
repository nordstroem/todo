#pragma once
#include "Date.hpp"
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace todo {

struct Task
{
    std::string task;
    int priority = 0;
    bool operator<(const Task& other) const { return this->priority < other.priority; }
};

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
     * 
     * @param task 
     * @param date 
     */
    void add(Task&& task, Date date);
    /**
     * 
     * @param date 
     * @return 
     */
    std::vector<Task> at(Date date) const;

private:
    std::unordered_map<int, std::vector<Task>> _tasks;
};

} // namespace todo

#pragma once
#include "Date.hpp"
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

namespace todo {

struct Task
{
    std::string task;
    int priority = 0;

public:
    bool operator>(const Task& other) const { return this->priority > other.priority; }
};

class Database
{
public:
    /// Constructs an empty database
    Database() = default;
    /// Destructs the database, and updates the linked database file
    ~Database();

    // Do not allow copying
    Database(const Database& other) = delete;
    Database& operator=(const Database& other) = delete;

    /**
     * Constructs a database that is linked to a binary database file
     * @param file the path to the database file 
     */
    Database(const std::string& file);
    /**
     * Adds a task to do at a specific date to the database
     * @param task task to add
     * @param date date to be added
     */
    void add(Task&& task, const Date& date);
    /**
     * Returns all tasks at a specific date
     * @param date the date
     * @return the tasks
     * @remark this vector is sorted from highest to lowest task priority
     */
    std::vector<Task> at(const Date& date) const;

private:
    std::unordered_map<Date, std::vector<Task>> _tasks;
    std::optional<std::string> _file;
};

} // namespace todo

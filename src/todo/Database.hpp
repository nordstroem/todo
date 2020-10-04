#pragma once
#include "Date.hpp"
#include "Task.hpp"
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

namespace todo {

class Database
{
public:
    /// Constructs an empty database
    Database() = default;
    /**
     * Constructs a database that is linked to a binary database file
     * @param file the path to the database file 
     */
    explicit Database(const std::string& file);
    /// Destructs the database, and updates the linked database file
    ~Database();
    // Do not allow copying or moving
    Database(const Database& other) = delete;
    Database(Database&& other) = delete;
    Database& operator=(const Database& other) = delete;
    Database& operator=(Database&& other) = delete;
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
    std::vector<HashedTask> at(const Date& date) const;

private:
    std::unordered_map<Date, std::vector<HashedTask>> _tasks;
    std::optional<std::string> _file;
};

} // namespace todo

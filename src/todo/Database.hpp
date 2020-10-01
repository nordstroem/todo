#pragma once
#include "Date.hpp"
#include <cereal/types/string.hpp>
#include <cereal/types/unordered_map.hpp>
#include <cereal/types/vector.hpp>
#include <functional>
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

    template <class Archive>
    void serialize(Archive& archive)
    {
        archive(task, priority);
    }
};

class Database
{
public:
    /// Constructs an empty database
    Database() = default;
    /// Destructs the database, and updates the linked database file
    ~Database();
    /**
     * Constructs a database that is linked to a database file
     * @param file the path to the database file 
     */
    Database(std::string_view file);
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
     */
    std::vector<Task> at(const Date& date) const;

public:
    template <class Archive>
    void serialize(Archive& archive)
    {
        archive(_tasks);
    }

private:
    std::unordered_map<Date, std::vector<Task>> _tasks;
    std::string _file;
};

} // namespace todo

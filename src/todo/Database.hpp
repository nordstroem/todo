#pragma once
#include <cereal/archives/binary.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/unordered_map.hpp>
#include <date/date.h>
#include <functional>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace std {
template <>
struct hash<date::year_month_day>
{
    std::size_t operator()(const date::year_month_day& date) const noexcept
    {
        return std::hash<int>{}(date::sys_days{date}.time_since_epoch().count());
    }
};
} // namespace std

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
    void add(Task&& task, const date::year_month_day& date);
    /**
     * Returns all tasks at a specific date
     * @param date the date
     * @return the tasks
     */
    std::vector<Task> at(const date::year_month_day& date) const;

public:
    template <class Archive>
    void serialize(Archive& archive)
    {
        archive(this->_tasks);
    }

private:
    std::unordered_map<date::year_month_day, std::vector<Task>> _tasks;
};

} // namespace todo

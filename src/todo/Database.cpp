#include "Database.hpp"

namespace todo {

Database::Database([[maybe_unused]] std::string_view file)
{
}

void Database::add(Task&& task, Date date)
{
    this->_tasks[date.daysSinceEpoch()].push_back(std::move(task));
}

std::vector<Task> Database::at(Date date) const
{
    if (auto task = this->_tasks.find(date.daysSinceEpoch()); task != this->_tasks.end())
        return task->second;
    return {};
}

} // namespace todo

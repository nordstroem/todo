#include "Database.hpp"

namespace todo {

Database::Database([[maybe_unused]] std::string_view file)
{
}

void Database::add(Task&& task, const date::year_month_day& date)
{
    this->_tasks[date].push_back(std::move(task));
}

std::vector<Task> Database::at(const date::year_month_day& date) const
{
    if (auto task = this->_tasks.find(date); task != this->_tasks.end())
        return task->second;
    return {};
}

} // namespace todo

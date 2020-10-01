#include "Database.hpp"

namespace todo {

Database::Database(std::string_view file)
    : _file(file)
{
}

Database::~Database()
{
}

void Database::add(Task&& task, const Date& date)
{
    this->_tasks[date].push_back(std::move(task));
}

std::vector<Task> Database::at(const Date& date) const
{
    if (auto task = this->_tasks.find(date); task != this->_tasks.end())
        return task->second;
    return {};
}

} // namespace todo

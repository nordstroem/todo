#pragma once
#include <string>

namespace todo {

struct Task
{
    std::string description;
    int priority = 0;

public:
    bool operator>(const Task& other) const { return this->priority > other.priority; }
};

struct HashedTask : public Task
{
public:
    HashedTask() noexcept = default;
    HashedTask(Task&& task, uint32_t hash) noexcept
    {
        this->description = std::move(task.description);
        this->priority = task.priority;
        this->hash = hash;
    }

public:
    uint32_t hash;
};

} // namespace todo

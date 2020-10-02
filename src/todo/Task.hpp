#pragma once
#include <string>

namespace todo {

struct Task
{
    std::string task;
    int priority = 0;

public:
    bool operator>(const Task& other) const { return this->priority > other.priority; }
};

} // namespace todo

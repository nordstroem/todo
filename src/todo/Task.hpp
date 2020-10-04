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

} // namespace todo

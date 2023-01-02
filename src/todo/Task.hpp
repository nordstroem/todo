#pragma once
#include "Date.hpp"
#include <optional>
#include <string>

namespace todo {

enum class Priority
{
    Low,
    Medium,
    High
};

struct Task
{
    std::string description;
    Date dueDate;
    Priority priority = Priority::Medium;
    uint32_t hash = 0;
    Date addedDate = Date::today();
    std::optional<Date> doneDate = std::nullopt;

    bool done() const { return doneDate.has_value(); }
};

} // namespace todo

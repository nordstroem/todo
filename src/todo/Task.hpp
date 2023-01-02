#pragma once
#include "Date.hpp"
#include <optional>
#include <string>

namespace todo {

struct Task
{
    std::string description;
    Date dueDate;
    int32_t priority = 0;
    uint32_t hash = 0;
    Date addedDate = Date::today();
    std::optional<Date> doneDate = std::nullopt;

    bool done() const { return doneDate.has_value(); }
};

} // namespace todo

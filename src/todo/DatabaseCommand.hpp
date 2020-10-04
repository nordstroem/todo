#pragma once
#include "Date.hpp"
#include "Task.hpp"
#include <string>
#include <variant>

namespace todo {

// clang-format off
struct AddTask { Task task; Date date; };
struct ShowMessage { std::string message; };
struct ShowTasks { Date date; };
struct DoNothing { };
// clang-format on

using DatabaseCommand = std::variant<AddTask, ShowMessage, ShowTasks, DoNothing>;

DatabaseCommand parse(int argc, const char** argv);

} // namespace todo

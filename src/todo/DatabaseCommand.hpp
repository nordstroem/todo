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
struct RemoveTask { uint32_t hash; };
struct CheckTask { uint32_t hash; };
struct MoveTask { uint32_t hash; Date date; };
struct DoNothing { };
// clang-format on

using DatabaseCommand = std::variant<AddTask, ShowMessage, ShowTasks, RemoveTask, CheckTask, MoveTask, DoNothing>;

DatabaseCommand parse(int argc, const char** argv);

} // namespace todo

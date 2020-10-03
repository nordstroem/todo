#pragma once
#include "Date.hpp"
#include "Task.hpp"
#include <array>
#include <optional>
#include <string>

namespace todo {

// @todo rename
struct InputSettings
{
    std::optional<std::string> message;
    std::optional<Task> task;
    std::optional<Date> date;
};

/**
 * Generates input settings from command line arguments
 * @param argc the number of arguments
 * @param argv the arguments
 * @return the input settings
 */
InputSettings parse(int argc, const char** argv);

} // namespace todo

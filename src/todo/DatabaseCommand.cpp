#include "DatabaseCommand.hpp"
#include <fmt/core.h>
#include <magic_enum.hpp>
using namespace magic_enum::istream_operators;
#include <cxxopts.hpp>

namespace todo {

DatabaseCommand parse(std::span<const char*> args)
{
    cxxopts::Options options("todo", "todo");
    options.add_options()                                                                                 //
            ("h,help", "Print usage")                                                                     //
            ("a,add", "Task to add", cxxopts::value<std::string>())                                       //
            ("p,priority", "Priority of added task", cxxopts::value<Priority>()->default_value("Medium")) //
            ("r,remove", "Hash of task to remove", cxxopts::value<uint32_t>())                            //
            ("c,check", "Check a task with a specific hash", cxxopts::value<uint32_t>())                  //
            ("m,move", "Move a task with a specific hash to another date", cxxopts::value<uint32_t>())    //
            ("s,show", "Show tasks at a specific date")                                                   //
            ("u,show_undone", "Show all undone tasks")                                                    //
            ("d,date", "Date to add or query", cxxopts::value<std::string>()->default_value("today"));    //
    options.parse_positional({"date"});
    options.positional_help("Date");
    options.show_positional_help();

    try {
        const auto result = options.parse(static_cast<int>(args.size()), &args.front());

        if (result.count("help") != 0)
            return ShowMessage{.message = options.help()};

        const Date date = Date::fromString(result["date"].as<std::string>());

        if (!date.valid())
            return ShowMessage{.message = fmt::format("{} is not a valid date", date.toString())};

        if (result.count("add") != 0) {
            Task task = Task{.description = result["add"].as<std::string>(), .dueDate = date, .priority = result["priority"].as<Priority>()};
            return AddTask{.task = std::move(task), .date = date};
        }

        if (result.count("remove") != 0)
            return RemoveTask{.hash = result["remove"].as<std::uint32_t>()};

        if (result.count("check") != 0)
            return CheckTask{.hash = result["check"].as<std::uint32_t>()};

        if (result.count("move") != 0)
            return MoveTask{.hash = result["move"].as<std::uint32_t>(), .date = date};

        if (result.count("show_undone") != 0)
            return ShowUndoneTasks{};

        if (result.count("show") != 0 || result.arguments().size() <= 1)
            return ShowTasks{.date = date};

    } catch (const std::exception& exception) {
        return ShowMessage{.message = exception.what()};
    }

    return DoNothing{};
}

} // namespace todo

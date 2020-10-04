#include "DatabaseCommand.hpp"
#include <cxxopts.hpp>
#include <fmt/core.h>

namespace todo {

DatabaseCommand parse(int argc, const char** argv)
{
    cxxopts::Options options("todo", "todo");
    options.add_options()                                                                              //
            ("h,help", "Print usage")                                                                  //
            ("a,add", "Task to add", cxxopts::value<std::string>())                                    //
            ("p,priority", "Priority of added task", cxxopts::value<int>()->default_value("0"))        //
            ("r,remove", "Hash of task to remove", cxxopts::value<uint32_t>())                         //
            ("s,show", "Show tasks at a specific date")                                                //
            ("d,date", "Date to add or query", cxxopts::value<std::string>()->default_value("today")); //

    try {
        auto result = options.parse(argc, argv);
        if (result.count("help") != 0)
            return ShowMessage{.message = options.help()};

        Date date = Date::fromString(result["date"].as<std::string>());

        if (result.count("add") != 0) {
            Task task = Task{.description = result["add"].as<std::string>(), .priority = result["priority"].as<int>()};
            return AddTask{.task = std::move(task), .date = date};
        }

        if (result.count("remove") != 0)
            return RemoveTask{.hash = result["remove"].as<std::uint32_t>()};

        if (result.count("show") != 0)
            return ShowTasks{.date = date};

    } catch (const std::exception& exception) {
        return ShowMessage{.message = exception.what()};
    }

    return DoNothing{};
}

} // namespace todo

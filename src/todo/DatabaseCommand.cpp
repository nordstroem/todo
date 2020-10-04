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
            ("p,priority", "Priority of the task", cxxopts::value<int>()->default_value("0"))          //
            ("s,show", "Show task at a specific date")                                                 //
            ("d,date", "Date to add or query", cxxopts::value<std::string>()->default_value("today")); //

    auto result = options.parse(argc, argv);

    if (result.count("help") != 0)
        return ShowMessage{.message = options.help()};

    auto date = Date::fromString(result["date"].as<std::string>());
    if (!date.has_value())
        return ShowMessage{.message = fmt::format("Date specificer is ill-formed, it must be the format yyyy-mm-dd or {} \n", R"("today")")};

    if (result.count("add") != 0) {
        Task task = Task{.task = result["add"].as<std::string>(), .priority = result["priority"].as<int>()};
        return AddTask{.task = std::move(task), .date = *date};
    }

    if (result.count("show") != 0)
        return ShowTasks{.date = *date};

    return DoNothing{};
}

} // namespace todo

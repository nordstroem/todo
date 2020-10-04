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
            ("d,date", "Date to add or query", cxxopts::value<std::string>()->default_value("today")); //

    auto result = options.parse(argc, argv);

    if (result.count("help") != 0)
        return ShowMessage{.message = options.help()};

    if (result.count("add") != 0) {
        if (auto date = Date::fromString(result["date"].as<std::string>())) {
            Task task = Task{.task = result["add"].as<std::string>(), .priority = result["priority"].as<int>()};
            return AddTask{.task = std::move(task), .date = *date};
        }
        return ShowMessage{.message = fmt::format("Date specificer is ill-formed, it must be the format yyyy-mm-dd or {} \n", R"("today")")};
    }

    return DoNothing{};
}

} // namespace todo

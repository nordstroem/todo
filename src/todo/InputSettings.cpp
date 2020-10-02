#include "InputSettings.hpp"
#include <cxxopts.hpp>

namespace todo {

InputSettings parse(int argc, const char* argv[])
{
    cxxopts::Options options("todo", "todo");
    options.add_options()                                                                     //
            ("h,help", "Print usage")                                                         //
            ("a,add", "Task to add", cxxopts::value<std::string>())                           //
            ("p,priority", "Priority of the task", cxxopts::value<int>()->default_value("0")) //
            ("d,date", "Date to add or query", cxxopts::value<std::string>());                //

    auto result = options.parse(argc, argv);

    InputSettings input;
    if (result.count("help")) {
        input.message = options.help();
        return input;
    }

    if (result.count("add"))
        input.task = {.task = result["add"].as<std::string>(), .priority = result["priority"].as<int>()};

    if (result.count("date"))
        input.date = Date::fromString(result["date"].as<std::string>());

    return input;
}

} // namespace todo

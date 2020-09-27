#include "InputSettings.hpp"
#include <cxxopts.hpp>
#include <fmt/core.h>

namespace todo {

InputSettings parse(int argc, const char* argv[])
{
    cxxopts::Options options("todo", "todo");
    options.add_options()             //
            ("h,help", "Print usage") //
            ("d,delete", "kek");      //

    auto result = options.parse(argc, argv);

    if (result.count("help")) {
        fmt::print("{}\n", options.help());
        exit(0);
    }

    return {.a = false, .b = true, .c = false};
}

} // namespace todo

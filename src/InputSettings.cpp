#include "InputSettings.hpp"
#include <cxxopts.hpp>

namespace todo {

InputSettings parse(int argc, const char* argv[])
{
    cxxopts::Options options("todo", "todo");
    options.add_options()("h,help", "Print usage");

    auto result = options.parse(argc, argv);

    if (result.count("help")) {
        std::cout << options.help() << "\n";
        exit(0);
    }

    return { .a = false, .b = true, .c = false };
}

}

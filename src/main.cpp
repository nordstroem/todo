#include "InputSettings.hpp"
#include <fmt/core.h>
#include <spdlog/spdlog.h>

int main(int argc, const char* argv[])
{
    auto settings = todo::parse(argc, argv);
    if (auto str = settings.message) {
        fmt::print("{}", *str);
        exit(0);
    }
    spdlog::info("Welcome to spdlog!");
    fmt::print("{}\n", "hej");
}

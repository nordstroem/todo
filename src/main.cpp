#include "todo/InputSettings.hpp"
#include <cereal/archives/binary.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/unordered_map.hpp>
#include <fmt/core.h>
#include <spdlog/spdlog.h>

int main(int argc, const char* argv[])
{
    [[maybe_unused]] auto settings = todo::parse(argc, argv);
    spdlog::info("Welcome to spdlog!");
    fmt::print("{}\n", "hej");
}

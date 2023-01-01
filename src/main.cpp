#include "DatabaseCommandVisitor.hpp"
#include <cstdlib>
#include <spdlog/spdlog.h>

int main(int argc, const char* argv[]) // NOLINT
{
    const auto* path = std::getenv("TODO_DATABASE_PATH");
    if (path == nullptr) {
        spdlog::error("Environment variable TODO_DATABASE_PATH not set");
        return 1;
    }
    const std::span<const char*> args(argv, argv + argc);
    auto command = todo::parse(args);
    std::visit(todo::DatabaseCommandVisitor{path}, std::move(command));
}

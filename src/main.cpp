#include "DatabaseCommandVisitor.hpp"
#include <cstdlib>
#include <spdlog/spdlog.h>

using namespace todo;

int main(int argc, const char* argv[])
{
    const auto path = std::getenv("TODO_DATABASE_PATH");
    if (path != nullptr) {
        auto command = todo::parse(argc, argv);
        std::visit(DatabaseCommandVisitor{path}, std::move(command));
    } else {
        spdlog::error("Environment variable TODO_DATABASE_PATH not set");
    }
}

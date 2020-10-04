#include "Database.hpp"
#include "DatabaseCommandVisitor.hpp"
#include <cstdlib>
#include <fmt/core.h>
#include <spdlog/spdlog.h>
#include <string_view>

using namespace todo;

int main(int argc, const char* argv[])
{
    const auto path = std::getenv("TODO_DATABASE_PATH");
    auto command = todo::parse(argc, argv);
    std::visit(DatabaseCommandVisitor{path}, std::move(command));
}

#include "Database.hpp"
#include "DatabaseCommandVisitor.hpp"
#include <cstdlib>
#include <fmt/core.h>
#include <spdlog/spdlog.h>
#include <string_view>

using namespace todo;

int main(int argc, const char* argv[])
{
    const auto command = todo::parse(argc, argv);

    //todo::Command c = todo::ShowMessage{"HejKek!\n"};
    //std::visit(DatabaseCommandVisitor{}, std::move(c));

    const auto path = std::getenv("TODO_DATABASE_PATH");
    spdlog::info("{}\n", path);
}

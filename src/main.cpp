#include "DatabaseCommandVisitor.hpp"
#include <cstdlib>

using namespace todo;

int main(int argc, const char* argv[])
{
    const auto path = std::getenv("TODO_DATABASE_PATH");
    auto command = todo::parse(argc, argv);
    std::visit(DatabaseCommandVisitor{path}, std::move(command));
}

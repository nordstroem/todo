#include <cxxopts.hpp>
#include <iostream>

int main(int argc, const char* argv[])
{
    cxxopts::Options options("MyProgram", "One line description of MyProgram");
    options.add_options()("d,debug", "Enable debugging");

    auto result = options.parse(argc, argv);
    bool debug = result["debug"].as<bool>();
    std::cout << debug << "\n";
}

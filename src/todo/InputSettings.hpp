#pragma once

namespace todo {

struct InputSettings
{
    bool a = false;
    bool b = false;
    bool c = false;
};

/**
 * Generates input settings from command line arguments
 * @param argc the number of arguments
 * @param argv the arguments
 * @return the input settings
 * @note this function exits the program if --help is an argument
 */
InputSettings parse(int argc, const char* argv[]);

} // namespace todo

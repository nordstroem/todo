#pragma once
#include <string_view>

namespace todo {

class Date
{
public:
    Date(std::string_view date);

public:
    static Date now();
    static Date tomorrow();

private:
};
} // namespace todo

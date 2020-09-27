#pragma once
#include <string_view>

namespace todo {

class Date
{
public:
    explicit Date(std::string_view date);

    // @return
    size_t ticks() const noexcept;

private:
    size_t _representation = 0;
};

} // namespace todo

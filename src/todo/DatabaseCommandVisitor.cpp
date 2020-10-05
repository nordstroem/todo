#include "DatabaseCommandVisitor.hpp"
#include <algorithm>
#include <fmt/color.h>
#include <fmt/core.h>

namespace {
template <typename T>
std::string toString(const T& t)
{
    if constexpr (std::is_same_v<T, std::string>)
        return t;
    else
        return std::to_string(t);
}

/**
 * 
 * @tparam Container 
 */
template <typename Container>
class MaxLengthHelper
{
public:
    explicit MaxLengthHelper(const Container& c)
        : _c(c)
    {}
    template <typename Transform>
    auto operator()(const Transform& t) const
    {
        auto compare = [&](const auto& a, const auto& b) { return toString(t(a)).length() < toString(t(b)).length(); };
        return toString(t(*std::max_element(this->_container.begin(), this->_container.end(), compare))).length();
    }

private:
    const Container& _container;
};
} // namespace

namespace todo {

DatabaseCommandVisitor::DatabaseCommandVisitor(std::string_view databasePath)
    : _database(Database(databasePath))
{
}

void DatabaseCommandVisitor::operator()(ShowMessage&& cmd) const
{
    fmt::print("{}\n", cmd.message);
}

void DatabaseCommandVisitor::operator()(ShowTasks&& cmd) const
{
    const auto& tasks = this->_database.at(cmd.date);
    const auto textColor = fg(fmt::color::indian_red);
    if (tasks.size() > 0) {
        MaxLengthHelper maxLength(tasks);
        constexpr std::array<std::string_view, 4> header = {"Hash", "Task", "Priority", "Done"};
        auto hashPadding = std::max(maxLength([](const auto& e) { return e.hash; }), header[0].length());
        auto descriptionPadding = std::max(maxLength([](const auto& e) { return e.description; }), header[1].length()) + 2;
        auto priorityPadding = std::max(maxLength([](const auto& e) { return e.priority; }), header[2].length());

        constexpr auto rowFormat = "{:>{}}  {:<{}}  {:<{}}  {}\n";
        fmt::print(textColor, "To do at {}:\n\n", cmd.date.toString());
        fmt::print(rowFormat, header[0], hashPadding, header[1], descriptionPadding, header[2], priorityPadding, header[3]);
        for (const auto& task : tasks) {
            auto checked = task.done ? fmt::format(fg(fmt::color::green), "[V]") : fmt::format("[ ]");
            fmt::print(rowFormat, task.hash, hashPadding, fmt::format(R"("{}")", task.description), descriptionPadding, task.priority, priorityPadding, checked);
        }
        fmt::print("\n");
    } else {
        fmt::print(textColor, "Nothing to do {}\n", cmd.date == Date::today() ? fmt::format("today") : fmt::format("at {}", cmd.date.toString()));
    }
}

void DatabaseCommandVisitor::operator()(AddTask&& cmd)
{
    this->_database.add(std::move(cmd.task), cmd.date);
}

void DatabaseCommandVisitor::operator()(RemoveTask&& cmd)
{
    this->_database.remove(cmd.hash);
}

void DatabaseCommandVisitor::operator()(CheckTask&& cmd)
{
    this->_database.check(cmd.hash);
}

void DatabaseCommandVisitor::operator()([[maybe_unused]] DoNothing&& cmd) const
{
}

const Database& DatabaseCommandVisitor::database() const noexcept
{
    return this->_database;
}

} // namespace todo

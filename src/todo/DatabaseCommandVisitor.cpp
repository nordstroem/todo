#include "DatabaseCommandVisitor.hpp"
#include <algorithm>
#include <fmt/color.h>
#include <fmt/core.h>
#include <ranges>
#include <string_view>

namespace ranges = std::ranges;
using namespace fmt;

namespace {

/**
 * Function object for calculating the maximum length of a string representation of the elements in a container
 * @tparam Container container type
 */
template <ranges::range Container>
class MaxLengthHelper
{
public:
    explicit MaxLengthHelper(const Container& container)
        : _container(container)
    {}
    /**
     * Calculates the maximum string representation length of the elements in the container
     * @param transform function that returns the string representation of an element
     * @return the maximum length, 0 if the container is empty
     */
    size_t operator()(const auto& transform) const
    {
        auto toStringLength = [&](const auto& e) { return std::string_view(transform(e)).length(); };
        if (!_container.empty()) {
            return ranges::max(_container | ranges::views::transform(toStringLength));
        }
        return 0;
    }

private:
    const Container& _container; // NOLINT
};
} // namespace

namespace todo {

DatabaseCommandVisitor::DatabaseCommandVisitor(std::string_view databasePath)
    : _database(Database(databasePath))
{
}

void DatabaseCommandVisitor::operator()(ShowMessage&& cmd) const
{
    print("{}\n", cmd.message);
}

void DatabaseCommandVisitor::operator()(ShowTasks&& cmd) const
{
    const auto& tasks = _database.at(cmd.date);
    if (!tasks.empty()) {
        const MaxLengthHelper maxLength(tasks);
        constexpr std::array<std::string_view, 4> header = {"Hash", "Task", "Priority", "Done"};
        const auto hashPadding = std::max(maxLength([](const auto& e) { return format("{}", e.hash); }), header[0].length());
        const auto taskPadding = std::max(maxLength([](const auto& e) { return format("\"{}\"", e.description); }), header[1].length());
        const auto prioPadding = std::max(maxLength([](const auto& e) { return format("{}", e.priority); }), header[2].length());

        constexpr auto rowFormat = "{:<{}}  {:<{}}  {:<{}}  {}\n";
        print("To do at {}:\n\n", cmd.date.toString());
        print(rowFormat, header[0], hashPadding, header[1], taskPadding, header[2], prioPadding, header[3]);
        for (const auto& task : tasks) {
            auto checked = task.done() ? format(fg(color::green), "V") : format(" ");
            print(rowFormat, task.hash, hashPadding, format("\"{}\"", task.description), taskPadding, task.priority, prioPadding, format("[{}]", checked));
        }
    } else {
        print("Nothing to do {}\n", cmd.date == Date::today() ? format("today") : format("at {}", cmd.date.toString()));
    }
}

void DatabaseCommandVisitor::operator()([[maybe_unused]] ShowUndoneTasks&& cmd) const
{
    const auto undone = _database.undone();
    if (!undone.empty()) {
        const MaxLengthHelper maxLength(undone);
        constexpr std::array<std::string_view, 5> header = {"Date", "Hash", "Task", "Priority"};
        const auto datePadding = std::max(maxLength([](const auto& e) { return format("{}", e.dueDate.toString()); }), header[0].length());
        const auto hashPadding = std::max(maxLength([](const auto& e) { return format("{}", e.hash); }), header[1].length());
        const auto taskPadding = std::max(maxLength([](const auto& e) { return format("\"{}\"", e.description); }), header[2].length());
        const auto prioPadding = std::max(maxLength([](const auto& e) { return format("{}", e.priority); }), header[3].length());

        print("Undone tasks:\n");
        constexpr auto rowFormat = "{:<{}}  {:<{}}  {:<{}}  {:<{}}\n";
        print(rowFormat, header[0], datePadding, header[1], hashPadding, header[2], taskPadding, header[3], prioPadding, header[4]);
        for (const auto& task : undone) {
            print(rowFormat, task.dueDate.toString(), datePadding, task.hash, hashPadding, format("\"{}\"", task.description), taskPadding, task.priority, prioPadding);
        }
    } else {
        print("No undone tasks\n");
    }
}

void DatabaseCommandVisitor::operator()(AddTask&& cmd)
{
    print("Added \"{}\" to do at {}\n", cmd.task.description, cmd.date.toString());
    _database.add(cmd.task.description, cmd.date, cmd.task.priority);
}

void DatabaseCommandVisitor::operator()(RemoveTask&& cmd)
{
    if (auto task = _database.get(cmd.hash)) {
        print("Removed task \"{}\"\n", task->description);
        _database.remove(cmd.hash);
    }
}

void DatabaseCommandVisitor::operator()(CheckTask&& cmd)
{
    if (auto task = _database.get(cmd.hash)) {
        const auto* checkMessage = task->done() ? "not done" : "done";
        print("Marked task \"{}\" as {}\n", task->description, checkMessage);
        _database.check(cmd.hash);
    }
}

void DatabaseCommandVisitor::operator()(MoveTask&& cmd)
{
    if (auto task = _database.get(cmd.hash)) {
        print("Moved task \"{}\" to {}\n", task->description, cmd.date.toString());
        _database.move(cmd.hash, cmd.date);
    }
}

void DatabaseCommandVisitor::operator()([[maybe_unused]] DoNothing&& cmd) const
{
}

const Database& DatabaseCommandVisitor::database() const noexcept
{
    return _database;
}

} // namespace todo

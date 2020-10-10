#include "DatabaseCommandVisitor.hpp"
#include <algorithm>
#include <fmt/color.h>
#include <fmt/core.h>

namespace {
/**
 * Function object for calculating the maximum length of a string representation of the elements in a container
 * @tparam Container container type
 */
template <typename Container>
class MaxLengthHelper
{
public:
    explicit MaxLengthHelper(const Container& container)
        : _container(container)
    {}
    /**
     * Calculates the maximum string representation length of the elements in the container
     * @param transform function that returns the string representation of an element 
     * @return the maximum length
     */
    template <typename Transform>
    auto operator()(const Transform& transform) const
    {
        auto compare = [&](const auto& a, const auto& b) { return transform(a).length() < transform(b).length(); };
        return transform(*std::max_element(this->_container.begin(), this->_container.end(), compare)).length();
    }

private:
    const Container& _container;
};
} // namespace

using namespace fmt;

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
    const auto& tasks = this->_database.at(cmd.date);
    if (!tasks.empty()) {
        MaxLengthHelper maxLength(tasks);
        constexpr std::array<std::string_view, 4> header = {"Hash", "Task", "Priority", "Done"};
        auto hashPadding = std::max(maxLength([](const auto& e) { return format("{}", e.hash); }), header[0].length());
        auto taskPadding = std::max(maxLength([](const auto& e) { return format("\"{}\"", e.description); }), header[1].length());
        auto prioPadding = std::max(maxLength([](const auto& e) { return format("{}", e.priority); }), header[2].length());

        constexpr auto rowFormat = "{:>{}}  {:<{}}  {:<{}}  {}\n";
        print("To do at {}:\n\n", cmd.date.toString());
        print(rowFormat, header[0], hashPadding, header[1], taskPadding, header[2], prioPadding, header[3]);
        for (const auto& task : tasks) {
            auto checked = task.done ? format(fg(color::green), "V") : format(" ");
            print(rowFormat, task.hash, hashPadding, format("\"{}\"", task.description), taskPadding, task.priority, prioPadding, format("[{}]", checked));
        }
    } else {
        print("Nothing to do {}\n", cmd.date == Date::today() ? format("today") : format("at {}", cmd.date.toString()));
    }
}

void DatabaseCommandVisitor::operator()(AddTask&& cmd)
{
    print("Added \"{}\" to do at {}\n", cmd.task.description, cmd.date.toString());
    this->_database.add(std::move(cmd.task), cmd.date);
}

void DatabaseCommandVisitor::operator()(RemoveTask&& cmd)
{
    if (auto task = this->_database.get(cmd.hash)) {
        print("Removed task \"{}\"\n", task->description);
        this->_database.remove(cmd.hash);
    }
}

void DatabaseCommandVisitor::operator()(CheckTask&& cmd)
{
    if (auto task = this->_database.get(cmd.hash)) {
        const auto* checkMessage = task->done ? "not done" : "done";
        print("Marked task \"{}\" as {}\n", task->description, checkMessage);
        this->_database.check(cmd.hash);
    }
}

void DatabaseCommandVisitor::operator()(MoveTask&& cmd)
{
    if (auto task = this->_database.get(cmd.hash)) {
        print("Moved task \"{}\" to {}\n", task->description, cmd.date.toString());
        this->_database.move(cmd.hash, cmd.date);
    }
}

void DatabaseCommandVisitor::operator()([[maybe_unused]] DoNothing&& cmd) const
{
}

const Database& DatabaseCommandVisitor::database() const noexcept
{
    return this->_database;
}

} // namespace todo

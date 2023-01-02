#include "Database.hpp"
#include <algorithm>
#include <cereal/archives/binary.hpp>
#include <cereal/types/optional.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/unordered_map.hpp>
#include <cereal/types/vector.hpp>
#include <fstream>
#include <functional>
#include <ranges>
#include <spdlog/spdlog.h>

namespace todo {

template <class Archive>
void serialize(Archive& archive, Task& task)
{
    archive(task.description, task.dueDate, task.priority, task.hash, task.addedDate, task.doneDate);
}

template <class Archive>
void serialize(Archive& archive, Date& date)
{
    archive(date.year, date.month, date.day);
}

namespace {
auto load(const std::string& file)
{
    std::ifstream istream(file, std::ios::binary);
    std::vector<Task> input;
    if (istream.good()) {
        cereal::BinaryInputArchive iarchive(istream);
        iarchive(input);
    } else {
        spdlog::info("{} could not be read, creating a new database", file);
    }
    return input;
}

} // namespace

Database::Database(std::string_view file)
    : _file(file)
    , _tasks(load(*_file))
{
}

Database::~Database()
{
    try {
        if (_file) {
            std::ofstream ostream(*_file, std::ios::binary);
            if (ostream.good()) {
                cereal::BinaryOutputArchive oarchive(ostream);
                oarchive(_tasks);
            } else {
                spdlog::error("{} could not be written to", *_file);
            }
        }
    } catch (const std::exception& e) {
        spdlog::error("{} could not be written to", *_file);
    }
}

void Database::add(const std::string& description, const Date& dueDate, Priority priority)
{
    const Task task = {
            .description = description,
            .dueDate = dueDate,
            .priority = priority,
            .hash = nextHash()};
    _tasks.emplace_back(task);
}

void Database::remove(uint32_t hash)
{
    std::erase_if(_tasks, [hash](const auto& t) { return t.hash == hash; });
}

void Database::check(uint32_t hash)
{
    if (auto task = std::ranges::find_if(_tasks, [hash](const auto& t) { return t.hash == hash; }); task != _tasks.end()) {
        task->doneDate = task->done() ? std::nullopt : std::optional<Date>(Date::today());
    }
}

void Database::move(uint32_t hash, const Date& date)
{
    if (auto task = std::ranges::find_if(_tasks, [hash](const auto& t) { return t.hash == hash; }); task != _tasks.end()) {
        task->dueDate = date;
    }
}

std::optional<Task> Database::get(uint32_t hash) const
{
    if (auto task = std::ranges::find_if(_tasks, [hash](const auto& t) { return t.hash == hash; }); task != _tasks.end())
        return *task;
    return std::nullopt;
}

std::vector<Task> Database::withDueDate(const Date& date) const
{
    const auto predicate = [&date](const auto& t) { return t.dueDate == date; };
    return query(predicate);
}

std::vector<Task> Database::withDoneDate(const Date& date) const
{
    const auto predicate = [&date](const auto& t) { return t.doneDate == date; };
    return query(predicate);
}

std::vector<Task> Database::undone() const
{
    const auto predicate = [](const auto& t) { return !t.done(); };
    return query(predicate);
}

std::vector<Task> Database::undoneUpToDueDate(const Date& date) const
{
    const auto predicate = [&date](const auto& t) { return !t.done() && t.dueDate <= date; };
    return query(predicate);
}

uint32_t Database::nextHash() const
{
    if (_tasks.empty()) {
        return 0;
    }
    return std::ranges::max(_tasks | std::views::transform([](const auto& t) { return t.hash; })) + 1;
}

std::vector<Task> Database::query(const std::function<bool(const Task&)>& predicate) const
{
    std::vector<Task> tasks;
    for (const auto& task : _tasks | std::views::filter(predicate)) {
        tasks.push_back(task);
    }
    std::ranges::sort(tasks, [](const auto& a, const auto& b) { return a.priority > b.priority; });
    return tasks;
}

} // namespace todo

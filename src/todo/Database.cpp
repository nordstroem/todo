#include "Database.hpp"
#include <algorithm>
#include <cereal/archives/binary.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/unordered_map.hpp>
#include <cereal/types/vector.hpp>
#include <fstream>
#include <functional>
#include <spdlog/spdlog.h>

namespace todo {

template <class Archive>
void serialize(Archive& archive, HashedTask& hashedTask)
{
    archive(hashedTask.description, hashedTask.priority, hashedTask.hash, hashedTask.done);
}

template <class Archive>
void serialize(Archive& archive, Date& date)
{
    archive(date.year, date.month, date.day);
}

static auto load(const std::string& file)
{
    std::ifstream istream(file, std::ios::binary);
    std::unordered_map<Date, std::vector<HashedTask>> input;
    if (istream.good()) {
        cereal::BinaryInputArchive iarchive(istream);
        iarchive(input);
    } else {
        spdlog::info("{} could not be read, creating a new database", file);
    }
    return input;
}

Database::Database(std::string_view file)
    : _file(file)
    , _tasks(load(*this->_file))
{
}

Database::~Database()
{
    try {
        if (this->_file) {
            std::ofstream ostream(*this->_file, std::ios::binary);
            if (ostream.good()) {
                cereal::BinaryOutputArchive oarchive(ostream);
                oarchive(this->_tasks);
            } else {
                spdlog::error("{} could not be written to", *this->_file);
            }
        }
    } catch (const std::exception& e) {
        spdlog::error("{} could not be written to", *this->_file);
    }
}

void Database::add(Task&& task, const Date& date)
{
    uint32_t largestHash = 0;
    for (const auto& [date, tasks] : this->_tasks)
        for (const auto& task : tasks)
            largestHash = std::max(largestHash, task.hash);

    this->_tasks[date].emplace_back(std::move(task), largestHash + 1);
}

void Database::remove(uint32_t hash)
{
    for (auto& [date, tasks] : this->_tasks)
        std::erase_if(tasks, [hash](const auto& t) { return t.hash == hash; });
}

void Database::check(uint32_t hash)
{
    for (auto& [date, tasks] : this->_tasks)
        if (auto task = std::ranges::find_if(tasks, [hash](const auto& t) { return t.hash == hash; }); task != tasks.end())
            task->done = !task->done;
}

void Database::move(uint32_t hash, const Date& date)
{
    for (auto& [currentDate, tasks] : this->_tasks)
        if (auto task = std::ranges::find_if(tasks, [hash](const auto& t) { return t.hash == hash; }); task != tasks.end()) {
            this->_tasks[date].push_back(*task);
            tasks.erase(task);
            break;
        }
}

std::optional<HashedTask> Database::get(uint32_t hash) const
{
    for (const auto& [date, tasks] : this->_tasks)
        if (auto task = std::ranges::find_if(tasks, [hash](const auto& t) { return t.hash == hash; }); task != tasks.end())
            return *task;
    return std::nullopt;
}

std::vector<HashedTask> Database::at(const Date& date) const
{
    if (auto tasksAtDate = this->_tasks.find(date); tasksAtDate != this->_tasks.end()) {
        auto tasks = tasksAtDate->second;
        std::sort(tasks.begin(), tasks.end(), std::greater<>());
        return tasks;
    }
    return {};
}

} // namespace todo

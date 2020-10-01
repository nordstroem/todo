#include "Database.hpp"
#include <cereal/archives/binary.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/unordered_map.hpp>
#include <cereal/types/vector.hpp>
#include <fstream>
#include <spdlog/spdlog.h>

namespace todo {

template <class Archive>
void serialize(Archive& archive, Task& task)
{
    archive(task.task, task.priority);
}

static auto load(const std::string& file)
{
    std::ifstream istream(file, std::ios::binary);
    std::unordered_map<Date, std::vector<Task>> input;
    if (istream.good()) {
        cereal::BinaryInputArchive iarchive(istream);
        iarchive(input);
    } else {
        spdlog::info("{} could not be read, creating a new database", file);
    }
    return input;
}

Database::Database(const std::string& file)
    : _tasks(load(file))
    , _file(file)
{
}

Database::~Database()
{
    if (this->_file) {
        std::ofstream ostream(*this->_file, std::ios::binary);
        if (ostream.good()) {
            cereal::BinaryOutputArchive oarchive(ostream);
            oarchive(this->_tasks);
        } else {
            spdlog::error("{} could not be written to", *this->_file);
        }
    }
}

void Database::add(Task&& task, const Date& date)
{
    this->_tasks[date].push_back(std::move(task));
}

std::vector<Task> Database::at(const Date& date) const
{
    if (auto task = this->_tasks.find(date); task != this->_tasks.end())
        return task->second;
    return {};
}

} // namespace todo

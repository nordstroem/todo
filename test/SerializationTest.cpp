#include <cereal/archives/binary.hpp>
#include <cereal/types/unordered_map.hpp>
#include <doctest.h>
#include <fmt/core.h>
#include <fstream>
#include <functional>
#include <string>

struct Record
{
    int a = 0;

    template <class Archive>
    void serialize(Archive& archive)
    {
        archive(a);
    }
};

TEST_CASE("hashing")
{
    int i = 1337;
    auto hashed = std::hash<std::string>{}(std::to_string(i));
    REQUIRE(i != hashed);
}

TEST_CASE("serialization with cereal")
{
    {
        std::unordered_map<int, Record> data;
        data[1] = Record{.a = 16};
        std::ofstream s("test.bin", std::ios::binary);
        cereal::BinaryOutputArchive oarchive(s);
        oarchive(data);
    }

    {
        std::ifstream s("test.bin", std::ios::binary);
        std::unordered_map<int, Record> data;
        cereal::BinaryInputArchive iarchive(s);
        iarchive(data);
        REQUIRE(data.find(1) != data.end());
    }
}

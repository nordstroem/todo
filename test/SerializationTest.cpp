#include <cereal/archives/binary.hpp>
#include <cereal/types/unordered_map.hpp>
#include <doctest.h>
#include <fmt/core.h>
#include <fstream>

struct Record
{
    int a = 0;

    template <class Archive>
    void serialize(Archive& archive)
    {
        archive(a);
    }
};

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
        fmt::print("{}\n", data[1].a);
    }
}

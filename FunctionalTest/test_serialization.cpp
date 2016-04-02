#include "catch.hpp"

#include <fstream>
#include <string>
#include <stdint.h>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

class Bar
{
public:
    Bar() :mId(0), mStr(L"default"){}

    void setId(uint32_t i){ mId = i; }

    void setName(const std::wstring& name){ mStr = name; }

    uint32_t getId() const { return mId; }

    const std::wstring& getName() const { return mStr; }

    friend class boost::serialization::access;
    template <class Archive>
    void serialize(Archive& ar, const uint32_t vertion)
    {
        ar & mId;
        ar & mStr;
    }

private:
    uint32_t mId;
    std::wstring mStr;
};


TEST_CASE("boost serialization", "[Core]") {
    const char* data_path = "../FunctionalTest/Data/serialization.txt";

    std::wstring name1 = L"Adam";
    std::wstring name2 = L"Jammie";

    Bar bar1;
    bar1.setId(1);
    bar1.setName(name1);

    Bar bar2;
    bar2.setId(2);
    bar2.setName(name2);

    SECTION("serialized to text stream") {
        std::ofstream ofs(data_path);
        boost::archive::text_oarchive oa(ofs);
        oa << bar1 << bar2;
        ofs.close();
    }

    SECTION("deserialize from text stream"){
        Bar bar3, bar4;
        std::ifstream ifs(data_path);
        boost::archive::text_iarchive ia(ifs);
        ia >> bar3 >> bar4;
        ifs.close();

        REQUIRE(bar3.getId() == 1);
        REQUIRE(bar3.getName().compare(name1) == 0);

        REQUIRE(bar4.getId() == 2);
        REQUIRE(bar4.getName().compare(name2) == 0);
    }

    // TODO binary serialization
}

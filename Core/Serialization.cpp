#include "Serialization.h"

#include <fstream>
#include <string>
#include <stdint.h>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>


class Bar
{
public:
    Bar() :mId(0), mStr(L"default"){}
    void setID(uint32_t i){ mId = i; }
    void setString(const std::wstring& s){ mStr = s; }

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

private:
};


void test_Serialization()
{
    Bar bar1;
    bar1.setID(1);
    bar1.setString(L"I am 1");

    Bar bar2;
    bar2.setID(2);
    bar2.setString(L"I am 2");

        std::ofstream ofs("data");
        boost::archive::text_oarchive oa(ofs);
        oa << bar1 << bar2;
        ofs.close();

    Bar bar3, bar4;
        std::ifstream ifs("data");
        boost::archive::text_iarchive ia(ifs);
        ia >> bar3 >> bar4;
        ifs.close(); 
}

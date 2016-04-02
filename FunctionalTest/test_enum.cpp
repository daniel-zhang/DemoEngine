#include "catch.hpp"
#include "Core/Util.h"

/*
*/

struct ECBufferVar
{
    enum 
    {
        MiscParam = 0,
        WorldMatrix,
        ViewMatrix,

        TotalNum,
    };

    static const char* str[];
};
const char* ECBufferVar::str[] = { "gMiscParam", "gWorld", "gView" };

struct ETextureVar
{
    enum 
    {
        DiffuseTexture = 0,
        FontTexture,
        GuiTexture,

        TotalNum,
    };

    static const char* str[];
};
const char* ETextureVar::str[] = { "gDiffuse", "gFont", "gTex" };

enum EIdentity
{
    None             = 0,
    Curious          = 1,
    GuitarPlayer     = 1 << 1,
    ScifiFan          = 1 << 2,
    Available        = 1 << 3,
    NotVeryTalktive  = 1 << 4,
    Programmer       = 1 << 5,
    BizzarTaste      = 1 << 6
};

TEST_CASE("Custom enumeration ", "[language]") 
{
    const char* s = ECBufferVar::str[ECBufferVar::MiscParam];
    const char* ss = ETextureVar::str[ETextureVar::DiffuseTexture];

    uint32_t id = 0;
    id |= EIdentity::Available;
    id |= EIdentity::ScifiFan;
    id |= EIdentity::Programmer;

    REQUIRE( (id & EIdentity::Curious) == 0 );
    REQUIRE( (id & EIdentity::ScifiFan) != 0 );
    REQUIRE( (id & EIdentity::BizzarTaste) == 0 );

}

#include "catch.hpp"

#include "Core/Util.h"
#include "Renderer/Image.h"

bool img_convert(const std::wstring& src, Image::EImageFileFormat dstFormat)
{
    Image img;
    if ( !img.createFromFile(src) ) 
        return false;

    if (!img.saveToFile(src, dstFormat))
        return false;

    return true;
}

TEST_CASE("Image", "[renderer][!mayfail]")
{
    std::wstring dir;
    dir = FileOp::getCurrPath().toString();
    dir += L"../FunctionalTest/Image/";

    std::wstring png = dir + L"Adium.png";
    std::wstring gif = dir + L"kitty.gif";

    REQUIRE(FileOp::checkFileExists(png) == true);
    REQUIRE(FileOp::checkFileExists(gif) == true);

    SECTION("PNG to other formats") {
        SECTION("JPEG"){
            REQUIRE(img_convert(png, Image::JPEG));
        }

        SECTION("BMP"){
            REQUIRE(img_convert(png, Image::BMP));
        }

        SECTION("PNG"){
            REQUIRE(img_convert(png, Image::PNG));
        }

        SECTION("TIFF"){
            REQUIRE(img_convert(png, Image::TIFF));
        }

        SECTION("GIF"){
            REQUIRE(img_convert(png, Image::GIF));
        }

        SECTION("WMP"){
            REQUIRE(img_convert(png, Image::WMP));
        }

        SECTION("ICO"){
            REQUIRE(img_convert(png, Image::ICO));
        }

        SECTION("TGA"){
            REQUIRE(img_convert(png, Image::TGA));
        }

        SECTION("DDS"){
            REQUIRE(img_convert(png, Image::DDS));

            SECTION("dds back to png to see if alpha channel is kept") {
                REQUIRE(img_convert(png + L".dds", Image::PNG));
            }
        }
    }

    SECTION("GIF to other formats") {
        SECTION("JPEG"){
            REQUIRE(img_convert(gif, Image::JPEG));
        }

        SECTION("BMP"){
            REQUIRE(img_convert(gif, Image::BMP));
        }

        SECTION("PNG"){
            REQUIRE(img_convert(gif, Image::PNG));
        }

        SECTION("TIFF"){
            REQUIRE(img_convert(gif, Image::TIFF));
        }

        SECTION("GIF"){
            REQUIRE(img_convert(gif, Image::GIF));
        }

        SECTION("WMP"){
            REQUIRE(img_convert(gif, Image::WMP));
        }

        SECTION("ICO"){
            REQUIRE(img_convert(gif, Image::ICO));
        }

        SECTION("TGA"){
            REQUIRE(img_convert(gif, Image::TGA));
        }

        SECTION("DDS"){
            REQUIRE(img_convert(gif, Image::DDS));
        }
    }
}
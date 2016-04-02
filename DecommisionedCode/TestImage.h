#pragma once
#include "Renderer/Image.h"
#include "Core/Util.h"

class TestImage
{
public:
    // Conversion
    static void convert(const std::wstring& src, Image::EImageFileFormat dstFormat )
    {
        std::wstring dir;
        getDir(dir);

        Image img;

        img.createFromFile(dir + src);

        img.saveToFile(dir + src, dstFormat);
    }

    static void getDir(std::wstring& dir)
    {
        dir = FileOp::getCurrPath().toString();
        dir += L"/TestImage/";
    }
};
#pragma once

#include "Core/Util.h"
#include <dxgiformat.h>

class Image2D 
{
public:
    Image2D();

public:
    void initialize(uint32_t width, uint32_t height, DXGI_FORMAT format);

    void loadFromFile(const std::string& inFilename);

    void saveToFile(const std::string& inFilename, DXGI_FORMAT inPixelFormat = DXGI_FORMAT_UNKNOWN) const ; 

    inline DXGI_FORMAT getFormat()const { return mPixelFormat; }

    inline uint32_t getWidth()const { return mWidth; }

    inline uint32_t getHeight() const { return mHeight; }

    inline uint32_t getRowPitch()const { return mRowPitch; }

    inline uint32_t getSlicePitch()const { return mSlicePitch; }

    inline std::vector<uint8_t>& getRawData(){ return mRawData; }

protected:
    DXGI_FORMAT mPixelFormat;
    uint32_t mWidth;
    uint32_t mHeight;

    uint32_t mRowPitch;
    uint32_t mSlicePitch;

    std::vector<uint8_t> mRawData;
};



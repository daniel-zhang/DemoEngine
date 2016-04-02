#include "Image2D.h"
#include <DirectXTex.h>

Image2D::Image2D() : mPixelFormat(DXGI_FORMAT_UNKNOWN), mWidth(0), mHeight(0), mRowPitch(0), mSlicePitch(0)
{

}

void Image2D::initialize(uint32_t width, uint32_t height, DXGI_FORMAT format)
{
    mPixelFormat = format;
    mWidth       = width;
    mHeight      = height;

    DirectX::ComputePitch(format, width, height, mRowPitch, mSlicePitch);

    mRawData.resize(mSlicePitch, 0);
}

void Image2D::loadFromFile(const std::string& inFilename)
{

}

void Image2D::saveToFile(const std::string& inFilename, DXGI_FORMAT inFormat) const
{
    // Copy the raw data to make this method const
    std::vector<uint8_t> temp;
    temp.reserve(mRawData.size());
    temp.insert(temp.end(), mRawData.begin(), mRawData.end());

    DirectX::Image img;
    img.format     = (inFormat == DXGI_FORMAT_UNKNOWN ? mPixelFormat : inFormat);
    img.width      = mWidth;
    img.height     = mHeight;
    img.rowPitch   = mRowPitch;
    img.slicePitch = mSlicePitch;
    img.pixels     = &temp[0];

    // TODO translate file ext to codec
    REFGUID codec = DirectX::GetWICCodec(DirectX::WIC_CODEC_PNG);
    DirectX::SaveToWICFile(img, DirectX::WIC_FLAGS_NONE, codec, s2ws(inFilename).c_str());
}


#pragma once

#include "Core/Util.h"
#include <dxgiformat.h>


/*
The Image class implements the CPU side of texture pipeline:
Path 1
    - read from image files of multiple formats
    - represent them in bitmap layout, which are consumed by Texture from the GPU side
    - write back to file with bitmap data provided by Texture
Path 2
    - created as an empty image
    - functions as the CPU memory cache for dynamic textures

It supports either a single bitmap or an array of bitmaps with the same size and format. Though these two cases are not explicitly 
distinguished, as the former is a special case of the latter when array size is 1.
*/
class Image
{
public:
    enum EDimension
    {
        Image_1D,
        Image_2D,
        Image_3D,
    };

    enum EImageFileFormat
    {
        BMP,              // Windows Bitmap (.bmp)
        JPEG,             // Joint Photographic Experts Group (.jpg, .jpeg)
        PNG,              // Portable Network Graphics (.png)
        TIFF,             // Tagged Image File Format  (.tif, .tiff)
        GIF,              // Graphics Interchange Format  (.gif)
        WMP,              // Windows Media Photo / HD Photo / JPEG XR (.hdp, .jxr, .wdp)
        ICO,              // Windows Icon (.ico)
        DDS,
        TGA,
    };

public:
    Image() :
        mIsInitialized(false), mDimension(EDimension::Image_2D), mDxgiFormat(DXGI_FORMAT_UNKNOWN), 
        mWidth(0), mHeight(0), mDepth(0), 
        mRowPitch(0), mSlicePitch(0)
    {}

    ~Image(){}

    bool isInitialized() const { return mIsInitialized; }

    // Typically used as CPU side cache for dynamic textures
    void createAsEmptyImage(DXGI_FORMAT format, uint32_t width, uint32_t height, uint32_t depth = 1, uint32_t arraySize = 1 );

    // Typically used as data source for immutable textures
    bool createFromFile(const std::wstring& filePathNameExt);

    bool saveToFile(const std::wstring& filePathName, EImageFileFormat targetFileFormat, DXGI_FORMAT targetPixelFormat = DXGI_FORMAT_UNKNOWN) ;

    void destroy();

    // @parameter index
    // for 1D/2D images, it is the index of the array
    // for 3D image, it is the depth of the volume
    std::vector<uint8_t>& getSurface(uint32_t index = 0) { return mMemory[index]; }

    const std::vector<uint8_t>& getSurface(uint32_t index = 0) const { return mMemory[index]; }

    EDimension getDimension() const { return mDimension; }

    DXGI_FORMAT getDxgiFormat()const { return mDxgiFormat; }

    uint32_t getWidth()const { return mWidth; }

    uint32_t getHeight() const { return mHeight; }

    uint32_t getDepth() const { return mDepth; }

    uint32_t getArraySize() const { return mMemory.size(); }

    // Byte size of a row in a bitmap
    uint32_t getRowPitch()const { return mRowPitch; }

    // Byte size of a bitmap
    uint32_t getSlicePitch()const { return mSlicePitch; }

protected:
    void determinImageDimension()
    {
        if ( mDepth != 1) mDimension = EDimension::Image_3D;
        else if (mHeight != 1) mDimension = EDimension::Image_2D;
        else mDimension = EDimension::Image_1D;
    }
protected:
    bool mIsInitialized;

    EDimension mDimension;

    DXGI_FORMAT mDxgiFormat;
    uint32_t mWidth;
    uint32_t mHeight;
    uint32_t mDepth;

    uint32_t mRowPitch;
    uint32_t mSlicePitch;

    // Non-array image is special case of image array when array size is 1.
    // For 1D-Array/2D-Array, this std::vector is indexed by 'slice'
    // For 3D image, this std::vector is indexed by 'depth'
    // No need to support 3D-Arrays, as they are not supported by graphics APIs 
    typedef std::vector<uint8_t> TSurfaceMem;
    std::vector<TSurfaceMem> mMemory;
};
#include "Image.h"
#include <DirectXTex.h>

const char* gImageFileExtensionString[] = {".bmp", ".jpg", ".png", ".tiff", ".giff", ".hdp", ".ico", ".dds", ".tga"};

void Image::createAsEmptyImage(DXGI_FORMAT format, uint32_t width, uint32_t height, uint32_t depth /*= 1*/, uint32_t arraySize /*= 1 */)
{
    mDxgiFormat = format;
    mWidth      = width;
    mHeight     = height;
    mDepth      = depth;

    determinImageDimension();

    // Given a bitmap's size and pixel format, figure out the byte sizes.
    DirectX::ComputePitch(mDxgiFormat, mWidth, mHeight, mRowPitch, mSlicePitch);

    // Preallocate memory, filled with 0
    mMemory.resize(arraySize);
    for (uint32_t i = 0; i < mMemory.size(); ++i)
    {
        mMemory[i].resize(mSlicePitch, 0);
    }

    mIsInitialized = true;
}


bool Image::createFromFile(const std::wstring& filePathNameExt)
{
    //
    // Try read file 
    //
    DirectX::ScratchImage scratchImage;
    DirectX::TexMetadata metaData;

    EImageFileFormat srcFileFormat;
    if (!!SUCCEEDED(DirectX::LoadFromDDSFile(filePathNameExt.c_str(), DirectX::DDS_FLAGS_NONE, &metaData, scratchImage)))
    {
        srcFileFormat = EImageFileFormat::DDS;
    }
    else if (!!SUCCEEDED(DirectX::LoadFromTGAFile(filePathNameExt.c_str(), &metaData, scratchImage)))
    {
        srcFileFormat = EImageFileFormat::TGA;
    }
    else if (!!SUCCEEDED(DirectX::LoadFromWICFile(filePathNameExt.c_str(), DirectX::WIC_FLAGS_NONE, &metaData, scratchImage)))
    {
        // TODO, extract source format from file name
    }
    else
    {
        return false;
    }

    //
    // Extract data from DirectX data structures
    //
    mMemory.clear();
    mDxgiFormat = metaData.format;
    mWidth      = metaData.width;
    mHeight     = metaData.height;
    mDepth      = metaData.depth;

    // Given a bitmap's size and pixel format, figure out the byte sizes.
    DirectX::ComputePitch(mDxgiFormat, mWidth, mHeight, mRowPitch, mSlicePitch);

    determinImageDimension();

    // We do not support mipmap for now, so we are using mip=0 to interact with DirectXTex
    switch (metaData.dimension)
    {

    case DirectX::TEX_DIMENSION_TEXTURE1D:
    case DirectX::TEX_DIMENSION_TEXTURE2D:
    {
        /*
        if (metaData.dimension == DirectX::TEX_DIMENSION_TEXTURE1D)
            check(mDimension == Image_1D);
        else
            check(mDimension == Image_2D);
        */

        mMemory.resize(metaData.arraySize);

        for (uint32_t arrayIndex = 0; arrayIndex < metaData.arraySize; ++arrayIndex)
        {
            TSurfaceMem& bitmapMem = mMemory[arrayIndex];

            // const Image* __cdecl GetImage(_In_ size_t mip, _In_ size_t item, _In_ size_t slice) const;
            // @mip: mip level index
            // @item: image array index, must be 0 for volume images as they don't support image array
            // @slice: 0 based image index for the current mip level of a volume image, not used in 1D/2D images 
            // if a volume image has 3 mip levels and 8 2D-images at top level, then the slice range for each mip level is:
            // mip = 0, slice = [0, 7]
            // mip = 1, slice = [0, 3]
            // mip = 2, slice = [0, 1]
            const DirectX::Image* dxImg = scratchImage.GetImage(0, arrayIndex, 0);

            // As only the top level mipmaps are extracted, we expect to
            //  - to have the same size
            //  - to be non-empty
            check(dxImg->rowPitch == mRowPitch);
            check(dxImg->slicePitch == mSlicePitch);
            check(dxImg->slicePitch > 0);

            // Copy image data from DirectXTex to our Image object
            bitmapMem.resize(dxImg->slicePitch, 0);
            memcpy(&bitmapMem[0], dxImg->pixels, dxImg->slicePitch);
        }
    }
        break;

    case DirectX::TEX_DIMENSION_TEXTURE3D:
    {
        check(mDimension == Image_3D);

        mMemory.resize(mDepth);
        for (uint32_t depthSlice = 0; depthSlice < mDepth; ++depthSlice)
        {
            TSurfaceMem& bitmapMem = mMemory[depthSlice];

            const DirectX::Image* dxImg = scratchImage.GetImage(0, 0, depthSlice);

            check(dxImg->rowPitch == mRowPitch);
            check(dxImg->slicePitch == mSlicePitch);
            check(dxImg->slicePitch > 0);

            // Copy image data from DirectXTex to our Image object
            bitmapMem.resize(dxImg->slicePitch, 0);
            memcpy(&bitmapMem[0], dxImg->pixels, dxImg->slicePitch);
        }
    }
        break;
    default:
        break;
    }

    mIsInitialized = true;

    return true;
}


bool Image::saveToFile(const std::wstring& filePathName, EImageFileFormat targetFileFormat, DXGI_FORMAT targetPixelFormat)
{
    //
    // Quit early if nothing to do
    //
    if (!mIsInitialized || mMemory.empty())
        return false;

    //
    // Create an array of DirectX::Image to describe our image data
    //
    std::vector<DirectX::Image> dxImages;
    dxImages.resize(mMemory.size());
    DXGI_FORMAT dstPixelFormat = (targetPixelFormat == DXGI_FORMAT_UNKNOWN ? mDxgiFormat : targetPixelFormat);

    for (uint32_t i = 0; i < mMemory.size(); ++i)
    {
        DirectX::Image& dxImg = dxImages[i];

        dxImg.format     = dstPixelFormat;
        dxImg.width      = mWidth;
        dxImg.height     = mHeight;
        dxImg.rowPitch   = mRowPitch;
        dxImg.slicePitch = mSlicePitch;
        dxImg.pixels     = &mMemory[i][0];
    }

    //
    // Save the image based on its array size and target format
    //
    std::wstring fileExt = s2ws(gImageFileExtensionString[(uint32_t)targetFileFormat]);
    std::wstring fileFullname;

    // As DDS and TGA do not support saving multiple frames in one file, those cases have to be distinguished here.
    if (targetFileFormat == EImageFileFormat::TGA || targetFileFormat == EImageFileFormat::DDS)
    {
        // Image contains a single frame
        if (dxImages.size() == 1)
        {
            fileFullname = filePathName + fileExt;
            if (targetFileFormat == EImageFileFormat::TGA)
            {
                HRESULT hr = DirectX::SaveToTGAFile(dxImages[0], fileFullname.c_str());
                if (FAILED(hr)) 
                    return false;
            }
            else 
            {
                HRESULT hr = DirectX::SaveToDDSFile(dxImages[0], DirectX::DDS_FLAGS_NONE, fileFullname.c_str());
                if (FAILED(hr)) 
                    return false;
            }
        }
        // Image contains multiple frames, indices are added to file names as in: $(path)/$(name)_$(index)$(ext)
        else
        {
            for (uint32_t i = 0; i < dxImages.size(); ++ i)
            {
                fileFullname = wstring_format(L"%s_%i%s", filePathName.c_str(), i, fileExt.c_str());
                if (targetFileFormat == EImageFileFormat::TGA)
                {
                    HRESULT hr = DirectX::SaveToTGAFile(dxImages[i], fileFullname.c_str());
                    if (FAILED(hr))
                        return false;
                }
                else
                {
                    HRESULT hr = DirectX::SaveToDDSFile(dxImages[i], DirectX::DDS_FLAGS_NONE, fileFullname.c_str());
                    if (FAILED(hr))
                        return false;
                }
            }
        }
    }
    else
    {
        // Determine WIC codec based on target image file format
        DirectX::WICCodecs wicCodecs;
        switch (targetFileFormat)
        {
        case Image::BMP:
            wicCodecs = DirectX::WIC_CODEC_BMP;
            break;
        case Image::JPEG:
            wicCodecs = DirectX::WIC_CODEC_JPEG;
            break;
        case Image::PNG:
            wicCodecs = DirectX::WIC_CODEC_PNG;
            break;
        case Image::TIFF:
            wicCodecs = DirectX::WIC_CODEC_TIFF;
            break;
        case Image::GIF:
            wicCodecs = DirectX::WIC_CODEC_GIF;
            break;
        case Image::WMP:
            wicCodecs = DirectX::WIC_CODEC_WMP;
            break;
        case Image::ICO:
            wicCodecs = DirectX::WIC_CODEC_ICO;
            break;
        case Image::DDS:
        case Image::TGA:
        default:
            break;
        }
        REFGUID codec = DirectX::GetWICCodec(wicCodecs);

        fileFullname = filePathName + fileExt;
        HRESULT hr = DirectX::SaveToWICFile(&dxImages[0], dxImages.size(), DirectX::WIC_FLAGS_NONE, codec, fileFullname.c_str());
        if (FAILED(hr))
            return false;
    }
    return true;
}

void Image::destroy()
{
    mIsInitialized = false;
    mDimension     = EDimension::Image_2D;
    mDxgiFormat    = DXGI_FORMAT_UNKNOWN;
    mWidth         = 0;
    mHeight        = 0;
    mDepth         = 0;
    mRowPitch      = 0;
    mSlicePitch    = 0;

    mMemory.clear();
}

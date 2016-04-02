#pragma once

#include "RendererCommon.h"
#include "Image.h"

/*
Memo
A D3D11 texture is defined by a bunch of parameters:
    mip-map levels
    array size
    multi-sample settings
    usage
    bind type
    CPU access flag
    ...

However, despite that these parameters do look descriptive, only a few combinations of them make sense when it comes down to practical 
use.

The Texture class here is to define a texture by its real life usages and to hide the messy parameter settings.

TODO
    - fully support mip map levels 
*/
class Texture
{
public:
    enum EUsage
    {
        DynamicShaderResource = 0,
        ImmutableShaderResource,
        RenderTargetAndShaderResource,
        DepthStencilAndShaderResource,
    };

    enum EDimension
    {
        Texture_1D,
        Texture_2D,
        Texture_3D,
    };

public:
    Texture();

    virtual ~Texture();

    // Set msCount = 1, msQuality = 0 to disable multi-sampling
    // Set mip = 0 to generate a full set of mip maps, with multi-sampling disabled.
    // Set width > 1, height = 1, depth = 1 for 1D textures
    // Set width > 1, height > 1, depth = 1 for 2D textures
    // Set width > 1, height > 1, depth > 1 for 3D textures

    // d3d11 doesn't support dynamic textures with mip maps or multiple array slices
    bool initializeAsDynamic( DXGI_FORMAT format, 
        uint32_t width, uint32_t height, uint32_t depth = 1, 
        uint32_t msCount = 1, uint32_t msQuality = 0);

    // TODO: support gamma correction by using _SRGB for RTV/SRV format
    // More on gamma correction and color space: https://msdn.microsoft.com/en-us/library/windows/desktop/hh972627(v=vs.85).aspx
    // d3d11 doesn't support multi-sampled textures with mip maps 
    bool initializeAsRenderTarget( DXGI_FORMAT format, 
        uint32_t width, uint32_t height,
        uint32_t arraySize = 1, uint32_t mip = 1,
        uint32_t msCount = 1, uint32_t msQuality = 0 );

    // Typical formats
    // resFormat = R32_Typeless
    // dsvFormat = D32_Float
    // srvFormat = R32_Float
    // d3d11 doesn't support multi-sampled textures with mip maps 
    bool initializeAsDepthStencil( DXGI_FORMAT resFormat, DXGI_FORMAT dsvFormat, DXGI_FORMAT srvFormat,
        uint32_t width, uint32_t height, 
        uint32_t arraySize = 1, uint32_t mip = 1,
        uint32_t msCount = 1, uint32_t msQuality = 0);

    bool initializeAsImmutableFromImage(const Image& srcImage, uint32_t msCount = 1, uint32_t msQuality = 0);

    // TODO: initialize as cube/cube_array textures

    // TODO: initialize as cube/cube_array textures from images

    void destroy();

    // Usages: dynamic
    Image& updateImageCache();

    // Usages: dynamic
    void commitImageCache_Tex1D();

    // Usages: dynamic
    void commitImageCache_Tex2D();

    // Usages: dynamic, TODO: test this
    void commitImageCache_Tex3D();

    void bindAsShaderResource(EShaderStage ss, uint32_t slot);

    // Copy this texture to a staging texture which is then read back from the CPU and written to a disk file.
    // The staging texture is released before return. Potential performance hit.
    void debugSave(const std::wstring& filepathname, Image::EImageFileFormat fileFormat);

public:
    bool isInitialized() const { return mIsInitalized; }

    bool isArray() const { return mArraySize > 1; }

    bool isMipmapEnabled() const { return mMipLevels != 1; }

    bool isMultisamplingEnabled() const { return mMultiSampleCount > 1 ; }

    DXGI_FORMAT getResourceFormat() const { return mFormat; }

    EUsage getUsage() const { return mUsage; }

    EDimension getDimension() const { return mDimension; }

    uint32_t getWidth() const { return mWidth; }

    uint32_t getHeight() const { return mHeight; }

    uint32_t getDepth() const { return mDepth; }

    uint32_t getMipLevels() const { return mMipLevels; }

    uint32_t getMultiSampleCount() const { return mMultiSampleCount; }

    uint32_t getMultiSampleQuality() const { return mMultiSampleQuality; }

    ID3D11DepthStencilView* getDSV() const { return mDSV; }

    ID3D11ShaderResourceView* getSRV() const { return mSRV; }

    ID3D11RenderTargetView* getRTV() const { return mRTV; }

    ID3D11UnorderedAccessView* getUAV() const { return mUAV; }

    uint32_t getMemoryUsage() const;

protected:
    void determineTextureDimension();

    void intializeImageCache();

    void validateSRV_Texture1D();

    void validateSRV_Texture2D();

    void validateSRV_Texture3D();

    // TODO
    void validateSRV_TextureCube();

protected:
    bool mIsInitalized;

    ID3D11Device* mDev;
    ID3D11DeviceContext* mCtx;

    DXGI_FORMAT mFormat;
    EUsage mUsage;

    EDimension mDimension;
    uint32_t mWidth;
    uint32_t mHeight;
    uint32_t mDepth;

    uint32_t mArraySize;
    uint32_t mMipLevels;

    uint32_t mMultiSampleCount;
    uint32_t mMultiSampleQuality;

    bool mIsImageCacheDirty;
    Image* mImageCache; // Used by dynamic textures

    ID3D11Texture1D* mTexture1D;
    ID3D11Texture2D* mTexture2D;
    ID3D11Texture3D* mTexture3D;

    ID3D11ShaderResourceView* mSRV;
    ID3D11RenderTargetView* mRTV;
    ID3D11DepthStencilView* mDSV;
    ID3D11UnorderedAccessView* mUAV;
};
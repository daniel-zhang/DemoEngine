#include "Texture.h"
#include "Renderer.h"
#include "DxgiFormatUtil.h"

Texture::Texture() :
    mIsInitalized(false),

    mDev(nullptr), mCtx(nullptr),

    mFormat(DXGI_FORMAT_UNKNOWN), 
    mUsage(EUsage::DynamicShaderResource), 
    mDimension(EDimension::Texture_2D),

    mWidth(0), mHeight(0), mDepth(0), mArraySize(0), 
    mMipLevels(1), mMultiSampleCount(1), mMultiSampleQuality(0),

    mIsImageCacheDirty(false), mImageCache(nullptr),

    mTexture1D(nullptr),
    mTexture2D(nullptr),
    mTexture3D(nullptr),

    mSRV(nullptr),
    mRTV(nullptr),
    mDSV(nullptr),
    mUAV(nullptr)
{

}

Texture::~Texture()
{
    destroy();
}

bool Texture::initializeAsDynamic(DXGI_FORMAT format, uint32_t width, uint32_t height, uint32_t depth /*= 1*/, uint32_t msCount /*= 1*/, uint32_t msQuality /*= 0*/)
{
    if (mIsInitalized) return false;

    mDev = Renderer::getInstance()->getDevice();
    mCtx = Renderer::getInstance()->getContext();

    mUsage              = EUsage::DynamicShaderResource;
    mFormat             = format;
    mWidth              = width;
    mHeight             = height;
    mDepth              = depth;
    mArraySize          = 1;
    mMipLevels          = 1;
    mMultiSampleCount   = msCount;
    mMultiSampleQuality = msQuality;

    determineTextureDimension();

    switch (mDimension)
    {
    case Texture::Texture_1D:
    {
        D3D11_TEXTURE1D_DESC texDesc;

        texDesc.Width          = mWidth;
        texDesc.Format         = mFormat;
        texDesc.MipLevels      = mMipLevels;
        texDesc.ArraySize      = mArraySize;
        texDesc.MiscFlags      = 0;
        texDesc.Usage          = D3D11_USAGE_DYNAMIC;
        texDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; // Only make sense for dynamic and staging textures
        texDesc.BindFlags      = D3D11_BIND_SHADER_RESOURCE;

        check_hr(mDev->CreateTexture1D(&texDesc, 0, &mTexture1D));
        check_hr(mDev->CreateShaderResourceView(mTexture1D, 0, &mSRV));

        validateSRV_Texture1D();
    }
        break;

    case Texture::Texture_2D:
    {
        D3D11_TEXTURE2D_DESC texDesc;

        texDesc.Width              = mWidth;
        texDesc.Height             = mHeight;
        texDesc.Format             = mFormat;
        texDesc.MipLevels          = mMipLevels;
        texDesc.ArraySize          = mArraySize;
        texDesc.SampleDesc.Count   = mMultiSampleCount;
        texDesc.SampleDesc.Quality = mMultiSampleQuality;
        texDesc.MiscFlags          = 0;
        texDesc.Usage              = D3D11_USAGE_DYNAMIC;
        texDesc.CPUAccessFlags     = D3D11_CPU_ACCESS_WRITE; // Only make sense for dynamic and staging textures
        texDesc.BindFlags          = D3D11_BIND_SHADER_RESOURCE;

        check_hr(mDev->CreateTexture2D(&texDesc, 0, &mTexture2D));

        check_hr(mDev->CreateShaderResourceView(mTexture2D, 0, &mSRV));

        validateSRV_Texture2D();
    }
        break;
    case Texture::Texture_3D:
    {
        D3D11_TEXTURE3D_DESC texDesc;

        texDesc.Width          = mWidth;
        texDesc.Height         = mHeight;
        texDesc.Depth          = mDepth;
        texDesc.Format         = mFormat;
        texDesc.MipLevels      = mMipLevels;
        texDesc.MiscFlags      = 0;
        texDesc.Usage          = D3D11_USAGE_DYNAMIC;
        texDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; // Only make sense for dynamic and staging textures
        texDesc.BindFlags      = D3D11_BIND_SHADER_RESOURCE;

        check_hr(mDev->CreateTexture3D(&texDesc, 0, &mTexture3D));

        check_hr(mDev->CreateShaderResourceView(mTexture3D, 0, &mSRV));

        validateSRV_Texture3D();
    }
        break;
    default:
        break;
    }

    intializeImageCache();

    mIsInitalized = true;
    return true;
}

bool Texture::initializeAsRenderTarget(DXGI_FORMAT format, uint32_t width, uint32_t height, uint32_t arraySize /*= 1*/, uint32_t mip /*= 1*/, uint32_t msCount /*= 1*/, uint32_t msQuality /*= 0 */)
{
    if (mIsInitalized)
        return false;

    if (msCount > 1) mip = 1;

    mDev = Renderer::getInstance()->getDevice();
    mCtx = Renderer::getInstance()->getContext();

    mUsage              = EUsage::RenderTargetAndShaderResource;
    mFormat             = format;
    mWidth              = width;
    mHeight             = height;
    mDepth              = 1;
    mArraySize          = arraySize;
    mMipLevels          = mip;
    mMultiSampleCount   = msCount;
    mMultiSampleQuality = msQuality;

    determineTextureDimension();

    switch (mDimension)
    {
    case Texture::Texture_1D:
    {
        D3D11_TEXTURE1D_DESC texDesc;

        texDesc.Width          = mWidth;
        texDesc.Format         = mFormat;
        texDesc.MipLevels      = mMipLevels;
        texDesc.ArraySize      = mArraySize;
        texDesc.MiscFlags      = 0;
        texDesc.Usage          = D3D11_USAGE_DEFAULT;
        texDesc.BindFlags      = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
        texDesc.CPUAccessFlags = 0;

        check_hr(mDev->CreateTexture1D(&texDesc, 0, &mTexture1D));

        check_hr(mDev->CreateRenderTargetView(mTexture1D, 0, &mRTV));

        check_hr(mDev->CreateShaderResourceView(mTexture1D, 0, &mSRV));

        validateSRV_Texture1D();
    }
        break;

    case Texture::Texture_2D:
    {
        D3D11_TEXTURE2D_DESC texDesc;

        texDesc.Width              = mWidth;
        texDesc.Height             = mHeight;
        texDesc.Format             = mFormat;
        texDesc.MipLevels          = mMipLevels;
        texDesc.ArraySize          = mArraySize;
        texDesc.SampleDesc.Count   = mMultiSampleCount;
        texDesc.SampleDesc.Quality = mMultiSampleQuality;
        texDesc.MiscFlags          = 0;
        texDesc.Usage              = D3D11_USAGE_DEFAULT;
        texDesc.BindFlags          = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
        texDesc.CPUAccessFlags     = 0;

        check_hr(mDev->CreateTexture2D(&texDesc, 0, &mTexture2D));

        check_hr(mDev->CreateRenderTargetView(mTexture2D, 0, &mRTV));

        check_hr(mDev->CreateShaderResourceView(mTexture2D, 0, &mSRV));

        validateSRV_Texture2D();
    }
        break;

    case Texture::Texture_3D:
    default:
        break;
    }

    mIsInitalized = true;
    return true;
}

bool Texture::initializeAsDepthStencil(DXGI_FORMAT resFormat, DXGI_FORMAT dsvFormat, DXGI_FORMAT srvFormat, uint32_t width, uint32_t height, uint32_t arraySize /*= 1*/, uint32_t mip /*= 1*/, uint32_t msCount /*= 1*/, uint32_t msQuality /*= 0*/)
{
    if (mIsInitalized)
        return false;

    if (msCount > 1) mip = 1;

    // For a depth stencil to be referenced by DSV and SRV at the same time, the depth stencil itself must be created
    // with a typeless format, which allows for DSV and SRV to use their own formats.
    // In d3d11, the formats used by resource views must be cast-able from the typeless format used by the resource.
    // i.e. they are in the same 'typeless family'.

    check(DxgiFormatUtil::isTypeless(resFormat) == true);
    check(DxgiFormatUtil::isDepthStencil(dsvFormat) == true);
    check(DxgiFormatUtil::bitsPerColor(resFormat) == DxgiFormatUtil::bitsPerColor(dsvFormat));
    check(DxgiFormatUtil::bitsPerPixel(dsvFormat) == DxgiFormatUtil::bitsPerPixel(srvFormat));

    mDev = Renderer::getInstance()->getDevice();
    mCtx = Renderer::getInstance()->getContext();

    mUsage              = EUsage::DepthStencilAndShaderResource;
    mFormat             = resFormat;
    mWidth              = width;
    mHeight             = height;
    mDepth              = 1;
    mArraySize          = arraySize;
    mMipLevels          = mip;
    mMultiSampleCount   = msCount;
    mMultiSampleQuality = msQuality;

    determineTextureDimension();

    switch (mDimension)
    {
    case Texture::Texture_1D:
    {
        D3D11_TEXTURE1D_DESC texDesc;

        texDesc.Width          = mWidth;
        texDesc.Format         = mFormat;
        texDesc.MipLevels      = mMipLevels;
        texDesc.ArraySize      = mArraySize;
        texDesc.MiscFlags      = 0;
        texDesc.Usage          = D3D11_USAGE_DEFAULT;
        texDesc.BindFlags      = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_DEPTH_STENCIL;
        texDesc.CPUAccessFlags = 0;

        //
        // Create the underlying 1D resource
        //
        check_hr(mDev->CreateTexture1D(&texDesc, 0, &mTexture1D));

        //
        // Create a dsv that has full access to the depth stencil resource
        //
        D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
        dsvDesc.Format = dsvFormat;
        dsvDesc.Flags = 0;  // 0 indicates this dsv is not read-only

        if (mArraySize > 1)
        {
            dsvDesc.ViewDimension                  = D3D11_DSV_DIMENSION_TEXTURE1DARRAY;
            dsvDesc.Texture1DArray.FirstArraySlice = 0;
            dsvDesc.Texture1DArray.ArraySize       = mArraySize;
            dsvDesc.Texture1DArray.MipSlice        = 0;
        }
        else
        {
            dsvDesc.ViewDimension      = D3D11_DSV_DIMENSION_TEXTURE1D;
            dsvDesc.Texture1D.MipSlice = 0;
        }
        check_hr(mDev->CreateDepthStencilView(mTexture1D, &dsvDesc, &mDSV));

        //
        // Create a srv that has full access to the depth stencil resource
        //
        D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
        srvDesc.Format = srvFormat;
        if (mArraySize > 1)
        {
            srvDesc.ViewDimension                  = D3D_SRV_DIMENSION_TEXTURE1DARRAY;
            srvDesc.Texture1DArray.FirstArraySlice = 0;
            srvDesc.Texture1DArray.ArraySize       = mArraySize;
            srvDesc.Texture1DArray.MostDetailedMip = 0;
            srvDesc.Texture1DArray.MipLevels       = mMipLevels;
        }
        else
        {
            srvDesc.ViewDimension             = D3D_SRV_DIMENSION_TEXTURE1D;
            srvDesc.Texture1D.MipLevels       = mMipLevels;
            srvDesc.Texture1D.MostDetailedMip = 0;
        }
        check_hr(mDev->CreateShaderResourceView(mTexture1D, 0, &mSRV));
    }
        break;

    case Texture::Texture_2D:
    {
        D3D11_TEXTURE2D_DESC texDesc;

        texDesc.Width              = mWidth;
        texDesc.Height             = mHeight;
        texDesc.Format             = mFormat;
        texDesc.MipLevels          = mMipLevels;
        texDesc.ArraySize          = mArraySize;
        texDesc.SampleDesc.Count   = mMultiSampleCount;
        texDesc.SampleDesc.Quality = mMultiSampleQuality;
        texDesc.MiscFlags          = 0;
        texDesc.Usage              = D3D11_USAGE_DEFAULT;
        texDesc.BindFlags          = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_DEPTH_STENCIL;
        texDesc.CPUAccessFlags     = 0;

        //
        // Create the underlying 2D resource
        //
        check_hr(mDev->CreateTexture2D(&texDesc, 0, &mTexture2D));

        //
        // Create a dsv that has full access to the depth stencil resource
        //
        D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
        dsvDesc.Format = dsvFormat;
        dsvDesc.Flags = 0;  // 0 indicates this dsv is not read-only
        // Array
        if (mArraySize > 1)
        {
            if (isMultisamplingEnabled())
            {
                dsvDesc.ViewDimension                    = D3D11_DSV_DIMENSION_TEXTURE2DMSARRAY;
                dsvDesc.Texture2DMSArray.ArraySize       = mArraySize;
                dsvDesc.Texture2DMSArray.FirstArraySlice = 0;
            }
            else
            {
                dsvDesc.ViewDimension                  = D3D11_DSV_DIMENSION_TEXTURE2DARRAY;
                dsvDesc.Texture2DArray.ArraySize       = mArraySize;
                dsvDesc.Texture2DArray.FirstArraySlice = 0;
                dsvDesc.Texture2DArray.MipSlice        = 0;
            }
        }
        // Single
        else
        {
            if (isMultisamplingEnabled())
            {
                dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
                dsvDesc.Texture2DMS.UnusedField_NothingToDefine;
            }
            else
            {
                dsvDesc.ViewDimension      = D3D11_DSV_DIMENSION_TEXTURE2D;
                dsvDesc.Texture2D.MipSlice = 0;
            }
        }
        check_hr(mDev->CreateDepthStencilView(mTexture2D, &dsvDesc, &mDSV));

        //
        // Create a srv that has full access to the depth stencil resource
        //
        D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
        srvDesc.Format = srvFormat;
        // Array
        if (mArraySize > 1)
        {
            if (isMultisamplingEnabled())
            {
                srvDesc.ViewDimension                    = D3D11_SRV_DIMENSION_TEXTURE2DMSARRAY;
                srvDesc.Texture2DMSArray.ArraySize       = mArraySize;
                srvDesc.Texture2DMSArray.FirstArraySlice = 0;
            }
            else
            {
                srvDesc.ViewDimension                  = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
                srvDesc.Texture2DArray.ArraySize       = mArraySize;
                srvDesc.Texture2DArray.FirstArraySlice = 0;
                srvDesc.Texture2DArray.MipLevels       = mMipLevels;
                srvDesc.Texture2DArray.MostDetailedMip = 0;
            }
        }
        // Single
        else
        {
            if (isMultisamplingEnabled())
            {
                srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DMS;
                srvDesc.Texture2DMS.UnusedField_NothingToDefine;
            }
            else
            {
                srvDesc.ViewDimension             = D3D11_SRV_DIMENSION_TEXTURE2D;
                srvDesc.Texture2D.MipLevels       = mMipLevels;
                srvDesc.Texture2D.MostDetailedMip = 0;
            }
        }
        check_hr(mDev->CreateShaderResourceView(mTexture2D, &srvDesc, &mSRV));
    }
        break;

    case Texture::Texture_3D:
    default:
        break;
    }

    mIsInitalized = true;
    return true;
}

bool Texture::initializeAsImmutableFromImage(const Image& srcImg, uint32_t msCount, uint32_t msQuality)
{
    if (mIsInitalized)
        return false;

    if (!srcImg.isInitialized())
        return false;

    mDev = Renderer::getInstance()->getDevice();
    mCtx = Renderer::getInstance()->getContext();

    mUsage              = EUsage::ImmutableShaderResource;
    mFormat             = srcImg.getDxgiFormat();
    mWidth              = srcImg.getWidth();
    mHeight             = srcImg.getHeight();
    mDepth              = srcImg.getDepth();
    mArraySize          = srcImg.getArraySize();
    mMipLevels          = 1;
    mMultiSampleCount   = msCount;
    mMultiSampleQuality = msQuality;

    determineTextureDimension();

    switch (mDimension)
    {
    case Texture::Texture_1D:
    {
        D3D11_TEXTURE1D_DESC texDesc;
        texDesc.Width          = mWidth;
        texDesc.Format         = mFormat;
        texDesc.MipLevels      = mMipLevels;
        texDesc.ArraySize      = mArraySize;
        texDesc.MiscFlags      = 0;
        texDesc.Usage          = D3D11_USAGE_IMMUTABLE;
        texDesc.BindFlags      = D3D11_BIND_SHADER_RESOURCE;
        texDesc.CPUAccessFlags = 0;

        std::vector<D3D11_SUBRESOURCE_DATA> srd;
        srd.resize(mArraySize);
        for (uint32_t i = 0; i < mArraySize; ++i)
        {
            const std::vector<uint8_t>& surface = srcImg.getSurface(i);

            check(!surface.empty());

            srd[i].pSysMem = &surface[0];
            srd[i].SysMemPitch = srcImg.getRowPitch();
            srd[i].SysMemSlicePitch = srcImg.getSlicePitch();
        }

        check_hr(mDev->CreateTexture1D(&texDesc, &srd[0], &mTexture1D));

        check_hr(mDev->CreateShaderResourceView(mTexture1D, nullptr, &mSRV));
    }
        break;
    case Texture::Texture_2D:
    {
        D3D11_TEXTURE2D_DESC texDesc;

        texDesc.Width              = mWidth;
        texDesc.Height             = mHeight;
        texDesc.Format             = mFormat;
        texDesc.MipLevels          = mMipLevels;
        texDesc.ArraySize          = mArraySize;
        texDesc.SampleDesc.Count   = mMultiSampleCount;
        texDesc.SampleDesc.Quality = mMultiSampleQuality;
        texDesc.MiscFlags          = 0;
        texDesc.Usage              = D3D11_USAGE_IMMUTABLE;
        texDesc.BindFlags          = D3D11_BIND_SHADER_RESOURCE;
        texDesc.CPUAccessFlags     = 0;

        //
        // Create resource and related views
        //
        std::vector<D3D11_SUBRESOURCE_DATA> srd;
        srd.resize(mArraySize);
        for (uint32_t i = 0; i < mArraySize; ++i)
        {
            const std::vector<uint8_t>& surface = srcImg.getSurface(i);

            check(!surface.empty());

            srd[i].pSysMem          = &surface[0];
            srd[i].SysMemPitch      = srcImg.getRowPitch();
            srd[i].SysMemSlicePitch = srcImg.getSlicePitch();
        }

        check_hr(mDev->CreateTexture2D(&texDesc, &srd[0], &mTexture2D));

        check_hr(mDev->CreateShaderResourceView(mTexture2D, 0, &mSRV));
    }
        break;

    case Texture::Texture_3D:
    {
        D3D11_TEXTURE3D_DESC texDesc;

        texDesc.Width          = mWidth;
        texDesc.Height         = mHeight;
        texDesc.Depth          = mDepth;
        texDesc.Format         = mFormat;
        texDesc.MipLevels      = mMipLevels;
        texDesc.MiscFlags      = 0;
        texDesc.Usage          = D3D11_USAGE_IMMUTABLE;
        texDesc.BindFlags      = D3D11_BIND_SHADER_RESOURCE;
        texDesc.CPUAccessFlags = 0;

        //
        // Create resource and related views
        //
        std::vector<D3D11_SUBRESOURCE_DATA> srd;
        srd.resize(mDepth);
        for (uint32_t depthSlice = 0; depthSlice < mDepth; ++depthSlice)
        {
            const std::vector<uint8_t>& surface = srcImg.getSurface(depthSlice);

            check(!surface.empty());

            srd[depthSlice].pSysMem          = &surface[0];
            srd[depthSlice].SysMemPitch      = srcImg.getRowPitch();
            srd[depthSlice].SysMemSlicePitch = srcImg.getSlicePitch();
        }

        check_hr(mDev->CreateTexture3D(&texDesc, &srd[0], &mTexture3D));

        check_hr(mDev->CreateShaderResourceView(mTexture3D, 0, &mSRV));
    }
        break;
    default:
        break;
    }

    mIsInitalized = true;
    return true;
}


void Texture::destroy()
{
    mDev                = nullptr;
    mCtx                = nullptr;

    mUsage              = EUsage::DynamicShaderResource; 
    mFormat             = DXGI_FORMAT_UNKNOWN; 
    mDimension          = EDimension::Texture_2D;

    mWidth              = 0; 
    mHeight             = 0; 
    mDepth              = 0;

    mArraySize          = 0; 
    mMipLevels          = 1;

    mMultiSampleCount   = 1;
    mMultiSampleQuality = 0;

    safe_delete(mImageCache);

    safe_release(mSRV);
    safe_release(mRTV);
    safe_release(mDSV);
    safe_release(mUAV);

    safe_release(mTexture1D);
    safe_release(mTexture2D);
    safe_release(mTexture3D);

    mIsImageCacheDirty = false;
    mIsInitalized = false;
}


Image& Texture::updateImageCache()
{
    mIsImageCacheDirty = true;
    return *mImageCache;
}

void Texture::commitImageCache_Tex1D()
{
    if (mIsImageCacheDirty)
    {
        mIsImageCacheDirty = false;

        D3D11_MAPPED_SUBRESOURCE msr;

        const std::vector<uint8_t>& srcSurface = mImageCache->getSurface(0);

        mCtx->Map(mTexture1D, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

        memcpy(msr.pData, &srcSurface[0], mImageCache->getSlicePitch());

        mCtx->Unmap(mTexture1D, 0);
    }
}

void Texture::commitImageCache_Tex2D()
{
    if (mIsImageCacheDirty)
    {
        mIsImageCacheDirty = false;

        D3D11_MAPPED_SUBRESOURCE msr;

        const std::vector<uint8_t>& srcSurface = mImageCache->getSurface(0);

        mCtx->Map(mTexture2D, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

        memcpy(msr.pData, &srcSurface[0], mImageCache->getSlicePitch());

        mCtx->Unmap(mTexture2D, 0);
    }
}

void Texture::commitImageCache_Tex3D()
{
    if (mIsImageCacheDirty)
    {
        mIsImageCacheDirty = false;

        D3D11_MAPPED_SUBRESOURCE msr;

        for (uint32_t depthSlice = 0; depthSlice < mImageCache->getDepth(); ++ depthSlice)
        {
            const std::vector<uint8_t>& srcSurface = mImageCache->getSurface(depthSlice);

            mCtx->Map(mTexture3D, depthSlice, D3D11_MAP_WRITE_DISCARD, 0, &msr);

            memcpy(msr.pData, &srcSurface[0], mImageCache->getSlicePitch());

            mCtx->Unmap(mTexture3D, depthSlice);
        }
    }
}

void Texture::bindAsShaderResource(EShaderStage ss, uint32_t slot)
{
    switch (ss)
    {
    case VertexShader_SM5:
        mCtx->VSSetShaderResources(slot, 1, &mSRV);
        break;

    case PixelShader_SM5:
        mCtx->PSSetShaderResources(slot, 1, &mSRV);
        break;

    case GeometryShader_SM5:
        mCtx->GSSetShaderResources(slot, 1, &mSRV);
        break;

    case HullShader_SM5:
        mCtx->HSSetShaderResources(slot, 1, &mSRV);
        break;

    case DomainShader_SM5:
        mCtx->DSSetShaderResources(slot, 1, &mSRV);
        break;

    case ComputeShader_SM5:
        mCtx->CSSetShaderResources(slot, 1, &mSRV);
        break;

    default:
        break;
    }
}

void Texture::determineTextureDimension()
{
    if (mDepth != 1)
    {
        mDimension = EDimension::Texture_3D;
    }
    else if (mHeight != 1)
    {
        mDimension = EDimension::Texture_2D;
    }
    else
    {
        mDimension = EDimension::Texture_1D;
    }
}

void Texture::intializeImageCache()
{
    safe_delete(mImageCache);
    mImageCache = new Image();
    mImageCache->createAsEmptyImage(mFormat, mWidth, mHeight, mDepth, mArraySize);
}

void Texture::validateSRV_Texture1D()
{
    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
    mSRV->GetDesc(&srvDesc);
    // Array
    if (mArraySize > 1) 
    {
        check(srvDesc.ViewDimension == D3D_SRV_DIMENSION_TEXTURE1DARRAY);
        check(srvDesc.Texture1DArray.ArraySize == mArraySize);
        check(srvDesc.Texture1DArray.FirstArraySlice == 0);
        check(srvDesc.Texture1DArray.MipLevels == mMipLevels);
        check(srvDesc.Texture1DArray.FirstArraySlice == 0);
    }
    // Single 
    else 
    {
        check(srvDesc.ViewDimension == D3D_SRV_DIMENSION_TEXTURE1D);
        check(srvDesc.Texture1D.MipLevels == mMipLevels);
        check(srvDesc.Texture1D.MostDetailedMip == 0);
    }
}

void Texture::validateSRV_Texture2D()
{
    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
    mSRV->GetDesc(&srvDesc);

    // Array
    if (mArraySize > 1) 
    {
        if (isMultisamplingEnabled()) 
        {
            check(srvDesc.ViewDimension == D3D_SRV_DIMENSION_TEXTURE2DMSARRAY);
            check(srvDesc.Texture2DMSArray.ArraySize == mArraySize);
            check(srvDesc.Texture2DMSArray.FirstArraySlice == 0);
        }
        else 
        {
            check(srvDesc.ViewDimension == D3D_SRV_DIMENSION_TEXTURE2DARRAY);
            check(srvDesc.Texture2DArray.ArraySize == mArraySize);
            check(srvDesc.Texture2DArray.FirstArraySlice== 0);
            check(srvDesc.Texture2DArray.MipLevels == mMipLevels);
            check(srvDesc.Texture2DArray.MostDetailedMip == 0);
        }
    }
    // Single 
    else 
    {
        if (isMultisamplingEnabled()) 
        {
            check(srvDesc.ViewDimension == D3D_SRV_DIMENSION_TEXTURE2DMS);
        }
        else 
        {
            check(srvDesc.ViewDimension == D3D_SRV_DIMENSION_TEXTURE2D);
            check(srvDesc.Texture2D.MipLevels == mMipLevels);
            check(srvDesc.Texture2D.MostDetailedMip == 0);
        }
    }
}

void Texture::validateSRV_Texture3D()
{
    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
    mSRV->GetDesc(&srvDesc);

    check(srvDesc.ViewDimension == D3D_SRV_DIMENSION_TEXTURE3D);
    check(srvDesc.Texture3D.MipLevels == mMipLevels);
    check(srvDesc.Texture3D.MostDetailedMip == 0);
}

void Texture::validateSRV_TextureCube()
{
    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
    mSRV->GetDesc(&srvDesc);

    check(mArraySize % 6 == 0);
    // Cube array
    if (mArraySize / 6 > 1) 
    {
        check(srvDesc.ViewDimension == D3D_SRV_DIMENSION_TEXTURECUBEARRAY);
        check(srvDesc.TextureCubeArray.MostDetailedMip == 0);
        check(srvDesc.TextureCubeArray.MipLevels == mMipLevels);
        check(srvDesc.TextureCubeArray.NumCubes == mArraySize / 6);
        check(srvDesc.TextureCubeArray.First2DArrayFace == 0);
    }
    // Single cube
    else 
    {
        check(mArraySize == 6);
        check(srvDesc.ViewDimension == D3D_SRV_DIMENSION_TEXTURECUBE);
        check(srvDesc.TextureCube.MipLevels == mMipLevels);
        check(srvDesc.TextureCube.MostDetailedMip == 0);
    }
}

void Texture::debugSave(const std::wstring& filepathname, Image::EImageFileFormat fileFormat)
{
    if (!mIsInitalized) return;

    ID3D11Texture1D* stagingTex1D = nullptr;
    ID3D11Texture2D* stagingTex2D = nullptr;
    ID3D11Texture3D* stagingTex3D = nullptr;

    // Init Image
    Image dstImage;
    dstImage.createAsEmptyImage(mFormat, mWidth, mHeight, mDepth, mArraySize);

    switch (mDimension)
    {
    case Texture::Texture_1D:
    {
        // Create a 1D staging texture
        D3D11_TEXTURE1D_DESC texDesc;
        texDesc.Width          = mWidth;
        texDesc.MipLevels      = mMipLevels;
        texDesc.ArraySize      = mArraySize;
        texDesc.Format         = mFormat;
        texDesc.Usage          = D3D11_USAGE_STAGING;
        texDesc.BindFlags      = 0;
        texDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
        texDesc.MiscFlags      = 0;

        check_hr(mDev->CreateTexture1D(&texDesc, 0, &stagingTex1D));
        // Copy this texture to the staging one
        mCtx->CopyResource(stagingTex1D, mTexture1D);

        // Map the Staging texture to copy all its sub resources into dstImage
        std::vector< D3D11_MAPPED_SUBRESOURCE> msr;
        msr.resize(mArraySize);
        for (uint32_t arrayIndex = 0; arrayIndex < mArraySize; ++arrayIndex)
        {
            std::vector<uint8_t>& dstSurface = dstImage.getSurface(arrayIndex);

            // We only want the top level mip map, as Image does not support mip map for now.
            uint32_t topLvlMipId = arrayIndex * mMipLevels;

            mCtx->Map(stagingTex1D, topLvlMipId, D3D11_MAP_READ, 0, &msr[arrayIndex]);

            check(msr[arrayIndex].DepthPitch == dstImage.getSlicePitch());

            memcpy(&dstSurface[0], msr[arrayIndex].pData, msr[arrayIndex].DepthPitch);

            mCtx->Unmap(stagingTex1D, topLvlMipId);
        }
    }
        break;
    case Texture::Texture_2D:
    {
        // Create a 2D staging texture
        D3D11_TEXTURE2D_DESC texDesc;
        texDesc.Width              = mWidth;
        texDesc.Height             = mHeight;
        texDesc.Format             = mFormat;
        texDesc.MipLevels          = mMipLevels;
        texDesc.ArraySize          = mArraySize;
        texDesc.SampleDesc.Count   = mMultiSampleCount;
        texDesc.SampleDesc.Quality = mMultiSampleQuality;
        texDesc.Usage              = D3D11_USAGE_STAGING;
        texDesc.BindFlags          = 0;
        texDesc.CPUAccessFlags     = D3D11_CPU_ACCESS_READ;
        texDesc.MiscFlags          = 0;
        check_hr(mDev->CreateTexture2D(&texDesc, 0, &stagingTex2D));

        // Copy this texture to the staging one
        mCtx->CopyResource(stagingTex2D, mTexture2D);

        // Map the Staging texture to copy all its sub resources into dstImage
        std::vector< D3D11_MAPPED_SUBRESOURCE> msr;
        msr.resize(mArraySize);
        for (uint32_t arrayIndex = 0; arrayIndex < mArraySize; ++arrayIndex)
        {
            std::vector<uint8_t>& dstSurface = dstImage.getSurface(arrayIndex);

            // We only want the top level mip map, as Image does not support mip map for now.
            uint32_t topLvlMipId = arrayIndex * mMipLevels;

            mCtx->Map(stagingTex2D, topLvlMipId, D3D11_MAP_READ, 0, &msr[arrayIndex]);

            check(msr[arrayIndex].DepthPitch == dstImage.getSlicePitch());

            memcpy(&dstSurface[0], msr[arrayIndex].pData, msr[arrayIndex].DepthPitch);

            mCtx->Unmap(stagingTex2D, topLvlMipId);
        }
    }
        break;
    case Texture::Texture_3D:
    {
        // Create a 3D staging texture
        D3D11_TEXTURE3D_DESC texDesc;
        texDesc.Width          =  mWidth;
        texDesc.Height         = mHeight;
        texDesc.Depth          = mDepth;
        texDesc.MipLevels      = mMipLevels;
        texDesc.Format         = mFormat;
        texDesc.Usage          = D3D11_USAGE_STAGING;
        texDesc.BindFlags      = 0;
        texDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
        texDesc.MiscFlags      = 0;
        check_hr(mDev->CreateTexture3D(&texDesc, 0, &stagingTex3D));

        // Copy this texture to the staging one
        mCtx->CopyResource(stagingTex3D, mTexture3D);

        // Map the Staging texture to copy all its sub resources into dstImage
        std::vector< D3D11_MAPPED_SUBRESOURCE> msr;
        msr.resize(mDepth);
        for (uint32_t depthSlice = 0; depthSlice < mDepth; ++depthSlice)
        {
            std::vector<uint8_t>& dstSurface = dstImage.getSurface(depthSlice);

            mCtx->Map(stagingTex2D, depthSlice, D3D11_MAP_READ, 0, &msr[depthSlice]);

            check(msr[depthSlice].DepthPitch == dstImage.getSlicePitch());

            memcpy(&dstSurface[0], msr[depthSlice].pData, msr[depthSlice].DepthPitch);

            mCtx->Unmap(stagingTex2D, depthSlice);
        }
    }
        break;
    default:
        break;
    }

    // Write image file
    dstImage.saveToFile(filepathname, fileFormat );

    safe_release(stagingTex1D);
    safe_release(stagingTex2D);
    safe_release(stagingTex3D);
}

uint32_t Texture::getMemoryUsage() const
{
    return DxgiFormatUtil::bitsPerPixel(mFormat) * mWidth * mHeight * mDepth / 8;
}

#include "Texture.h"
#include "Renderer.h"

DepthStencil::DepthStencil(uint32_t width, uint32_t height, DXGI_FORMAT format, uint32_t inMSAACount, uint32_t inMSAAQuality) 
    : Texture2D(width, height, format)
{
    mMSAACount     = inMSAACount;
    mMSAAQuality   = inMSAAQuality;
}

DepthStencil::~DepthStencil()
{
    destroy();
}

void DepthStencil::create()
{
    if (mTexture) return;

    mDev = Renderer::getInstance()->getDevice();
    mCtx = Renderer::getInstance()->getContext();


    D3D11_TEXTURE2D_DESC dsDesc;

    dsDesc.Width     = mWidth;
    dsDesc.Height    = mHeight;
    dsDesc.MipLevels = 1;
    dsDesc.ArraySize = 1;
    dsDesc.Format    = mTextureFormat;

    if (mMSAACount > 0) // Use 4X MSAA? --must match swap chain MSAA values.
    {
        dsDesc.SampleDesc.Count = mMSAACount;
        dsDesc.SampleDesc.Quality = mMSAAQuality;
    }
    else // No MSAA
    {
        dsDesc.SampleDesc.Count = 1;
        dsDesc.SampleDesc.Quality = 0;
    }

    // D3D11_USAGE_DEFAULT -- a resource that requires read and write access by the GPU.
    dsDesc.Usage          = D3D11_USAGE_DEFAULT;
    dsDesc.BindFlags      = D3D11_BIND_DEPTH_STENCIL;
    dsDesc.CPUAccessFlags = 0; // Not sure if I can set it to Read 
    dsDesc.MiscFlags      = 0;

    check_hr(mDev->CreateTexture2D(&dsDesc, 0, &mTexture));
    check_hr(mDev->CreateDepthStencilView(mTexture, 0, &mDSV));
}

void DepthStencil::resize(uint32_t newWidth, uint32_t newHeight)
{
    if (mWidth == newWidth && mHeight == newHeight)
    {
        return;
    }

    destroy();
    mWidth = newWidth;
    mHeight = newHeight;
    create();
}

DynamicTexture2D::DynamicTexture2D(uint32_t width, uint32_t height, DXGI_FORMAT format) : 
    Texture2D(width, height, format),
    mCacheTouched(false)
{
    mImageCache.initialize(width, height, format);
}

void DynamicTexture2D::create()
{
    if (mTexture) return;

    mDev = Renderer::getInstance()->getDevice();
    mCtx = Renderer::getInstance()->getContext();

    D3D11_TEXTURE2D_DESC txDesc;

    txDesc.Width              = mWidth;
    txDesc.Height             = mHeight;
    txDesc.MipLevels          = 1;
    txDesc.ArraySize          = 1;
    txDesc.Format             = mTextureFormat;
    txDesc.SampleDesc.Count   = 1;
    txDesc.SampleDesc.Quality = 0;
    txDesc.Usage              = D3D11_USAGE_DYNAMIC;
    txDesc.BindFlags          = D3D11_BIND_SHADER_RESOURCE;
    txDesc.CPUAccessFlags     = D3D11_CPU_ACCESS_WRITE;
    txDesc.MiscFlags          = 0;

    D3D11_SUBRESOURCE_DATA srd;
    srd.pSysMem          = &mImageCache.getRawData()[0];
    srd.SysMemPitch      = mImageCache.getRowPitch();
    srd.SysMemSlicePitch = mImageCache.getSlicePitch();

    check_hr(mDev->CreateTexture2D(&txDesc, &srd, &mTexture));
    check_hr(mDev->CreateShaderResourceView(mTexture, 0, &mSRV));
}

Image2D& DynamicTexture2D::update()
{
    mCacheTouched = true;
    return mImageCache;
}

void DynamicTexture2D::commit()
{
    if (mCacheTouched)
    {
        D3D11_MAPPED_SUBRESOURCE msr;
        mCtx->Map(mTexture, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
        memcpy(msr.pData, &mImageCache.getRawData()[0], mImageCache.getSlicePitch());
        mCtx->Unmap(mTexture, 0);

        mCacheTouched = false;
    }
}

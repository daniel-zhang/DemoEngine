#pragma once

#include "RendererCommon.h"
#include "Image2D.h"

class Texture2D : public IRenderResource
{
public:
    virtual const wchar_t* getResourceName() override { return L"Texture"; }

    // Force implementation in concrete classes
    virtual void create() override = 0;

    virtual void destroy() override 
    {
        safe_release(mSRV);
        safe_release(mDSV);
        safe_release(mRTV);
        safe_release(mTexture);
    }

    virtual void recreate() override 
    {
        destroy();
        create();
    }

public:
    Texture2D(uint32_t width, uint32_t height, DXGI_FORMAT format) :
        mDev(nullptr), mCtx(nullptr),  
        mWidth(width), mHeight(height), 
        mTextureFormat(format),
        mTexture(nullptr), 
        mDSV(nullptr), mSRV(nullptr), mRTV(nullptr)
    { }

    virtual ~Texture2D() { destroy(); }

    template<EShaderStage ss>
    void bind(uint32_t slot)
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
        case DomainShader_SM5:
        case ComputeShader_SM5:
            break;

        default:
            break;
        }
    }

    inline ID3D11DepthStencilView* getDSV() const { return mDSV; }

    inline ID3D11ShaderResourceView* getSRV() const { return mSRV; }

    inline ID3D11RenderTargetView* getRTV() const { return mRTV; }

    inline uint32_t getWidth() const { return mWidth; }

    inline uint32_t getHeight() const { return mHeight; }

    inline DXGI_FORMAT getPixelFormat() const { return mTextureFormat; }

protected:
    ID3D11Device* mDev;
    ID3D11DeviceContext* mCtx;

    DXGI_FORMAT mTextureFormat;
    uint32_t mWidth;
    uint32_t mHeight;

    ID3D11Texture2D* mTexture;

    // Not all of these have to be valid given a concrete texture2d object.
    ID3D11DepthStencilView* mDSV;
    ID3D11RenderTargetView* mRTV;
    ID3D11ShaderResourceView* mSRV;
};

// Static 2D texture that once created:
// CPU no access
// GPU read only (as a SRV)
class StaticTexture2D : public Texture2D
{
public:
    // TODO

};

// Dynamic 2D texture 
// CPU write only 
// GPU read only
class DynamicTexture2D : public Texture2D
{
public:
    DynamicTexture2D(uint32_t width, uint32_t height, DXGI_FORMAT format);

public:
    virtual const wchar_t* getResourceName() override { return L"DynamicTexture2D"; }

    virtual void create() override;

    Image2D& update();

    void commit();

    const Image2D& getImageCache() const { return mImageCache; }

protected:
    bool mCacheTouched;
    Image2D mImageCache;
};

// GPU read and write(as a DSV)
class DepthStencil : public Texture2D 
{
public:
    virtual const wchar_t* getResourceName() override { return L"DepthStencil"; }

    virtual void create() override;


public:
    DepthStencil(uint32_t width, uint32_t height, DXGI_FORMAT format, uint32_t inMSAACount, uint32_t inMSAAQuality);

    ~DepthStencil();

    void resize(uint32_t newWidth, uint32_t newHeight) ;

protected:
    uint32_t mMSAACount;
    uint32_t mMSAAQuality;
};
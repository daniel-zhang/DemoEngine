#pragma once

#include "RendererCommon.h"
#include "Texture.h"

/*
Transient structure, need to be in sync with texture format
*/
struct RenderTargetSpec
{
    RenderTargetSpec() : 
        FillHwndWindow(true),
        RequiredWidth(0), RequiredHeight(0), // RT will adapt the window size if these are set to zero.
        RequestedSampleCount(0), // 0 for no MSAA
        BackBufferFormat(DXGI_FORMAT_R8G8B8A8_UNORM), 

        EnableDepthStencil(true), 
        //DepthStencilFormat( DXGI_FORMAT_D24_UNORM_S8_UINT),
        DS_Format( DXGI_FORMAT_R32_TYPELESS),
        DSV_Format(DXGI_FORMAT_D32_FLOAT),
        DS_RSV_Format(DXGI_FORMAT_R32_FLOAT)
    {}
    bool FillHwndWindow;
    uint32_t RequiredWidth;
    uint32_t RequiredHeight;

    uint32_t RequestedSampleCount;

    DXGI_FORMAT BackBufferFormat;

    bool EnableDepthStencil;
    DXGI_FORMAT DS_Format;
    DXGI_FORMAT DSV_Format;
    DXGI_FORMAT DS_RSV_Format;
};

class RenderTarget : public IRenderResource
{
public:
    // IRenderResource interface
    virtual const wchar_t* getResourceName() override { return L"RenderTarget"; }

    virtual void create() override;

    virtual void destroy() override;

    virtual void recreate() override;

public:
    RenderTarget(HWND hwnd, const RenderTargetSpec& spec) : 
        mDev(nullptr), mCtx(nullptr), 
        mSwapChain(nullptr), mRTV(nullptr), 
        mDepthStencil(nullptr),
        mHwnd(hwnd), mWidth(0), mHeight(0), 
        mSupportedMSAAQualityLevel(0)
    {
        mSpec = spec;
    }

    ~RenderTarget() { destroy(); }

    void bind();

    void present();

    void resize(uint32_t width, uint32_t height);

    void clear(const Color& color);
    
protected:
    HWND mHwnd;
    ID3D11Device* mDev;
    ID3D11DeviceContext* mCtx;

    IDXGISwapChain* mSwapChain;
    ID3D11RenderTargetView*	mRTV;

    Texture* mDepthStencil;

    RenderTargetSpec mSpec;
    uint32_t mWidth; 
    uint32_t mHeight;

    uint32_t mSupportedMSAAQualityLevel;
};
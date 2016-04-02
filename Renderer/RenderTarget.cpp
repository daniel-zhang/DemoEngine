#include "RenderTarget.h"
#include "Renderer.h"

void RenderTarget::create()
{
    mDev = Renderer::getInstance()->getDevice();
    mCtx = Renderer::getInstance()->getContext();

    // Determine dimension
    if (mSpec.FillHwndWindow)
    {
        RECT clientArea;
        ::GetClientRect(mHwnd, &clientArea);
        mWidth = clientArea.right - clientArea.left;
        mHeight = clientArea.bottom - clientArea.top;
    }
    else 
    {
        mWidth = mSpec.RequiredWidth;
        mHeight = mSpec.RequiredHeight;
    }

    DXGI_SWAP_CHAIN_DESC scDesc;
    scDesc.BufferDesc.Width                   = mWidth;
    scDesc.BufferDesc.Height                  = mHeight;
    scDesc.BufferDesc.RefreshRate.Numerator   = 60;
    scDesc.BufferDesc.RefreshRate.Denominator = 1;
    scDesc.BufferDesc.Format                  = mSpec.BackBufferFormat;
    scDesc.BufferDesc.ScanlineOrdering        = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    scDesc.BufferDesc.Scaling                 = DXGI_MODE_SCALING_UNSPECIFIED;
    scDesc.BufferUsage                        = DXGI_USAGE_RENDER_TARGET_OUTPUT;

    scDesc.BufferCount  = 1;
    scDesc.OutputWindow = mHwnd;
    scDesc.Windowed     = true;
    // More on swap chain presentation models: https://msdn.microsoft.com/en-us/library/windows/desktop/bb173077(v=vs.85).aspx#DXGI_SWAP_EFFECT_DISCARD
    scDesc.SwapEffect   = DXGI_SWAP_EFFECT_DISCARD; // Default bit-block swap model
    scDesc.Flags        = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH; // Allow truly full screen

    if (mSpec.RequestedSampleCount > 0)
    {
        mDev->CheckMultisampleQualityLevels(
            scDesc.BufferDesc.Format,
            mSpec.RequestedSampleCount,
            &mSupportedMSAAQualityLevel
            );

        scDesc.SampleDesc.Count = mSpec.RequestedSampleCount;
        scDesc.SampleDesc.Quality = mSupportedMSAAQualityLevel - 1;
    }
    else
    {
        scDesc.SampleDesc.Count = 1;
        scDesc.SampleDesc.Quality = 0;
    }

    // Get the underlying IDXGIFactory to create the swapchain
    {
        IDXGIDevice* dxgiDevice = 0;
        check_hr(mDev->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice));

        IDXGIAdapter* dxgiAdapter = 0;
        check_hr(dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdapter));

        IDXGIFactory* dxgiFactory = 0;
        check_hr(dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory));

        check_hr(dxgiFactory->CreateSwapChain(mDev, &scDesc, &mSwapChain));

        safe_release(dxgiDevice);
        safe_release(dxgiAdapter);
        safe_release(dxgiFactory);
    }
    
    // Resize the swap chain and create the render target view.
    {
        check_hr(mSwapChain->ResizeBuffers(1, mWidth, mHeight, scDesc.BufferDesc.Format, 0));

        ID3D11Texture2D* backBuffer;
        check_hr(mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer)));
        check_hr(mDev->CreateRenderTargetView(backBuffer, 0, &mRTV));

        safe_release(backBuffer);
    }

    if (mSpec.EnableDepthStencil)
    {
        safe_delete(mDepthStencil);
        mDepthStencil = new Texture();
        mDepthStencil->initializeAsDepthStencil( mSpec.DS_Format, mSpec.DSV_Format, mSpec.DS_RSV_Format,
            scDesc.BufferDesc.Width, scDesc.BufferDesc.Height, 
            1,  // array size
            1,  // max mip levels
            scDesc.SampleDesc.Count, scDesc.SampleDesc.Quality);
    }
}

void RenderTarget::destroy()
{
    safe_release(mRTV);
    safe_release(mSwapChain);

    safe_delete(mDepthStencil);
}

void RenderTarget::recreate()
{
    destroy();
    recreate();
}

void RenderTarget::bind()
{
    if (mSpec.EnableDepthStencil)
    {
        ID3D11DepthStencilView* dsv = mDepthStencil->getDSV();
        check(dsv);
        mCtx->OMSetRenderTargets(1, &mRTV, dsv);
    }
    else
    {
        mCtx->OMSetRenderTargets(1, &mRTV, nullptr);
    }
}

void RenderTarget::present()
{
    mSwapChain->Present(0, 0);
}

void RenderTarget::resize(uint32_t width, uint32_t height)
{
    if (mWidth == width && mHeight == height)
    {
        return;
    }
    mWidth = width;
    mHeight = height;

    // Recreate RTV while keeping swap chain alive. Unlike textures, swap chain can be resized without recreated
    safe_release(mRTV);
    {
        check_hr(mSwapChain->ResizeBuffers(1, mWidth, mHeight, mSpec.BackBufferFormat, 0));
        ID3D11Texture2D* backBuffer;
        check_hr(mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer)));
        check_hr(mDev->CreateRenderTargetView(backBuffer, 0, &mRTV));
        safe_release(backBuffer);
    }

    // Recreate DepthStencil with new size 
    if (mSpec.EnableDepthStencil)
    {
        DXGI_SWAP_CHAIN_DESC scDesc;
        mSwapChain->GetDesc(&scDesc);

        mDepthStencil->destroy();
        mDepthStencil->initializeAsDepthStencil(mSpec.DS_Format, mSpec.DSV_Format, mSpec.DS_RSV_Format,
            scDesc.BufferDesc.Width, scDesc.BufferDesc.Height,
            1,  // array size
            1,  // max mip levels
            scDesc.SampleDesc.Count, scDesc.SampleDesc.Quality);
    }
}

void RenderTarget::clear(const Color& color)
{
    mCtx->ClearRenderTargetView(mRTV, reinterpret_cast<const float*>(&color));

    if (mSpec.EnableDepthStencil)
    {
        ID3D11DepthStencilView* dsv = mDepthStencil->getDSV();
        mCtx->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
    }
}

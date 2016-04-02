#include "Renderer.h"

RendererSettings::RendererSettings() :
    DriverType(D3D_DRIVER_TYPE_HARDWARE),
    FeatureLevel(D3D_FEATURE_LEVEL_11_0),
    SingleThreaded(true),

#if defined(_DEBUG) || defined(DEBUG)
    EnableDebugLayer(true),
#else
    EnableDebugLayer(false),
#endif

    EnableShaderDebug(false), 
    SupportBGRAFormat(false)
{
}

uint32_t extractCreationFlag(const RendererSettings& s)
{
    // MSDN about creation flags
    // https://msdn.microsoft.com/en-us/library/windows/desktop/ff476107(v=vs.85).aspx

    uint32_t flag = 0;

    if (s.EnableDebugLayer)
        flag |= D3D11_CREATE_DEVICE_DEBUG;

    if (s.EnableShaderDebug)
        flag |= D3D11_CREATE_DEVICE_DEBUGGABLE;

    if (s.SingleThreaded)
        flag |= D3D11_CREATE_DEVICE_SINGLETHREADED;

    if (s.SupportBGRAFormat)
        flag |= D3D11_CREATE_DEVICE_BGRA_SUPPORT;

    return flag;
}

Renderer::Renderer() :
    mDev(nullptr),
    mCtx(nullptr),
    mGDI(nullptr),
    mRenderStates(nullptr)
{ }

void Renderer::initialize(const RendererSettings& settings)
{
    // Skip reinitialize
    if (mDev && mCtx) return;

    mSettings = settings;

    check_hr( D3D11CreateDevice(
            NULL,
            mSettings.DriverType,
            0,
            extractCreationFlag(mSettings),
            NULL,
            0,
            D3D11_SDK_VERSION,
            &mDev,
            &mSettings.FeatureLevel,
            &mCtx 
    ));

    // Initialize global render states
    mRenderStates = RenderStates::getInstance();
    mRenderStates->destroy();
    mRenderStates->create();

    Sampler::getInstance()->initialize();

    // Initialize font library
    mFontLibrary.reset(new FontLibrary());
    mFontLibrary->initialize();

    //mGDI = GuiDrawInterface::getInstance();
    //mGDI = new GuiDrawInterface();
    mGDI.reset(new GuiDrawInterface());
    mGDI->initialize();

    // Debug: disable back face culling
    mCtx->RSSetState(mRenderStates->RasterizerState_Normal);
}

void Renderer::destroy()
{
    // Not really necessary for std::unique_ptr, just put these reset() here in case correct(inverse) destroy order is somehow required.
    mGDI.reset(nullptr);
    mFontLibrary.reset(nullptr);
    mRenderStates = nullptr;

    safe_release(mDev);
    safe_release(mCtx);
}

bool Renderer::createGuiRenderTarget(HWND hwnd)
{
    return mGDI->createRenderTarget(hwnd, RenderTargetSpec());
}

void Renderer::resizeGuiRenderTarget(HWND hwnd, const Vector2D& newSize)
{
    mGDI->resizeRenderTarget(hwnd, newSize);
}

bool Renderer::removeGuiRenderTarget(HWND hwnd)
{
    return mGDI->removeRenderTarget(hwnd);
}

void Renderer::drawGui(const VisualElementGroups& inGroups)
{
    mGDI->draw(inGroups);
}

std::unique_ptr<class FontLibrary>& Renderer::getFontLibrary()
{
    return mFontLibrary;
}


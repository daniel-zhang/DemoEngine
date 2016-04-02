#pragma once

#include "Core/Util.h"
#include "Core/Math.h"
#include <d3d11_2.h>

#include "VisualElementGroups.h"
#include "GuiDrawInterface.h"
#include "RenderStates.h"
#include "FontLibrary.h"

struct RendererSettings
{
    RendererSettings();

    D3D_DRIVER_TYPE DriverType;
    D3D_FEATURE_LEVEL FeatureLevel;
    bool SingleThreaded;
    bool EnableDebugLayer;
    bool EnableShaderDebug;
    bool SupportBGRAFormat;
};

class Renderer
{
public:
    static Renderer* getInstance() 
    {
        static Renderer instance;
        return &instance; 
    }

    ~Renderer() { destroy(); }

public:
    void initialize(const RendererSettings& settings);

    void destroy();

    bool createGuiRenderTarget(HWND hwnd);

    void resizeGuiRenderTarget(HWND hwnd, const Vector2D& newSize);

    bool removeGuiRenderTarget(HWND hwnd);
    
    void drawGui(const VisualElementGroups& inGroups);

public:
    inline ID3D11Device* getDevice(){ return mDev; }
    inline ID3D11DeviceContext* getContext(){ return mCtx; }
    std::unique_ptr<FontLibrary>& getFontLibrary();

private:
    Renderer();
    Renderer(const Renderer&);
    Renderer& operator=(const Renderer&);

private:
    RendererSettings mSettings;

    ID3D11Device* mDev;
    ID3D11DeviceContext* mCtx;

    RenderStates* mRenderStates;
    std::unique_ptr<FontLibrary> mFontLibrary;
    std::unique_ptr<GuiDrawInterface> mGDI;
    //class GuiDrawInterface* mGDI; // No, not that GDI/GDI+...

};
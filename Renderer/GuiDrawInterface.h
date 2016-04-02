#pragma once

#include "RendererCommon.h"
#include "VisualElementGroups.h"
#include "GuiBatcher.h"
#include "RenderTarget.h"
#include "Viewport.h"
#include "GeometryBuffer.h"
#include "RenderStates.h"
#include "ShaderLibrary.h"

class GuiDrawInterface
{
public:
    GuiDrawInterface();
    ~GuiDrawInterface(){ destroy(); }
    GuiDrawInterface(const GuiDrawInterface&) = delete;
    GuiDrawInterface& operator=(const GuiDrawInterface&) = delete;

public:
    void initialize();

    void destroy();

    bool createRenderTarget(HWND hwnd, const RenderTargetSpec& inRTSpec);

    bool removeRenderTarget(HWND hwnd);

    void resizeRenderTarget(HWND hwnd, const Vector2D& newSize);

    void draw(const VisualElementGroups& inGroups);

    void enableDebugClippingRect(bool flag);

protected:
    ID3D11Device* mDev;
    ID3D11DeviceContext* mCtx;

    class GuiViewport 
    {
    public:
        GuiViewport() : D3DRenderTarget(nullptr) { }

        ~GuiViewport();

        void create(HWND hwnd, const RenderTargetSpec& inSpec);

        void resize(HWND hwnd, const Vector2D& newSize);

    protected:
        friend class GuiDrawInterface;

        Matrix ProjMatrix;
        RenderTarget* D3DRenderTarget;
        Viewport D3DViewport;
    };

    std::unordered_map<HWND, GuiViewport> mHwndViewportMap;

    RenderBatchList mGuiRenderBatchList;
    GuiBatcher mBatcher;

    Sampler* mSampler;
    RenderStates* mRenderStates;

    GuiVertexShader mGuiVS;
    GuiPixelShaderDSL mGuiPS;
    bool mShowDebugClipping;

    Texture mBoxFilter;
    Texture mGaussFilter;
    Texture mBackupFilter;

    GeometryBuffer mVertexBuffer;
    GeometryBuffer mIndexBuffer;

    const uint32_t mInitialVertexBufferByteSize;
    const uint32_t mInitialIndexBufferByteSize;
};
#pragma once
#include "Renderer/Renderer.h"
#include "Renderer/VertexDeclaration.h"

class TestRenderer_2D
{
public:
    TestRenderer_2D();
    ~TestRenderer_2D();

    void initialize(HWND hwnd);
    void testDraw();

protected:
    void createTestData();

protected:
    HWND mHwnd;
    Renderer* mRenderer;
    ID3D11DeviceContext* mCtx;

    uint32_t mVBSize;
    uint32_t mIBSize;

    std::vector<GuiVertex> mVertexStream;
    std::vector<uint32_t> mIndexStream;

    Matrix mView, mProj;

    uint32_t mWidth, mHeight;

    class RenderStates* mRenderStates;

    class GeometryBuffer* mVertexBuffer;
    class GeometryBuffer* mIndexBuffer;

    class GuiDefaultVS* mVertexShader;
    class GuiDefaultPS* mPixelShader;

    class Viewport* mViewport;
    class RenderTarget* mRenderTarget;
};
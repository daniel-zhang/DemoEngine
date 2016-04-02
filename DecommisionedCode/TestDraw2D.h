#pragma once
#include "Renderer.h"
#include "VertexDeclaration.h"

class TestDraw2D
{
public:
    TestDraw2D();
    ~TestDraw2D();

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

    class VertexBuffer* mVertexBuffer;
    class IndexBuffer* mIndexBuffer;

    class GuiDefaultVS* mVertexShader;
    class GuiDefaultPS* mPixelShader;

    class Viewport* mViewport;
    class RenderTarget* mRenderTarget;
};
#pragma once

#include "Renderer.h"

#include "VertexBuffer.h"

#include "ConstBuffer.h"

#include "BaseVertexShader.h"
#include "GuiShaders.h"
#include "BasePixelShader.h"

#include "Viewport.h"
#include "RenderStates.h"
#include "RenderTarget.h"

#include "Material.h"

class TestRenderer
{
public:
    TestRenderer(); 

    ~TestRenderer();

    void initialize(HWND hwnd);

    void testDraw(float dt);

protected:
    void testTransformation();
    void createTransformations(float zPos);
    void createYellowTriangle();
    void createRedBox();
    void createHuman();

protected:
    float mViewWidth, mViewHeight;
    float mNearZ, mFarZ;
    Matrix mWorld, mView, mProjection;

    std::vector<BaseVertex> mVertexStream;
    std::vector<uint32_t> mIndexStream;

    Renderer* mRenderer;

    ID3D11DeviceContext* mCtx;

    // Overall
    RenderStates* mRenderStates;

    // IA, VS
    VertexBuffer* mVertexBuffer;
    IndexBuffer* mIndexBuffer;
    VertexShader* mVertexShader;

    // RS
    Viewport mViewport;

    // PS - TODO
    PixelShader* mPixelShader;
    
    // OM
    HWND mHwnd;
    RenderTarget* mRenderTarget;
};

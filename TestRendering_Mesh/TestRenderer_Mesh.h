#pragma once

// Reuse the d3d init code
#include "Renderer/Renderer.h"

// Render resources
#include "Renderer/GeometryBuffer.h"
#include "Renderer/VertexDeclaration.h"
#include "Renderer/ConstBuffer.h"
#include "Renderer/Viewport.h"
#include "Renderer/RenderStates.h"
#include "Renderer/RenderTarget.h"
#include "Renderer/Material.h"

// Shaders
#include "Renderer/GuiShaders.h"
#include "Renderer/BaseShaders.h"

class TestRenderer_Mesh
{
public:
    TestRenderer_Mesh();

    ~TestRenderer_Mesh();

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
    GeometryBuffer* mVertexBuffer;
    GeometryBuffer* mIndexBuffer;

    BaseVertexShader mBaseVS;

    // RS
    Viewport mViewport;

    // PS - TODO
    BasePixelShader mBasePS;

    // OM
    HWND mHwnd;
    RenderTarget* mRenderTarget;
};

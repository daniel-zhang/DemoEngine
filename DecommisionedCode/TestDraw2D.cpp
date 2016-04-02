#include "TestDraw2D.h"

#include "RenderStates.h"
#include "VertexBuffer.h"
#include "GuiShaders.h"
#include "Viewport.h"
#include "RenderTarget.h"

#include "ShaderLibrary.h"

TestDraw2D::TestDraw2D() : mCtx(nullptr),
    mRenderer(nullptr), mRenderStates(nullptr),
    mVertexBuffer(nullptr), mIndexBuffer(nullptr),
    mVertexShader(nullptr), mPixelShader(nullptr),
    mViewport(nullptr), mRenderTarget(nullptr),
    mVBSize(2048), mIBSize(4096)
{
    mViewport = new Viewport;
}

TestDraw2D::~TestDraw2D()
{
    safe_delete(mViewport);
    safe_delete(mVertexBuffer);
    safe_delete(mIndexBuffer);
}

void TestDraw2D::createTestData()
{
    {
        RECT clientArea;
        ::GetClientRect(mHwnd, &clientArea);
        mWidth = clientArea.right - clientArea.left;
        mHeight = clientArea.bottom - clientArea.top;
    }

    // Transformations
    mView = Matrix();
    //mView.fromXMMATRIX(DirectX::XMMatrixLookToLH( Vector3D::zero().toXMVECTOR(), Vector3D::unitZ().toXMVECTOR(), Vector3D::unitY().toXMVECTOR())); 

    mProj.makeOrthoProjLH((float)mWidth, (float)mHeight, .1f, 10.f);
    //mProj.fromXMMATRIX(DirectX::XMMatrixOrthographicLH((float)mWidth, (float)mHeight, .1f, 10.f));
    //mProj.fromXMMATRIX(DirectX::XMMatrixPerspectiveFovLH(0.25f * Math::PI, (float)mWidth / mHeight, 1, 100));

    // Streams
    mVertexStream.clear();
    mIndexStream.clear();

    float depth = 1;
    float width = 100;
    float height = 100;

    Vector3D topLeft     = Vector3D(10,                  10,                  depth);
    Vector3D topRight    = Vector3D(topLeft.x + width,  topLeft.y,          depth);
    Vector3D bottomLeft  = Vector3D(topLeft.x,          topLeft.y + height, depth);
    Vector3D bottomRight = Vector3D(topLeft.x + width,  topLeft.y + height, depth);

    Color color = Palette::Red;

    mVertexStream.push_back(GuiVertex(topLeft,      color, Vector2D(0.0f, 0.0f)));
    mVertexStream.push_back(GuiVertex(topRight,     color, Vector2D(1.0f, 0.0f)));
    mVertexStream.push_back(GuiVertex(bottomRight,  color, Vector2D(1.0f, 1.0f)));
    mVertexStream.push_back(GuiVertex(bottomLeft,   color, Vector2D(0.0f, 1.0f)));

    // Font face - clockwise
    mIndexStream.push_back(2);
    mIndexStream.push_back(0);
    mIndexStream.push_back(1);

    mIndexStream.push_back(2);
    mIndexStream.push_back(3);
    mIndexStream.push_back(0);
}

void TestDraw2D::initialize(HWND hwnd)
{
    mRenderer = Renderer::getInstance();
    mRenderer->initialize(RendererSettings());

    mCtx = mRenderer->getContext();
    mHwnd = hwnd;

    mRenderStates = RenderStates::getInstance();
    mRenderStates->create();

    createTestData();

    //
    // IA
    //

    // VB
    {
        mVertexBuffer = new  VertexBuffer(mVBSize);
        mVertexBuffer->create();

        memcpy(mVertexBuffer->lock(), &mVertexStream[0], mVertexStream.size() * sizeof GuiVertex);
        mVertexBuffer->unlock();

        uint32_t stride = sizeof GuiVertex;
        uint32_t offset = 0;
        mCtx->IASetVertexBuffers(0, 1, mVertexBuffer->get2(), &stride, &offset);
    }

    // IB
    {
        mIndexBuffer = new IndexBuffer(mIBSize);
        mIndexBuffer->create();
        
        memcpy(mIndexBuffer->lock(), &mIndexStream[0], mIndexStream.size() * sizeof uint32_t);
        mIndexBuffer->unlock();

        mCtx->IASetIndexBuffer(mIndexBuffer->get(), DXGI_FORMAT_R32_UINT, 0);
    }

    mRenderStates->setTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
   
    //
    // VS, PS
    //
    mVertexShader = ShaderLibrary::getInstance()->getShader_RTTI<GuiDefaultVS>();
    mPixelShader = ShaderLibrary::getInstance()->getShader_RTTI<GuiDefaultPS>();

    //
    // RS
    //
    mViewport->create();
    D3D11_VIEWPORT& vp = mViewport->get();
    vp.Width    = (float)mWidth;
    vp.Height   = (float)mHeight;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    vp.MinDepth = 0.f;
    vp.MaxDepth = 1.f;
    mViewport->bind();

    // Debug: disable back face culling
    mCtx->RSSetState(mRenderStates->RasterizerState_Normal);
    mRenderStates->disableDepthTest();

    //
    // OM
    //
    RenderTargetSpec spec;
    mRenderTarget = new RenderTarget(hwnd, spec);
    mRenderTarget->create();
    mRenderTarget->bind();
}

void TestDraw2D::testDraw()
{
    mRenderTarget->clear(Palette::LightBlue);

    /*
    mVertexShader->setView(mView);
    mVertexShader->setProjection(mProj);
    */
    mVertexShader->setParameter(ECbufferSemantics::ViewMatrix, mView);
    mVertexShader->setParameter(ECbufferSemantics::ProjMatrix, mProj);

    std::string viewStr = mView.toDebugString();
    std::string projStr = mProj.toDebugString();

    mVertexShader->apply();
    mPixelShader->apply();

    //IndexCount, StartIndexLocation, BaseVertexLocation
    mCtx->DrawIndexed(mIndexStream.size(), 0, 0);

    mRenderTarget->present();
}

#include "TestRenderer_Mesh.h"

#include "Core/FbxLoader.h"
#include "Core/Math.h"

TestRenderer_Mesh::TestRenderer_Mesh() :
mCtx(nullptr),
mRenderer(nullptr),
mRenderTarget(nullptr),
mVertexBuffer(nullptr),
mIndexBuffer(nullptr)
{

}

TestRenderer_Mesh::~TestRenderer_Mesh()
{
    if (mRenderTarget)
    {
        delete mRenderTarget;
        mRenderTarget = nullptr;
    }

    if (mVertexBuffer)
    {
        delete mVertexBuffer;
        mVertexBuffer = nullptr;
    }

    if (mIndexBuffer)
    {
        delete mIndexBuffer;
        mIndexBuffer = nullptr;
    }
}

void TestRenderer_Mesh::testTransformation()
{
    Vector3D translation(3.f, 10.f, 2.f);
    Vector3D rotation(1.f, 0.5f, 0.2f);
    Vector3D scaling(1.f, 1.f, 1.f);

    DirectX::XMMATRIX transformation_1 = DirectX::XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z);
    transformation_1.r[0] = DirectX::XMVectorScale(transformation_1.r[0], scaling.x);
    transformation_1.r[1] = DirectX::XMVectorScale(transformation_1.r[1], scaling.y);
    transformation_1.r[2] = DirectX::XMVectorScale(transformation_1.r[2], scaling.z);
    transformation_1.r[3] = DirectX::XMVectorSetW(DirectX::XMLoadFloat3(&translation), 1.f);

    Matrix matrix_1;
    matrix_1.fromXMMATRIX(transformation_1);

    DirectX::XMMATRIX transformation_2 = DirectX::XMMatrixAffineTransformation(
        scaling.toXMVECTOR(),
        Vector3D::zero().toXMVECTOR(),
        DirectX::XMQuaternionRotationRollPitchYaw(rotation.x, rotation.y, rotation.z), // (pitch, yaw, roll) - (right, up, in) - (x, y, z)
        translation.toXMVECTOR());

    Matrix matrix_2;
    matrix_2.fromXMMATRIX(transformation_2);
}


void TestRenderer_Mesh::createTransformations(float zPos)
{
    float quaterPi = Math::HALF_PI / 2;

    Vector4D objPos(0, 0, zPos, 1);
    //Vector3D objRotation(quaterPi, quaterPi, quaterPi);
    Vector3D objRotation(0, 0, 0);
    Vector3D objScaling(1.f, 1.f, 1.f);

    mViewWidth = 400.f;
    mViewHeight = 300.f;
    mNearZ = 1.f;
    mFarZ = 1000.f;

    mWorld.fromXMMATRIX(DirectX::XMMatrixAffineTransformation(
        objScaling.toXMVECTOR(),
        Vector3D::zero().toXMVECTOR(),
        DirectX::XMQuaternionRotationRollPitchYaw(objRotation.x, objRotation.y, objRotation.z), // (pitch, yaw, roll) - (right, up, in) - (x, y, z)
        objPos.toXMVECTOR()
        ));

    mView.fromXMMATRIX(DirectX::XMMatrixLookToLH(
        Vector3D::zero().toXMVECTOR(), // EyePosition
        Vector3D::unitZ().toXMVECTOR(), // EyeDirection
        Vector3D::unitY().toXMVECTOR())); // UpDirection

    //gProjection.fromXMMATRIX( DirectX::XMMatrixPerspectiveLH(gViewWidth, gViewHeight, gNearZ, gFarZ) );
    mProjection.fromXMMATRIX(DirectX::XMMatrixPerspectiveFovLH(0.25f * Math::PI, 4.f / 3.f, mNearZ, mFarZ));
}

void TestRenderer_Mesh::createYellowTriangle()
{
    createTransformations(3);
    BaseVertex v0, v1, v2, v3;

    v0.Position = Vector4D(0, 100, 0, 1);
    v1.Position = Vector4D(100, 0, 0, 1);
    v2.Position = Vector4D(-100, 0, 0, 1);
    v3.Position = Vector4D(0, -100, 0, 1);

    v0.Color = Palette::Yellow;
    v1.Color = Palette::Yellow;
    v2.Color = Palette::Yellow;
    v3.Color = Palette::Blue;

    mVertexStream.push_back(v0);
    mVertexStream.push_back(v1);
    mVertexStream.push_back(v2);
    mVertexStream.push_back(v3);

    mIndexStream.push_back(0);
    mIndexStream.push_back(1);
    mIndexStream.push_back(2);

    mIndexStream.push_back(1);
    mIndexStream.push_back(3);
    mIndexStream.push_back(2);
}

void TestRenderer_Mesh::createRedBox()
{
    createTransformations(0);
    float w = 60;
    BaseVertex v0, v1, v2, v3, v4, v5, v6, v7;
    v0.Position = Vector4D(w, w, -w, 1);
    v1.Position = Vector4D(w, -w, -w, 1);
    v2.Position = Vector4D(-w, -w, -w, 1);
    v3.Position = Vector4D(-w, w, -w, 1);
    v4.Position = Vector4D(w, w, w, 1);
    v5.Position = Vector4D(w, -w, w, 1);
    v6.Position = Vector4D(-w, -w, w, 1);
    v7.Position = Vector4D(-w, w, w, 1);

    v0.Color = Palette::Red;
    v1.Color = Palette::Green;
    v2.Color = Palette::Yellow;
    v3.Color = Palette::Blue;
    v4.Color = Palette::White;
    v5.Color = Palette::Gray;
    v6.Color = Palette::Purple;
    v7.Color = Palette::Orange;

    mVertexStream.push_back(v0);
    mVertexStream.push_back(v1);
    mVertexStream.push_back(v2);
    mVertexStream.push_back(v3);
    mVertexStream.push_back(v4);
    mVertexStream.push_back(v5);
    mVertexStream.push_back(v6);
    mVertexStream.push_back(v7);

    uint32_t index[] = {
        0, 1, 2, 0, 2, 3,
        0, 4, 5, 0, 5, 1,
        4, 7, 6, 4, 6, 5,
        7, 3, 2, 7, 2, 6,
        7, 4, 0, 7, 0, 3,
        2, 1, 5, 2, 5, 6
    };
    for (uint32_t i = 0; i < 36; ++i)
    {
        mIndexStream.push_back(index[i]);
    }

}

void TestRenderer_Mesh::createHuman()
{
    createTransformations(0);

    FbxLoader::getInstance()->initialize();

    FbxInput input;
    input.FilePath = ws2s(FileOp::getDefaultPath<FileOp::MeshResource>().toString());
    input.Filename = "opening_a_filing_cabinet_1.fbx";
    input.RemoveDuplicatedVertex = true;

    FbxOutput output;

    FbxLoader::getInstance()->load(input, output);
    check(output.Errors.empty());

    // Extract what we want from output
    FbxHelper::Scene* helperScene = output.OutputScene;

    check(helperScene->getMeshNum() > 0);
    FbxHelper::Mesh& helperMesh = helperScene->getMesh(3);
    FbxHelper::VertexStream& helperStream = helperMesh.getVertexStream();

    helperStream.negateZ();
    helperStream.reverseWindingOrder();

    const std::vector<FbxHelper::Vertex>& helperVertexData = helperStream.getVertexData();

    // Pick what we need from helperVertexData
    mVertexStream.clear();
    mVertexStream.reserve(helperVertexData.size());
    for (auto helperVertex : helperVertexData)
    {
        const Vector4D& inPos = helperVertex.getPosition();
        const Vector4D& inNormal = helperVertex.getNormal();
        const Vector2D& inUV = helperVertex.getUV();

        BaseVertex baseVertex;

        baseVertex.Position.x = inPos.x;
        baseVertex.Position.y = inPos.y;
        baseVertex.Position.z = inPos.z;
        baseVertex.Position.w = 1;

        baseVertex.Normal.x = inNormal.x;
        baseVertex.Normal.y = inNormal.y;
        baseVertex.Normal.z = inNormal.z;

        baseVertex.Texcoord = inUV;

        baseVertex.Color = Palette::Yellow;

        mVertexStream.push_back(baseVertex);
    }

    // Generate index data
    helperStream.generateIndexData(mIndexStream);
}

void TestRenderer_Mesh::initialize(HWND hwnd)
{
    // Init renderer
    mRenderer = Renderer::getInstance();
    mRenderer->initialize(RendererSettings());

    mCtx = mRenderer->getContext();
    mHwnd = hwnd;

    /************************************************************************/
    // IA, VS
    /************************************************************************/
    createYellowTriangle();
    //createRedBox();
    //createHuman();

    mRenderStates = RenderStates::getInstance();
    mRenderStates->create();
    mRenderStates->setTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    // VB
    uint32_t vertexDataSize = mVertexStream.size() * sizeof BaseVertex;

    mVertexBuffer = new  GeometryBuffer();
    mVertexBuffer->initialize(GeometryBuffer::DynamicVertexBuffer, vertexDataSize);
    memcpy(mVertexBuffer->lock(), &mVertexStream[0], vertexDataSize);
    mVertexBuffer->unlock();

    uint32_t stride = sizeof BaseVertex;
    uint32_t offset = 0;
    mCtx->IASetVertexBuffers(0, 1, mVertexBuffer->getResourcePtr(), &stride, &offset);

    // IB
    uint32_t indexDataSize = mIndexStream.size() * sizeof uint32_t;

    mIndexBuffer = new GeometryBuffer();
    mIndexBuffer->initialize(GeometryBuffer::DynamicIndexBuffer, indexDataSize);
    memcpy(mIndexBuffer->lock(), &mIndexStream[0], indexDataSize);
    mIndexBuffer->unlock();

    mCtx->IASetIndexBuffer(mIndexBuffer->getResource(), DXGI_FORMAT_R32_UINT, 0);

    // VS -- create a base vertex shader
    mBaseVS.initialize();


    /************************************************************************/
    // RS
    /************************************************************************/
    mViewport.create();
    D3D11_VIEWPORT& vp = mViewport.get();
    vp.Width = mViewWidth;
    vp.Height = mViewHeight;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    vp.MinDepth = 0.f;
    vp.MaxDepth = 1.f;
    mViewport.bind();

    // Debug: disable back face culling
    mCtx->RSSetState(mRenderStates->RasterizerState_Normal);

    /************************************************************************/
    // PS
    /************************************************************************/
    mBasePS.initialize();

    /************************************************************************/
    // OM
    /************************************************************************/
    RenderTargetSpec spec;
    mRenderTarget = new RenderTarget(hwnd, spec);
    mRenderTarget->create();
    mRenderTarget->bind();
}

void TestRenderer_Mesh::testDraw(float dt)
{
    static float anim = 0;
    if (anim > 10)
    {
        anim = 0;
    }
    anim += dt;

    createTransformations(anim * 100);

    mRenderTarget->clear();

    mBaseVS.setParameter(BaseVertexShader::ECBufferParam::WorldMatrix, mWorld);
    mBaseVS.setParameter(BaseVertexShader::ECBufferParam::ViewMatrix, mView);
    mBaseVS.setParameter(BaseVertexShader::ECBufferParam::ProjMatrix, mProjection);

    std::string worldStr = mWorld.toDebugString();
    std::string viewStr = mView.toDebugString();
    std::string projStr = mProjection.toDebugString();

    mBaseVS.apply();
    mBasePS.apply();

    //IndexCount, StartIndexLocation, BaseVertexLocation
    mCtx->DrawIndexed(mIndexStream.size(), 0, 0);
    mRenderTarget->present();
}

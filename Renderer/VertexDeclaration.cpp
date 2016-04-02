#include "VertexDeclaration.h"
#include "Renderer.h"


VertexDeclaration::VertexDeclaration() :
    mpByteCode(nullptr),
    mByteCodeSize(0),
    mInputLayout(nullptr),
    mDev(nullptr), mCtx(nullptr)
{
}

VertexDeclaration::~VertexDeclaration()
{
    destroy();
}

void VertexDeclaration::destroy()
{
    // Release d3d resource
    safe_release(mInputLayout);

    // Clear local states
    mpByteCode = nullptr;
    mByteCodeSize = 0;
    mDev = nullptr;
    mCtx = nullptr;
}

void BaseVertexDecl::create()
{
    if (mInputLayout) return;

    check(mpByteCode != nullptr && mByteCodeSize != 0);

    mCtx = Renderer::getInstance()->getContext();
    mDev = Renderer::getInstance()->getDevice();

    static const D3D11_INPUT_ELEMENT_DESC desc[] =
    {
        // SemanticName;SemanticIndex;Format;InputSlot;AlignedByteOffset;InputSlotClass;InstanceDataStepRate;
        { "POSITION",   0, DXGI_FORMAT_R32G32B32A32_FLOAT,  0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "NORMAL",     0, DXGI_FORMAT_R32G32B32_FLOAT,     0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD",   0, DXGI_FORMAT_R32G32_FLOAT,        0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TANGENT",    0, DXGI_FORMAT_R32G32B32_FLOAT,     0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR",      0, DXGI_FORMAT_R32G32B32A32_FLOAT,  0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
    };

    HRESULT hr = mDev->CreateInputLayout( desc, sizeof desc / sizeof D3D11_INPUT_ELEMENT_DESC, mpByteCode, mByteCodeSize, &mInputLayout);
    check_hr(hr);
}

uint32_t BaseVertexDecl::defineStride() const
{
    return sizeof BaseVertex;
}

void GuiVertexDecl::create()
{
    if (mInputLayout) return;

    check(mpByteCode != nullptr && mByteCodeSize != 0);

    mCtx = Renderer::getInstance()->getContext();
    mDev = Renderer::getInstance()->getDevice();

    static const D3D11_INPUT_ELEMENT_DESC desc[] =
    {
        // SemanticName;SemanticIndex;Format;InputSlot;AlignedByteOffset;InputSlotClass;InstanceDataStepRate;
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "CLIPRECT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
    };

    HRESULT hr = mDev->CreateInputLayout(desc, sizeof desc / sizeof D3D11_INPUT_ELEMENT_DESC, mpByteCode, mByteCodeSize, &mInputLayout);
    check_hr(hr);
}

uint32_t GuiVertexDecl::defineStride() const
{
    return sizeof GuiVertex;
}

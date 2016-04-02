#include "RenderStates.h"
#include "Renderer.h"

RenderStates::RenderStates() :
    mDev(nullptr),
    mCtx(nullptr),

    BlendState_On(nullptr),
    BlendState_Off(nullptr),

    DepthStencilState_On(nullptr),
    DepthStencilState_Off(nullptr),

    RasterizerState_Normal(nullptr),
    RasterizerState_NoMSAA(nullptr),
    RasterizerState_Scissor(nullptr),
    RasterizerState_Wireframe(nullptr)
{ }

RenderStates::~RenderStates()
{
    destroy();
}

void RenderStates::create()
{
    if (mDev || mCtx) return;

    mDev = Renderer::getInstance()->getDevice();
    mCtx = Renderer::getInstance()->getContext();

    //
    // Blend States
    //
    D3D11_BLEND_DESC blendDesc;

    blendDesc.AlphaToCoverageEnable = false;
    blendDesc.IndependentBlendEnable = false;
    blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
    blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA;
    blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
    blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
    blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
    blendDesc.RenderTarget[0].BlendEnable = true;
    mDev->CreateBlendState(&blendDesc, &BlendState_On);

    blendDesc.RenderTarget[0].BlendEnable = false;
    mDev->CreateBlendState(&blendDesc, &BlendState_Off);

    //
    // Depth Stencil States
    //
    D3D11_DEPTH_STENCIL_DESC dsDesc;
    // This is the default value, see https://msdn.microsoft.com/en-us/library/windows/desktop/ff476110(v=vs.85).aspx
    dsDesc.DepthEnable                  = true;
    dsDesc.DepthWriteMask               = D3D11_DEPTH_WRITE_MASK_ALL;
    dsDesc.DepthFunc                    = D3D11_COMPARISON_LESS;
    dsDesc.StencilEnable                = false;
    dsDesc.StencilReadMask              = D3D11_DEFAULT_STENCIL_READ_MASK;
    dsDesc.StencilWriteMask             = D3D11_DEFAULT_STENCIL_WRITE_MASK;
    dsDesc.FrontFace.StencilFunc        = D3D11_COMPARISON_ALWAYS;
    dsDesc.BackFace.StencilFunc         = D3D11_COMPARISON_ALWAYS;
    dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
    dsDesc.BackFace.StencilDepthFailOp  = D3D11_STENCIL_OP_KEEP;
    dsDesc.FrontFace.StencilPassOp      = D3D11_STENCIL_OP_KEEP;
    dsDesc.BackFace.StencilPassOp       = D3D11_STENCIL_OP_KEEP;
    dsDesc.FrontFace.StencilFailOp      = D3D11_STENCIL_OP_KEEP;
    dsDesc.BackFace.StencilFailOp       = D3D11_STENCIL_OP_KEEP;

    mDev->CreateDepthStencilState(&dsDesc, &DepthStencilState_On);

    dsDesc.DepthEnable = false;
    mDev->CreateDepthStencilState(&dsDesc, &DepthStencilState_Off);

    //
    // Rasterizer States
    //
    D3D11_RASTERIZER_DESC rasterizerDesc;

    rasterizerDesc.CullMode = D3D11_CULL_NONE; // Always draw all triangles.
    rasterizerDesc.FillMode = D3D11_FILL_SOLID;
    rasterizerDesc.FrontCounterClockwise = false;
    rasterizerDesc.DepthBias = 0;
    rasterizerDesc.DepthBiasClamp = 0;
    rasterizerDesc.SlopeScaledDepthBias = 0;
    rasterizerDesc.ScissorEnable = false;
    rasterizerDesc.MultisampleEnable = true;
    rasterizerDesc.AntialiasedLineEnable = false;
    check_hr( mDev->CreateRasterizerState(&rasterizerDesc, &RasterizerState_Normal) );

    rasterizerDesc.MultisampleEnable = false;
    mDev->CreateRasterizerState(&rasterizerDesc, &RasterizerState_NoMSAA);

    rasterizerDesc.MultisampleEnable = true;
    rasterizerDesc.ScissorEnable = true;
    mDev->CreateRasterizerState(&rasterizerDesc, &RasterizerState_Scissor);

    rasterizerDesc.AntialiasedLineEnable = false;
    rasterizerDesc.ScissorEnable = false;
    rasterizerDesc.FillMode = D3D11_FILL_WIREFRAME;
    mDev->CreateRasterizerState(&rasterizerDesc, &RasterizerState_Wireframe);
}

void RenderStates::destroy()
{
    safe_release(BlendState_On);
    safe_release(BlendState_Off);

    safe_release(DepthStencilState_On);
    safe_release(DepthStencilState_Off);

    safe_release(RasterizerState_Normal);
    safe_release(RasterizerState_NoMSAA);
    safe_release(RasterizerState_Scissor);
    safe_release(RasterizerState_Wireframe);

    mDev = nullptr;
    mCtx = nullptr;
}

void RenderStates::recreate()
{
    destroy();
    create();
}

void RenderStates::disableDepthTest()
{
    mCtx->OMSetDepthStencilState(DepthStencilState_Off, 0x01);
}

void RenderStates::enableDepthTest()
{
    mCtx->OMSetDepthStencilState(DepthStencilState_On, 0x01);
}

void RenderStates::setTopology(D3D_PRIMITIVE_TOPOLOGY topology)
{
    mCtx->IASetPrimitiveTopology(topology);
}

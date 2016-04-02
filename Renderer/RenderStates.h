#pragma once

#include "RendererCommon.h"

class RenderStates : public IRenderResource
{
public:
    // Make singleton
    static RenderStates* getInstance()
    {
        static RenderStates rs;
        return &rs;
    }

public:
    ~RenderStates();

    virtual const wchar_t* getResourceName() override { return L"RenderStates"; }

    virtual void create() override;

    virtual void destroy() override;

    virtual void recreate() override;

    bool isValid() { return mDev != nullptr; }

public:
    // TODO: 
    // 1. State changes should only be performed by calling these wrappers
    // 2. Keep track of pipeline states via the wrappers, so it's possible to do a redundancy check before commit
    void disableDepthTest();

    void enableDepthTest();

    void setTopology(D3D_PRIMITIVE_TOPOLOGY topology);

public:
    ID3D11BlendState* BlendState_On;
    ID3D11BlendState* BlendState_Off;

    ID3D11DepthStencilState* DepthStencilState_On;
    ID3D11DepthStencilState* DepthStencilState_Off;

    ID3D11RasterizerState* RasterizerState_Normal;
    ID3D11RasterizerState* RasterizerState_NoMSAA; // For font rendering
    ID3D11RasterizerState* RasterizerState_Scissor;
    ID3D11RasterizerState* RasterizerState_Wireframe;

protected:
    RenderStates();

protected:
    ID3D11Device* mDev;
    ID3D11DeviceContext* mCtx;
};
#pragma once

#include "RendererCommon.h"

class Viewport : public IRenderResource
{
public:
    virtual const wchar_t* getResourceName() override { return L"Viewport"; }

    virtual void create() override;

    virtual void destroy() override
    {
        mCtx = nullptr;
    }

    virtual void recreate() override
    {
    }

    D3D11_VIEWPORT& get()
    {
        return mViewport;
    }

    void bind()
    {
        check(mCtx);
        /*
        Which viewport to use is determined by the SV_ViewportArrayIndex semantic output by a geometry shader 
        (see shader semantic syntax). If a geometry shader does not make use of the SV_ViewportArrayIndex 
        semantic then Direct3D will use the first viewport in the array.
        */
        mCtx->RSSetViewports(1, &mViewport);
    }

public:
    Viewport() :mCtx(nullptr)
    {}
    ~Viewport()
    {
        destroy();
    }

protected:
    ID3D11DeviceContext* mCtx;
    D3D11_VIEWPORT mViewport;
};
#pragma once
//#include "RendererCommon.h"

/*
Interface for everything that is involved in the rendering pipeline. 

This interface describe them from the perspective of resource life cycle management, and 
does NOT model how resources interact with each other.

Resource organization is implicitly constructed through the code. Here's a brief memo:
IA+VS
    VertexDeclaration, VertexBuffer, IndexBuffer
    ConstantBuffer,
    VertexShader
RS
    Viewport, RenderStates
PS
    ConstantBuffer
    Texture Sampler
    PixelShader
OM
    RenderTarget(a dedicated class for swap chain, and a Texture for off-screen scenario)
*/
class IRenderResource
{
public:
    virtual ~IRenderResource()
    {
    }

    virtual void create()
    {
    }

    virtual void destroy()
    {
    }

    virtual void recreate()
    {
    }

    virtual const wchar_t* getResourceName() = 0;
};
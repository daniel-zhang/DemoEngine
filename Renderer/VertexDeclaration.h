#pragma once

#include "RendererCommon.h"
/*
Singleton: 
    Only one instance of any given concrete VertexDeclaration class exists throughout the system

Purpose:
    Used as a vertex type identifier when Material tries to find a proper vertex shader for a given Mesh
    Describes the vertex format 
    Functions as a render resource(d3d input layout) which is bound to pipeline at proper time
*/
class VertexDeclaration : public IRenderResource
{
public:
    VertexDeclaration();
    virtual ~VertexDeclaration();

    virtual void destroy() override;
    virtual void recreate() override { /* Do not allow this*/ }

    // D3D requires to check shader signature when creating input layout. Must set this before creation.
    inline void setVertexShaderSignature(const void* code, uint32_t size) { mpByteCode = code; mByteCodeSize = size; }
    inline ID3D11InputLayout* getInputLayout() { return mInputLayout; }
    inline uint32_t getStride(){ return defineStride(); }
    inline void bind() { mCtx->IASetInputLayout(mInputLayout);}

protected:
    virtual uint32_t defineStride() const = 0;

protected:
    const void* mpByteCode;
    uint32_t mByteCodeSize;

    ID3D11Device* mDev;
    ID3D11DeviceContext* mCtx;
    ID3D11InputLayout* mInputLayout;
};

struct BaseVertex 
{
    Vector4D Position;
    Vector3D Normal;
    Vector2D Texcoord;
    Vector3D Tangent;
    Color    Color;
};

class BaseVertexDecl : public VertexDeclaration
{
private:
    BaseVertexDecl(){}
    BaseVertexDecl(const BaseVertexDecl&);
    BaseVertexDecl& operator=(const BaseVertexDecl&);

public:
    static BaseVertexDecl* getInstance() { static BaseVertexDecl instance; return &instance; }
    const wchar_t* getResourceName() override { return L"BaseVertexDecl"; }

    virtual void create() override;

protected:
    uint32_t defineStride() const override;
};

struct GuiVertex
{
    GuiVertex(const Vector3D& inPos, const Color& inColor, const Vector2D& inUV) : 
        Position(inPos), TexCoords(inUV.x, inUV.y, 0.f, 0.f), Color(inColor) 
    {}

    GuiVertex(const Vector2D& inPos, const Color& inColor, const Vector2D& inUV) : 
        Position(inPos.x, inPos.y, 1.f), TexCoords(inUV.x, inUV.y, 0.f, 0.f), Color(inColor) 
    {}

    GuiVertex(const Vector3D& inPos, const Vector2D& inUV, const Vector2D& inTiling, const Color& inColor, const Vector4D inClipRect) :
        Position(inPos), TexCoords(inUV.x, inUV.y, inTiling.x, inTiling.y), Color(inColor), ClipRect(inClipRect)
    {}

    GuiVertex(const Vector2D& inPos, const Vector2D& inUV, const Vector2D& inTiling, const Color& inColor, const Vector4D inClipRect) :
        Position(inPos.x, inPos.y, 1.f), TexCoords(inUV.x, inUV.y, inTiling.x, inTiling.y), Color(inColor), ClipRect(inClipRect)
    {}

    Vector3D Position;
    Vector4D TexCoords;
    Vector4D ClipRect; // Left, top, right , bottom
    Color Color;
};

class GuiVertexDecl: public VertexDeclaration
{
private:
    GuiVertexDecl(){}
    GuiVertexDecl(const GuiVertexDecl&);
    GuiVertexDecl& operator=(const GuiVertexDecl&);

public:
    static GuiVertexDecl* getInstance() { static GuiVertexDecl instance; return &instance; }

    const wchar_t* getResourceName() override { return L"GuiVertexDecl"; }

    virtual void create() override;

protected:
    uint32_t defineStride() const override;
};
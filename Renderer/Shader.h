#pragma once

#include "RendererCommon.h"
#include "VertexDeclaration.h"
#include "ConstBuffer.h"
//#include "Texture.h"
#include "Texture.h"
#include "ShaderParameter.h"

// Implement RTTI on our own
// They say virtual call can be 10 times faster than dynamic_cast
// Yes shader creation is rarely a bottleneck, I am doing this just for fun
class UniqueID
{
public:
    UniqueID()
    {
        // A simple GUID scheme 
        static uint32_t dummyGUID = 0;
        mDummySerialNumber = dummyGUID++;
    }
    bool operator==(const UniqueID& rhs) const
    {
        return (this->mDummySerialNumber == rhs.mDummySerialNumber);
    }

protected:
    uint32_t mDummySerialNumber;
};

#define DECL_SHADER() public:\
    static const UniqueID& id()\
    {\
        static UniqueID uId;\
        return uId;\
    }\
    virtual const UniqueID& getUID() override\
    {\
        return this->id();\
    }

// This exists to provide a class id interface for handcrafted RTII
// TODO: move parameter and compiling code here
class IShader 
{
public:
    virtual const UniqueID& getUID() = 0; // Do not implement this manually. 

    virtual ~IShader(){}

    virtual void initialize() = 0;

    virtual void destroy() = 0;

    virtual void apply() = 0;
};

class VertexShader : public IShader
{
public:
    virtual VertexDeclaration* getVertexDeclaration() = 0;
};

class PixelShader : public IShader
{
public:

};

/*
// Base for all vertex shaders
class VertexShader : public Shader
{
public:
    VertexShader(): 
        mDev(nullptr), mCtx(nullptr), 
        mVertexShader(nullptr), 
        mVertexDecl(nullptr)
    {}
    virtual ~VertexShader(){ destroy(); }

public:
    // RenderResource interface
    virtual void create() override;

    virtual void destroy() override;

    virtual void recreate() override;

    // Required by material to select a VertexShader for a given mesh
    VertexDeclaration* getVertexDeclaration(){ return mVertexDecl; }

    // Bind vertex shader object and all related shader resources to the pipeline
    // Any changes made to cbuffers are also committed at this point.
    void apply();

    template<typename T>
    void setParameter(ECbufferSemantics semantic, const T& inVal)
    {
        mCBufferParamGroup.set((uint32_t)semantic, inVal);
    }

    inline void setParameter(ETexture2DSemantics semantic, Texture* inTexture)
    {
        mTextureParamGroup.set((uint32_t)semantic, inTexture);
    }

    inline void setParameter(ESamplerSemantics semantic, ID3D11SamplerState* inSampler) 
    { 
        mSamplerParamGroup.set((uint32_t)semantic, inSampler); 
    }

protected:
    virtual const char* sourceFilename() const = 0;

    virtual const char* entrypointName() const = 0;

    virtual void createVertexDeclaration(const void* pCode, const uint32_t bytesize) = 0;

protected:
    ID3D11Device* mDev;
    ID3D11DeviceContext* mCtx;
    ShaderParamMap mParamMap;
    CbufferParameterGroup mCBufferParamGroup;
    SamplerParameterGroup mSamplerParamGroup;
    TextureParameterGroup mTextureParamGroup;

    VertexDeclaration* mVertexDecl;
    ID3D11VertexShader* mVertexShader;
};

// Base for all pixel shaders
class PixelShader : public Shader
{
public:
    PixelShader() :mDev(nullptr), mCtx(nullptr), mPixelShader(nullptr), mClassLinkage(nullptr){}

    virtual ~PixelShader()
    {
        destroy();
    }

public:
    // RenderResource interface
    void create() override;

    void destroy() override;

    void recreate() override;

    // Bind this shader and all its shader resources(cbuffers for now) to the render pipeline
    // Also commit all changes 
    void apply();

    template<typename T>
    void setParameter(ECbufferSemantics semantic, const T& inVal)
    {
        mCBufferParamGroup.set((uint32_t)semantic, inVal);
    }

    inline void setParameter(ETexture2DSemantics semantic, Texture* inTexture)
    {
        mTextureParamGroup.set((uint32_t)semantic, inTexture);
    }

    inline void setParameter(ESamplerSemantics semantic, ID3D11SamplerState* inSampler)
    {
        mSamplerParamGroup.set((uint32_t)semantic, inSampler);
    }

protected:
    virtual const char* sourceFilename() const = 0;

    virtual const char* entrypointName() const = 0;

protected: 
    ID3D11Device* mDev;

    ID3D11DeviceContext* mCtx;

    ShaderParamMap mParamMap;

    CbufferParameterGroup mCBufferParamGroup;

    SamplerParameterGroup mSamplerParamGroup;

    TextureParameterGroup mTextureParamGroup;

    ID3D11PixelShader* mPixelShader;

    ID3D11ClassLinkage* mClassLinkage;

    std::vector<ID3D11ClassInstance*> mInterfacePtrs;
};

*/

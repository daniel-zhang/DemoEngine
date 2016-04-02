#pragma once

#include "RendererCommon.h"
#include "ConstBuffer.h"
#include "Texture.h"
#include "Sampler.h"

/*
# Memo
## Design

A parameter is defined as a wrapper object for a particular type of render resource(cbuffer/texture/sampler), along with the 
information about how that resource is updated and bound to pipeline.

Ideally a shader and its parameters would only hold references to the underlying resources, instead of taking ownership of them. 
This allows for different shaders sharing the same resources rather than duplicating them.

An exception is how cbuffer is handled. Each shader object creates and manages an independent set of cbuffers. This is reasonable 
because it gives the shader author(me) the maximum freedom about how cbuffers are constructed.

## General flow of parameter initialization
    - a shader creates a const array of all its allowed parameters, indexed by semantic enum
    - it then matches the parameter array with incoming shader reflection data 
    - if a given semantic is found in the reflection, bind its corresponding allocation layout to that parameter
    - the resulting array is not necessarily fully bound. Setting unbound parameter is simply ignored
*/
class CbufferParameter
{
public:
    CbufferParameter();

    ~CbufferParameter();

    void initialize(uint32_t byteOffset, uint32_t byteSize, ConstBuffer* cbufferRef);

    void destroy();

    bool isInitialized() const { return mCBufferRef != nullptr; }

    template<typename T>
    void set(const T& val)
    {
        check(sizeof T == mByteSize);

        if (mCBufferRef) 
        {
            mCBufferRef->update(mByteOffset, (uint8_t*)&val, mByteSize);
        }
    }

protected:
    ConstBuffer* mCBufferRef;

    // The parameter's byte offset into the cbuffer
    uint32_t mByteOffset;

    // The parameter's data size in bytes
    uint32_t mByteSize;
};

/*
*/
class CbufferParameterGroup
{
public:
    CbufferParameterGroup(){}

    ~CbufferParameterGroup(){}
        
    // Template version to remove the dependency on how semantics are declared.
    template<typename TParamDecl>
    void initialize_t(ShaderParamMap& inParamMap)
    {
        // Create ConstBuffers with their binding slot by parsing shader reflection data
        const std::vector<CBufferDesc>& cbufferDescs = inParamMap.getCBufferDescs();
        mBindings.resize(cbufferDescs.size());

        for (uint32_t i = 0; i < cbufferDescs.size(); ++i)
        {
            mBindings[i].Slot = cbufferDescs[i].BufferIndex;
            mBindings[i].CBuffer.initialize(cbufferDescs[i].BufferSize);
        }

        // Preallocate uninitialized parameters
        mParams.resize((uint32_t)TParamDecl::TotalNum);

        // Initialize parameters if they are defined in ShaderParamMap
        for (uint32_t semantic = 0; semantic < (uint32_t)TParamDecl::TotalNum; ++semantic)
        {
            // Translate semantic into shader variable name string
            std::string name = TParamDecl::str[semantic];

            ShaderParamAllocation allocation;
            if (inParamMap.bindParamByName(name, allocation))
            {
                // Find BindInfo by binding slot
                ConstBuffer* targetCBuffer = nullptr;
                for (BindInfo& bindInfo : mBindings)
                {
                    if (bindInfo.Slot == allocation.BufferIndex)
                    {
                        targetCBuffer = &bindInfo.CBuffer;
                        break;
                    }
                }
                check(targetCBuffer != nullptr);

                mParams[semantic].initialize(allocation.ByteOffset, allocation.ByteSize, targetCBuffer);
            }
        }
    }

    //void initialize(ShaderParamMap& inParamMap);

    void destroy();

    template<typename T>
    void set(uint32_t paramIndex, const T& val)
    {
        mParams[paramIndex].set(val);
    }

    // Commit changes without binding
    void commit();

    // Commit changes and bind ConstBuffers to the pipeline
    void bind(EShaderStage ss, ID3D11DeviceContext* ctx);

protected:
    struct BindInfo
    {
        BindInfo() :Slot(0){}
        BindInfo(uint32_t slot) : Slot(slot) {}

        uint32_t Slot;
        ConstBuffer CBuffer;
    };

protected:
    std::vector<CbufferParameter> mParams;
    std::vector<BindInfo> mBindings;
};

class SamplerParameter
{
public:
    SamplerParameter();

    void initialize(uint32_t slot);

    void set(ID3D11SamplerState* inSampler);

    void destroy();

    void bind(EShaderStage ss, ID3D11DeviceContext* ctx);

protected:
    ID3D11SamplerState* mSampler;
    uint32_t mSlot;
    bool mIsInitialized;
};

class SamplerParameterGroup
{
public:
    // Template version to remove the dependency on how semantics are declared.
    template<typename TParamDecl>
    void initialize_t(ShaderParamMap& inParamMap)
    {
        mParams.resize( (uint32_t)TParamDecl::TotalNum );

        for (uint32_t semantic = 0; semantic < (uint32_t)TParamDecl::TotalNum; ++semantic)
        {
            std::string name = TParamDecl::str[semantic];

            ShaderParamAllocation allocation;
            if (inParamMap.bindParamByName(name, allocation))
            {
                SamplerParameter& sp = mParams[semantic];
                sp.initialize(allocation.ByteOffset);
            }
        }
    }
    //void initialize(ShaderParamMap& inParamMap);

    void set(uint32_t paramIndex, ID3D11SamplerState* inSampler);

    void bind(EShaderStage ss, ID3D11DeviceContext* ctx);

protected:
    std::vector<SamplerParameter> mParams;
};

class TextureParameter
{
public:
    TextureParameter() : mTexture(nullptr), mSlot(0), mIsInitialized(false) { }

    void initialize(uint32_t slot);

    void set(Texture* inTexture);

    void bind(EShaderStage ss);

protected:
    Texture* mTexture;
    uint32_t mSlot;
    bool mIsInitialized;
};

class TextureParameterGroup
{
public:
    // Template version to remove the dependency on how semantics are declared. 
    template<typename TParamDecl>
    void initialize_t(ShaderParamMap& inParamMap)
    {
        mParams.resize((uint32_t)TParamDecl::TotalNum);
        for (uint32_t semantic = 0; semantic < (uint32_t)TParamDecl::TotalNum; ++semantic)
        {
            std::string name = TParamDecl::str[semantic];

            ShaderParamAllocation allocation;
            if (inParamMap.bindParamByName(name, allocation))
            {
                TextureParameter& tp = mParams[semantic];
                tp.initialize(allocation.ByteOffset);
            }
        }
    }

    void initialize(ShaderParamMap& inParamMap);

    void set(uint32_t paramIndex, Texture* inTexture);

    void bind(EShaderStage ss);

protected:
    std::vector<TextureParameter> mParams;
};


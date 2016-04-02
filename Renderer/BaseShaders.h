#pragma once

#include "Shader.h"

class BaseVertexShader : public VertexShader
{
    DECL_SHADER()
public:
    struct ECBufferParam {
        enum Semantic{
            WorldMatrix = 0,
            ViewMatrix,
            ProjMatrix,
            WorldMatrix_InverseTransposed,

            TotalNum,
        };
        static const char* str[];
    };

    struct ESamplerParam {
        enum Semantic{
            TotalNum = 0,
        };
        static const char* str[];
    };

    struct ETextureParam {
        enum Semantic{
            TotalNum = 0,
        };
        static const char* str[];
    };

public:
    BaseVertexShader();

    ~BaseVertexShader();

    virtual void initialize() override;

    virtual void destroy() override;

    virtual void apply() override;

    virtual VertexDeclaration* getVertexDeclaration() override;

    template<typename T>
    void setParameter(ECBufferParam::Semantic semantic, const T& inVal)
    {
        mCBufferParamGroup.set((uint32_t)semantic, inVal);
    }

    inline void setParameter(ETextureParam::Semantic semantic, Texture* inTexture)
    {
        mTextureParamGroup.set((uint32_t)semantic, inTexture);
    }

    inline void setParameter(ESamplerParam::Semantic semantic, ID3D11SamplerState* inSampler)
    {
        mSamplerParamGroup.set((uint32_t)semantic, inSampler);
    }

protected:
    const char* mSourceFileName;

    const char* mEntryPointName;

    ShaderParamMap mParamMap;

    CbufferParameterGroup mCBufferParamGroup;

    SamplerParameterGroup mSamplerParamGroup;

    TextureParameterGroup mTextureParamGroup;

    ID3D11Device* mDev;

    ID3D11DeviceContext* mCtx;

    VertexDeclaration* mVertexDecl;

    ID3D11VertexShader* mVertexShader;

};

class BasePixelShader : public PixelShader
{
    DECL_SHADER()
public:
    // Use struct to mimic enumeration with names
    struct ECBufferParam {
        enum Semantic{
            TotalNum,
        };
        static const char* str[];
    };

    struct ESamplerParam {
        enum Semantic{
            TotalNum,
        };
        static const char* str[];
    };

    struct ETextureParam {
        enum Semantic{
            TotalNum,
        };
        static const char* str[];
    };

public:
    BasePixelShader();

    ~BasePixelShader();

    virtual void initialize() override;

    virtual void destroy() override;

    virtual void apply() override;

    // CBuffer
    template<typename T>
    void setParameter(ECBufferParam::Semantic semantic, const T& inVal)
    {
        mCBufferParams.set(semantic, inVal);
    }

    // Texture
    inline void setParameter(ETextureParam::Semantic semantic, Texture* inTexture)
    {
        mTextureParams.set(semantic, inTexture);
    }

    // Sampler
    inline void setParameter(ESamplerParam::Semantic semantic, ID3D11SamplerState* inSampler)
    {
        mSamplerParams.set(semantic, inSampler);
    }

protected:
    const char* mSourceFileName;

    const char* mEntryPointName;

    ID3D11Device* mDev;

    ID3D11DeviceContext* mCtx;

    ShaderParamMap mParamMap;

    CbufferParameterGroup mCBufferParams;

    SamplerParameterGroup mSamplerParams;

    TextureParameterGroup mTextureParams;

    ID3D11PixelShader* mPixelShader;
};


#pragma once

#include "Shader.h"

class GuiVertexShader : VertexShader
{ 
    DECL_SHADER()
public:
    struct ECBufferParam {
        enum Semantic{
            ViewMatrix = 0,
            ProjMatrix,

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
    GuiVertexShader();

    ~GuiVertexShader();

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

class GuiPixelShaderDSL : PixelShader
{
    DECL_SHADER()
public:
    // Use struct to mimic enumeration with names
    struct ECBufferParam {
        enum Semantic{
            GuiMiscParam = 0,

            TotalNum,
        };
        static const char* str[];
    };

    struct ESamplerParam {
        enum Semantic{
            GuiSampler = 0,

            TotalNum,
        };
        static const char* str[];
    };

    struct ETextureParam {
        enum Semantic{
            GuiTexture = 0,
            LineFilter,

            TotalNum,
        };
        static const char* str[];
    };

public:
    GuiPixelShaderDSL();

    ~GuiPixelShaderDSL();

    virtual void initialize() override;

    virtual void destroy() override;

    virtual void apply() override;

    // Dynamic linkage 
    inline void setNoTextureShader()
    {
        mInterfacePtrs[ELinkageSlot::IColor] = mInstances[EClassType::NoTextureColor];
    }

    inline void setDefaultShader()
    {
        mInterfacePtrs[ELinkageSlot::IColor] = mInstances[EClassType::DefaultColor];
    }
    inline void setFontShader()
    {
        mInterfacePtrs[ELinkageSlot::IColor] = mInstances[EClassType::FontColor];
    }

    inline void setLineShader()
    {
        mInterfacePtrs[ELinkageSlot::IColor] = mInstances[EClassType::LineColor];
    }

    inline void setBorderShader()
    {
        mInterfacePtrs[ELinkageSlot::IColor] = mInstances[EClassType::BorderColor];
    }

    inline void enableDebugClip()
    {
        mInterfacePtrs[ELinkageSlot::IClip] = mInstances[EClassType::DebugClipOn];
    }

    inline void disableDebugClip()
    {
        mInterfacePtrs[ELinkageSlot::IClip] = mInstances[EClassType::NormalClipOn];
    }

    inline void enableDisabledEffect()
    {
        mInterfacePtrs[ELinkageSlot::IDisabledEffect] = mInstances[EClassType::DisabledOn];
    }
    inline void disableDisabledEffect()
    {
        mInterfacePtrs[ELinkageSlot::IDisabledEffect] = mInstances[EClassType::DisabledOff];
    }

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
    enum ELinkageSlot
    {
        IClip           = 0,
        IColor          = 1,
        IDisabledEffect = 2,
        TotalNum        = 3,
    };

    struct EClassType
    {
        static const char* str[];
        enum 
        {
            NormalClipOn = 0,
            DebugClipOn,
            NoTextureColor,
            DefaultColor,
            FontColor,
            LineColor,
            BorderColor,
            DisabledOn,
            DisabledOff,

            TotalNum
        };
    };

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

    // Dynamic shader linkage 
    ID3D11ClassLinkage* mClassLinkage;

    std::vector<ID3D11ClassInstance*> mInstances;

    std::vector<ID3D11ClassInstance*> mInterfacePtrs;
};


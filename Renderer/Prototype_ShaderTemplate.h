#pragma once
#include "Shader.h"
#include <d3dcompiler.h>
#include "Renderer.h"

/*
A variant of CRTP to abstract shader management tasks into the template base class, allowing
derived shader class to provide/define its own parameter layout and dynamic linkage. 

For the record, this design is not in use for now, as it is based on several relatively simple shaders. 
We'll see if it fits into bigger pictures.
*/
template<typename EnumCBufferParam, typename EnumTextureParam, typename EnumSamplerParam>
class PixelShader_Temp : public IShader
{
    DECL_SHADER()
public:
    PixelShader_Temp() :mDev(nullptr), mCtx(nullptr), mPixelShader(nullptr)
    {
    }
     
    ~PixelShader_Temp()
    {
        destroy();
    }

    virtual void initialize() override
    {
        // Skip re-creation without destroy first
        if (mPixelShader) return;

        mCtx = Renderer::getInstance()->getContext();
        mDev = Renderer::getInstance()->getDevice();

        // Construct input for shader loader
        CompilerInput compilerInput;
        compilerInput.CompilerTarget.setTarget(EShaderStage::PixelShader_SM5);
        compilerInput.SrcFileName = std::string(sourceFilename());
        compilerInput.EntryPointName = std::string(entrypointName());
    #if defined(_DEBUG) || defined(DEBUG)
        compilerInput.Flags.addFlag(D3DCOMPILE_DEBUG);
        compilerInput.Flags.addFlag(D3DCOMPILE_SKIP_OPTIMIZATION);
    #endif

        // Load from disk cache or recompile if cache is outdated
        CompilerOutput compilerOutput;
        ShaderLoader::getInstance()->load(compilerInput, compilerOutput);

        // Get shader parameter map from compiler output
        mParamMap = compilerOutput.ParamMap;

        // Create d3d shader object
        const void* pCode = &compilerOutput.ByteCode[0];
        const uint32_t codeSize = compilerOutput.ByteCode.size();
        mDev->CreatePixelShader(pCode, codeSize, nullptr, &mPixelShader);

        // Class instances can be 'empty', i.e. their class has no member variable defined.
        // Empty and non-empty class instances are handled differently:

        mCBufferParamGroup.initialize_t<EnumCBufferParam>(mParamMap);
        mTextureParamGroup.initialize_t<EnumTextureParam>(mParamMap);
        mSamplerParamGroup.initialize_t<EnumSamplerParam>(mParamMap);

        check(mParamMap.checkIfAllParamsAreBound());
    }

    virtual void destroy() override
    {
        safe_release(mPixelShader);
    }

    virtual void apply() override
    {
        mCBufferParamGroup.bind(EShaderStage::PixelShader_SM5, mCtx);
        mSamplerParamGroup.bind(EShaderStage::PixelShader_SM5, mCtx);
        mTextureParamGroup.bind(EShaderStage::PixelShader_SM5);
        mCtx->PSSetShader(mPixelShader, nullptr, 0);
    }

    template<typename T>
    void setParameter(typename EnumCBufferParam::Semantic semantic, const T& inVal)
    {
        mCBufferParamGroup.set((uint32_t)semantic, inVal);
    }

    inline void setParameter(typename EnumTextureParam::Semantic semantic, Texture* inTexture)
    {
        mTextureParamGroup.set((uint32_t)semantic, inTexture);
    }

    inline void setParameter(typename EnumSamplerParam::Semantic semantic, ID3D11SamplerState* inSampler)
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

};

struct ETempCB {
    static const char* str[];
    enum Semantic {
        WorldMatrix = 0,
        ViewMatrix,
        ProjMatrix,
        GuiMiscParam,

        TotalNum
    };
};

struct ETempTexture 
{
    static const char* str[];
    enum Semantic{
        DiffuseTexture = 0,
        FontTexture,
        GuiTexture,
        LineFilter,

        TotalNum
    };
};

struct ETempSampler 
{
    static const char* str[];
    enum Semantic{
        DiffuseSampler = 0,
        FontSampler,
        GuiSampler,

        TotalNum
    };
};

class GuiDefaultPS : public PixelShader_Temp<ETempCB, ETempTexture, ETempSampler>
{
    DECL_SHADER()
protected:
    const char* sourceFilename() const override
    {
        static const char* filename = "GuiDefaultPS.hlsl";
        return filename;
    }

    const char* entrypointName() const override
    {
        static const char* entrypoint = "PSMain";
        return entrypoint;
    }
};

class GuiFontPS : public PixelShader_Temp<ETempCB, ETempTexture, ETempSampler>
{
    DECL_SHADER()
protected:
    const char* sourceFilename() const override
    {
        static const char* filename = "GuiFontPS.hlsl";
        return filename;
    }

    const char* entrypointName() const override
    {
        static const char* entrypoint = "PSMain";
        return entrypoint;
    }
};

class GuiTexturePS : public PixelShader_Temp<ETempCB, ETempTexture, ETempSampler>
{
    DECL_SHADER()
protected:
    const char* sourceFilename() const override
    {
        static const char* filename = "GuiTexturePS.hlsl";
        return filename;
    }

    const char* entrypointName() const override
    {
        static const char* entrypoint = "PSMain";
        return entrypoint;
    }

};

class GuiBorderPS: public PixelShader_Temp<ETempCB, ETempTexture, ETempSampler>
{
    DECL_SHADER()
protected:
    const char* sourceFilename() const override
    {
        static const char* filename = "GuiBorderPS.hlsl";
        return filename;
    }

    const char* entrypointName() const override
    {
        static const char* entrypoint = "PSMain";
        return entrypoint;
    }

};

class GuiLinePS: public PixelShader_Temp<ETempCB, ETempTexture, ETempSampler>
{
    DECL_SHADER()
protected:
    const char* sourceFilename() const override
    {
        static const char* filename = "GuiLinePS.hlsl";
        return filename;
    }

    const char* entrypointName() const override
    {
        static const char* entrypoint = "PSMain";
        return entrypoint;
    }

};

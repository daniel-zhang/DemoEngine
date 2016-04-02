#include "ShaderParameter.h"


CbufferParameter::CbufferParameter() : 
    mCBufferRef(nullptr) ,
    mByteOffset(0),
    mByteSize(0)
{
}

CbufferParameter::~CbufferParameter()
{
    destroy();
}

void CbufferParameter::initialize(uint32_t offset, uint32_t size, ConstBuffer* cbuffer)
{
    mCBufferRef = cbuffer;
    mByteOffset = offset;
    mByteSize = size;
}

void CbufferParameter::destroy()
{
    mCBufferRef = nullptr;
    mByteOffset = 0;
    mByteSize = 0;
}

/*
void CbufferParameterGroup::initialize(ShaderParamMap& inParamMap)
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
    mParams.resize((uint32_t)ECbufferSemantics::TotalNum);

    // Initialize parameters if they are defined in ShaderParamMap
    for (uint32_t semantic = 0; semantic < (uint32_t)ECbufferSemantics::TotalNum; ++semantic)
    {
        // Translate semantic into shader variable name string
        std::string name;
        translateSemantic((ECbufferSemantics)semantic, name);

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

            mParams[semantic].initialize( allocation.ByteOffset, allocation.ByteSize, targetCBuffer );
        }
    }
}
*/

void CbufferParameterGroup::destroy()
{
    mBindings.clear();
    mParams.clear();
}

void CbufferParameterGroup::commit()
{
    for (BindInfo& binding : mBindings)
    {
        binding.CBuffer.commit();
    }
}

void CbufferParameterGroup::bind(EShaderStage ss, ID3D11DeviceContext* ctx)
{
    for (BindInfo& binding : mBindings)
    {
        binding.CBuffer.commit();

        switch (ss)
        {
        case VertexShader_SM5:
            ctx->VSSetConstantBuffers(binding.Slot, 1, binding.CBuffer.getResourcePtr());
            break;
        case PixelShader_SM5:
            ctx->PSSetConstantBuffers(binding.Slot, 1, binding.CBuffer.getResourcePtr());
            break;

        case HullShader_SM5:
            ctx->HSSetConstantBuffers(binding.Slot, 1, binding.CBuffer.getResourcePtr());
            break;

        case DomainShader_SM5:
            ctx->DSSetConstantBuffers(binding.Slot, 1, binding.CBuffer.getResourcePtr());
            break;

        case GeometryShader_SM5:
            ctx->GSSetConstantBuffers(binding.Slot, 1, binding.CBuffer.getResourcePtr());
            break;

        case ComputeShader_SM5:
            ctx->CSSetConstantBuffers(binding.Slot, 1, binding.CBuffer.getResourcePtr());
            break;

        default:
            break;
        }

    }
}

SamplerParameter::SamplerParameter() : 
    mSampler(nullptr), mIsInitialized(false), mSlot(0)
{
}

void SamplerParameter::initialize(uint32_t slot)
{
    mSlot = slot;
    mIsInitialized = true;
}

void SamplerParameter::set(ID3D11SamplerState* inSampler)
{
    mSampler = inSampler;
}

void SamplerParameter::destroy()
{
    mSampler = nullptr;
    mIsInitialized = false;
    mSlot = 0;
}

void SamplerParameter::bind(EShaderStage ss, ID3D11DeviceContext* ctx)
{
    if (!mIsInitialized || !mSampler)
        return;

    switch (ss)
    {
    case VertexShader_SM5:
        ctx->VSSetSamplers(mSlot, 1, &mSampler);
        break;
    case HullShader_SM5:
        ctx->HSSetSamplers(mSlot, 1, &mSampler);
        break;
    case DomainShader_SM5:
        ctx->DSSetSamplers(mSlot, 1, &mSampler);
        break;
    case PixelShader_SM5:
        ctx->PSSetSamplers(mSlot, 1, &mSampler);
        break;
    case GeometryShader_SM5:
        ctx->GSSetSamplers(mSlot, 1, &mSampler);
        break;
    case ComputeShader_SM5:
        ctx->CSSetSamplers(mSlot, 1, &mSampler);
        break;
    default:
        break;
    }
}


/*
void SamplerParameterGroup::initialize(ShaderParamMap& inParamMap)
{
    mParams.resize((uint32_t)ESamplerSemantics::TotalNum);

    for (uint32_t semantic = 0; semantic < (uint32_t)ESamplerSemantics::TotalNum; ++semantic)
    {
        std::string name;
        translateSemantic((ESamplerSemantics)semantic, name);

        ShaderParamAllocation allocation;
        if (inParamMap.bindParamByName(name, allocation))
        {
            SamplerParameter& sp = mParams[semantic];
            sp.initialize(allocation.ByteOffset);
        }
    }
}*/

void SamplerParameterGroup::set(uint32_t paramIndex, ID3D11SamplerState* inSampler)
{
    mParams[paramIndex].set(inSampler);
}

void SamplerParameterGroup::bind(EShaderStage ss, ID3D11DeviceContext* ctx)
{
    for (SamplerParameter& sp : mParams)
    {
        sp.bind(ss, ctx);
    }
}

void TextureParameter::initialize(uint32_t slot)
{
    mSlot = slot;
    mIsInitialized = true;
}

void TextureParameter::set(Texture* inTexture)
{
    mTexture = inTexture;
}

void TextureParameter::bind(EShaderStage ss)
{
    if (mTexture && mIsInitialized)
    {
        mTexture->bindAsShaderResource(ss, mSlot);
    }
}

/*
void TextureParameterGroup::initialize(ShaderParamMap& inParamMap)
{
    mParams.resize((uint32_t)ETexture2DSemantics::TotalNum);
    for (uint32_t semantic = 0; semantic < (uint32_t)ETexture2DSemantics::TotalNum; ++semantic)
    {
        std::string name;
        translateSemantic((ETexture2DSemantics)semantic, name);

        ShaderParamAllocation allocation;
        if (inParamMap.bindParamByName(name, allocation))
        {
            TextureParameter& tp = mParams[semantic];
            tp.initialize(allocation.ByteOffset);
        }
    }
}*/

void TextureParameterGroup::set(uint32_t paramIndex, Texture* inTexture)
{
    mParams[paramIndex].set(inTexture);
}

void TextureParameterGroup::bind(EShaderStage ss)
{
    for (TextureParameter& param : mParams)
    {
        param.bind(ss);
    }
}

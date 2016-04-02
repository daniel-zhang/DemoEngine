#include "BaseShaders.h"
#include "Renderer.h"
#include <d3dcompiler.h>

const char* BaseVertexShader::ECBufferParam::str[] = {
    "gWorld",
    "gView",
    "gProj",
    "gWorld_InverseTranspose",
};

const char* BaseVertexShader::ESamplerParam::str[] = {
    ""
};

const char* BaseVertexShader::ETextureParam::str[] = {
    ""
};

BaseVertexShader::BaseVertexShader() :
    mDev(nullptr), mCtx(nullptr),
    mVertexShader(nullptr), mVertexDecl(nullptr),
    mSourceFileName("BaseVS.hlsl"), mEntryPointName("VSMain")
{
}

BaseVertexShader::~BaseVertexShader()
{
    destroy();
}

void BaseVertexShader::initialize()
{
    // Skip re-creation without destroy first
    if (mVertexShader) return;

    mCtx = Renderer::getInstance()->getContext();
    mDev = Renderer::getInstance()->getDevice();

    // Load shader
    CompilerInput compilerInput;
    compilerInput.CompilerTarget.setTarget(EShaderStage::VertexShader_SM5);
    compilerInput.SrcFileName = mSourceFileName;
    compilerInput.EntryPointName = mEntryPointName;
#if defined(_DEBUG) || defined(DEBUG)
    compilerInput.Flags.addFlag(D3DCOMPILE_DEBUG);
    compilerInput.Flags.addFlag(D3DCOMPILE_SKIP_OPTIMIZATION);
#endif

    CompilerOutput compilerOutput;
    ShaderLoader::getInstance()->load(compilerInput, compilerOutput);

    // Create d3d shader object
    const void* pCode = &compilerOutput.ByteCode[0];
    const uint32_t codeSize = compilerOutput.ByteCode.size();
    mDev->CreateVertexShader(pCode, codeSize, NULL, &mVertexShader);

    // Create parameters from reflection data
    mParamMap = compilerOutput.ParamMap;
    mCBufferParamGroup.initialize_t<ECBufferParam>(mParamMap);
    mTextureParamGroup.initialize_t<ETextureParam>(mParamMap);
    mSamplerParamGroup.initialize_t<ESamplerParam>(mParamMap);

    //check(mParamMap.checkIfAllParamsAreBound());

    // Get or create vertex declaration 
    mVertexDecl = BaseVertexDecl::getInstance();
    mVertexDecl->setVertexShaderSignature(pCode, codeSize);
    mVertexDecl->create();
}

void BaseVertexShader::destroy()
{
    mDev = nullptr;
    mCtx = nullptr;

    mVertexDecl = nullptr;
    safe_release(mVertexShader);
}

void BaseVertexShader::apply()
{
    // Bind input layout 
    mVertexDecl->bind();

    // Bind shader resource
    mCBufferParamGroup.bind(EShaderStage::VertexShader_SM5, mCtx);

    mSamplerParamGroup.bind(EShaderStage::VertexShader_SM5, mCtx);

    mTextureParamGroup.bind(EShaderStage::VertexShader_SM5);

    // Bind shader
    mCtx->VSSetShader(mVertexShader, nullptr, 0);
}

VertexDeclaration* BaseVertexShader::getVertexDeclaration()
{
    return mVertexDecl;
}


const char* BasePixelShader::ECBufferParam::str[] = { "" };

const char* BasePixelShader::ESamplerParam::str[] = {""};

const char* BasePixelShader::ETextureParam::str[] = {""};

BasePixelShader::BasePixelShader() : 
    mSourceFileName("BasePS.hlsl"),
    mEntryPointName("PSMain"),
    mDev(nullptr),
    mCtx(nullptr),
    mPixelShader(nullptr)
{
}

BasePixelShader::~BasePixelShader()
{
    destroy();
}

void BasePixelShader::initialize()
{
    mCtx = Renderer::getInstance()->getContext();
    mDev = Renderer::getInstance()->getDevice();

    // Construct input for shader loader
    CompilerInput compilerInput;
    compilerInput.CompilerTarget.setTarget(EShaderStage::PixelShader_SM5);
    compilerInput.SrcFileName = mSourceFileName;
    compilerInput.EntryPointName = mEntryPointName;
#if defined(_DEBUG) || defined(DEBUG)
    compilerInput.Flags.addFlag(D3DCOMPILE_DEBUG);
    compilerInput.Flags.addFlag(D3DCOMPILE_SKIP_OPTIMIZATION);
#endif

    // Load from disk cache or recompile if cache is outdated
    CompilerOutput compilerOutput;
    ShaderLoader::getInstance()->load(compilerInput, compilerOutput);

    // Create params from ParamMap
    mParamMap = compilerOutput.ParamMap;

    mCBufferParams.initialize_t<ECBufferParam>(mParamMap);
    mTextureParams.initialize_t<ETextureParam>(mParamMap);
    mSamplerParams.initialize_t<ESamplerParam>(mParamMap);

    //check(mParamMap.checkIfAllParamsAreBound());

    // Create d3d shader object
    const void* pCode = &compilerOutput.ByteCode[0];
    const uint32_t codeSize = compilerOutput.ByteCode.size();
    mDev->CreatePixelShader(pCode, codeSize, nullptr, &mPixelShader);
}

void BasePixelShader::destroy()
{
    mDev = nullptr;
    mCtx = nullptr;

    safe_release(mPixelShader);
}

void BasePixelShader::apply()
{
    // Bind shader resources
    mCBufferParams.bind(EShaderStage::PixelShader_SM5, mCtx);

    mSamplerParams.bind(EShaderStage::PixelShader_SM5, mCtx);

    mTextureParams.bind(EShaderStage::PixelShader_SM5);

    mCtx->PSSetShader(mPixelShader, nullptr, 0);
}


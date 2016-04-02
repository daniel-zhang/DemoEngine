#include "GuiShaders.h"
#include "Renderer.h"
#include <D3DCompiler.h>


/*
const char* GuiDefaultVS::sourceFilename() const
{
    static const char* filename = "GuiDefaultVS.hlsl";
    return filename;
}

const char* GuiDefaultVS::entrypointName() const
{
    static const char* entrypoint = "VSMain";
    return entrypoint;
}

void GuiDefaultVS::createVertexDeclaration(const void* pCode, const uint32_t bytesize)
{
    mVertexDecl = GuiVertexDecl::getInstance();

    // Try to create it if not created yet
    mVertexDecl->setVertexShaderSignature(pCode, bytesize);
    mVertexDecl->create();
}
*/

const char* GuiVertexShader::ECBufferParam::str[] = {
    "gView",
    "gProj",
};

const char* GuiVertexShader::ESamplerParam::str[] = {
    ""
};

const char* GuiVertexShader::ETextureParam::str[] = {
    ""
};

GuiVertexShader::GuiVertexShader() : 
    mDev(nullptr), mCtx(nullptr), 
    mVertexShader(nullptr), mVertexDecl(nullptr),
    mSourceFileName("GuiDefaultVS.hlsl"), mEntryPointName("VSMain")
{
}

GuiVertexShader::~GuiVertexShader()
{
    destroy();
}

void GuiVertexShader::initialize()
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
    mVertexDecl = GuiVertexDecl::getInstance();
    mVertexDecl->setVertexShaderSignature(pCode, codeSize);
    mVertexDecl->create();
}

void GuiVertexShader::destroy()
{
    mDev = nullptr;
    mCtx = nullptr;

    mVertexDecl = nullptr;
    safe_release(mVertexShader);
}

void GuiVertexShader::apply()
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

VertexDeclaration* GuiVertexShader::getVertexDeclaration()
{
    return mVertexDecl;
}


const char* GuiPixelShaderDSL::ECBufferParam::str[] = {
    "gGuiParam",
};

const char* GuiPixelShaderDSL::ESamplerParam::str[] = {
    "gGuiSampler",
};

const char* GuiPixelShaderDSL::ETextureParam::str[] = {
    "gGuiTexture",
    "gLineFilter",
};

const char* GuiPixelShaderDSL::EClassType::str[] = {
    "NormalClipOn",
    "DebugClipOn",
    "NoTextureColor",
    "DefaultColor",
    "FontColor",
    "LineColor",
    "BorderColor",
    "DisabledOn",
    "DisabledOff",
};

const char* GuiPS_InstanceNames[] = {
    "gNormalClipOn",
    "gDebugClipOn",
    "gNoTextureColor",
    "gDefaultColor",
    "gFontColor",
    "gLineColor",
    "gBorderColor",
    "gDisabledOn",
    "gDisabledOff",
};

GuiPixelShaderDSL::GuiPixelShaderDSL() : 
    mSourceFileName("GuiPS_DSL.hlsl"),
    mEntryPointName("PSMain"),
    mDev(nullptr),
    mCtx(nullptr),
    mPixelShader(nullptr),
    mClassLinkage(nullptr)
{
}

GuiPixelShaderDSL::~GuiPixelShaderDSL()
{
    destroy();
}

void GuiPixelShaderDSL::destroy()
{
    mDev = nullptr;
    mCtx = nullptr;

    safe_release(mPixelShader);
    safe_release(mClassLinkage);

    for (uint32_t i = 0; i < mInstances.size(); ++i)
    {
        safe_release(mInstances[i]);
    }
}

void GuiPixelShaderDSL::initialize()
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

    // Create a class linkage object only used by this shader
    mDev->CreateClassLinkage(&mClassLinkage);

    // Create d3d shader object
    const void* pCode = &compilerOutput.ByteCode[0];
    const uint32_t codeSize = compilerOutput.ByteCode.size();
    mDev->CreatePixelShader(pCode, codeSize, mClassLinkage, &mPixelShader);

    // Create instances
    for (uint32_t i = 0; i < (uint32_t)EClassType::TotalNum; ++i)
    {
        ID3D11ClassInstance* ci = nullptr;

         mClassLinkage->CreateClassInstance(EClassType::str[i], 0, 0, 0, 0, &ci);
        //mClassLinkage->GetClassInstance(GuiPS_InstanceNames[i], 0, &ci);

        mInstances.push_back(ci);
    }

    mInterfacePtrs.resize(ELinkageSlot::TotalNum, nullptr);

    // Set default state
    setDefaultShader();
    disableDebugClip();
    disableDisabledEffect();
}

void GuiPixelShaderDSL::apply()
{
    // Bind shader resources
    mCBufferParams.bind(EShaderStage::PixelShader_SM5, mCtx);

    mSamplerParams.bind(EShaderStage::PixelShader_SM5, mCtx);

    mTextureParams.bind(EShaderStage::PixelShader_SM5);

    // Bind shader
    //void PSSetShader( ID3D11PixelShader *pPixelShader,  ID3D11ClassInstance *const *ppClassInstances, UINT NumClassInstances) = 0;
    if (mInterfacePtrs.empty())
    {
        mCtx->PSSetShader(mPixelShader, nullptr, 0);
    }
    else
    {
        mCtx->PSSetShader(mPixelShader, &mInterfacePtrs[0], mInterfacePtrs.size());
    }
}


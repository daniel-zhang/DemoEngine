#include "Shader.h"
#include "Renderer.h"
#include <D3DCompiler.h>

/*
void VertexShader::create()
{
    // Skip re-creation without destroy first
    if (mVertexShader) return;

    mCtx = Renderer::getInstance()->getContext();
    mDev = Renderer::getInstance()->getDevice();

    // Construct input for shader loader
    CompilerInput compilerInput;
    compilerInput.CompilerTarget.setTarget(EShaderStage::VertexShader_SM5);
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
    mDev->CreateVertexShader(pCode, codeSize, NULL, &mVertexShader);

    // Create the proper vertex declaration if not created yet
    createVertexDeclaration(pCode, codeSize);

    // Create parameters from ParamMap
    mCBufferParamGroup.initialize(mParamMap);
    mTextureParamGroup.initialize(mParamMap);
    mSamplerParamGroup.initialize(mParamMap);

    check(mParamMap.checkIfAllParamsAreBound());
}

void VertexShader::destroy()
{
    mVertexDecl = nullptr;
    safe_release(mVertexShader);
}

void VertexShader::recreate()
{
    destroy();
    create();
}

void VertexShader::apply()
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

void PixelShader::create()
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

    // Create a class linkage object only used by this shader
    mDev->CreateClassLinkage(&mClassLinkage);

    // Create d3d shader object
    const void* pCode = &compilerOutput.ByteCode[0];
    const uint32_t codeSize = compilerOutput.ByteCode.size();
    mDev->CreatePixelShader(pCode, codeSize, mClassLinkage, &mPixelShader);

    // Class instances can be 'empty', i.e. their class has no member variable defined.
    // Empty and non-empty class instances are handled differently:
     CreateClassInstance(
        LPCSTR pClassTypeName,
        UINT ConstantBufferOffset,
        UINT ConstantVectorOffset,
        UINT TextureOffset,
        UINT SamplerOffset,
        ID3D11ClassInstance **ppInstance) = 0; 
     GetClassInstance(
        LPCSTR pClassInstanceName,
        UINT InstanceIndex,
        ID3D11ClassInstance **ppInstance) = 0;
   */


/*
// Is it possible to reflect interface pointers of a shader?
// - Yes, they are all stored in a special cbuffer called $ThisPointer
// Are 'empty' class instances "invisible" to shader reflection?
// - Yes, they are totally dropped by compiler even if declared in a cbuffer
// Non-empty class instances are seen as regularly bound cbuffer vars, is it possible to pick them up by type instead of by name?
// -
    // Create params from ParamMap
    mCBufferParamGroup.initialize(mParamMap);
    mTextureParamGroup.initialize(mParamMap);
    mSamplerParamGroup.initialize(mParamMap);

    check(mParamMap.checkIfAllParamsAreBound());
}

void PixelShader::destroy()
{
    safe_release(mClassLinkage);
    safe_release(mPixelShader);
}

void PixelShader::recreate()
{
    destroy();
    create();
}

void PixelShader::apply()
{
    // Bind shader resources
    mCBufferParamGroup.bind(EShaderStage::PixelShader_SM5, mCtx);

    mSamplerParamGroup.bind(EShaderStage::PixelShader_SM5, mCtx);

    mTextureParamGroup.bind(EShaderStage::PixelShader_SM5);

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
*/

#include "Compiler.h"

#include <d3dcompiler.h>
#include <d3d11shader.h>

class Reflection
{
public:
    struct InterfacePointerDesc
    {
        uint32_t InterfaceSlot;
        D3D11_SHADER_VARIABLE_DESC VarDesc;
        D3D11_SHADER_TYPE_DESC ShaderTypeDesc;
    };

    struct CBufferVariableDesc
    {
        uint32_t InterfaceSlot;
        std::string ParentCBufferName;
        D3D11_SHADER_VARIABLE_DESC VarDesc;
        D3D11_SHADER_TYPE_DESC ShaderTypeDesc;
    };

public:
    void initialize(ID3D11ShaderReflection* r)
    {
        D3D11_SHADER_DESC shaderDesc;
        r->GetDesc(&shaderDesc);

        for (uint32_t i = 0; i < shaderDesc.ConstantBuffers; ++i)
        {
            ID3D11ShaderReflectionConstantBuffer* cbuffer = r->GetConstantBufferByIndex(i);
            D3D11_SHADER_BUFFER_DESC bufferDesc;
            cbuffer->GetDesc(&bufferDesc);
            mBuffers.push_back(bufferDesc);

            // Interface pointers are stored in a dedicated system CBuffer called "$ThisPointer", 
            // which does not count as a bound CBuffer.
            if (bufferDesc.Type == D3D_CBUFFER_TYPE::D3D11_CT_INTERFACE_POINTERS)
            {
                for (uint32_t varIndex = 0; varIndex < bufferDesc.Variables; ++varIndex)
                {
                    InterfacePointerDesc ipDesc;

                    ID3D11ShaderReflectionVariable* interfacePointer = cbuffer->GetVariableByIndex(varIndex);
                    interfacePointer->GetDesc(&ipDesc.VarDesc);
                    interfacePointer->GetType()->GetDesc(&ipDesc.ShaderTypeDesc);
                    ipDesc.InterfaceSlot = interfacePointer->GetInterfaceSlot(0);

                    mInterfacePointers.push_back(ipDesc);
                }
            }
            else if (bufferDesc.Type == D3D_CBUFFER_TYPE::D3D_CT_CBUFFER)
            {
                for (uint32_t varIndex = 0; varIndex < bufferDesc.Variables; ++varIndex)
                {
                    CBufferVariableDesc cbvDesc;

                    cbvDesc.ParentCBufferName = bufferDesc.Name;

                    ID3D11ShaderReflectionVariable* var = cbuffer->GetVariableByIndex(varIndex);
                    var->GetDesc(&cbvDesc.VarDesc);
                    var->GetType()->GetDesc(&cbvDesc.ShaderTypeDesc);
                    cbvDesc.InterfaceSlot = var->GetInterfaceSlot(0);

                    mCBufferVariables.push_back(cbvDesc);
                }
            }
            else 
            {
                // Ignore other cbuffer types, namely D3D_CT_TBUFFER and D3D_CT_RESOURCE_BIND_INFO 
            }
        }

        for (uint32_t i = 0; i < shaderDesc.BoundResources; ++i)
        {
            D3D11_SHADER_INPUT_BIND_DESC bindDesc;
            r->GetResourceBindingDesc(i, &bindDesc);
            mBindings.push_back(bindDesc);
        }

        for (uint32_t i = 0; i < shaderDesc.InputParameters; ++i)
        {
            D3D11_SIGNATURE_PARAMETER_DESC signatureDesc;
            r->GetInputParameterDesc(i, &signatureDesc);
            mInputSignatures.push_back(signatureDesc);
        }

        for (uint32_t i = 0; i < shaderDesc.OutputParameters; ++i)
        {
            D3D11_SIGNATURE_PARAMETER_DESC signatureDesc;
            r->GetOutputParameterDesc(i, &signatureDesc);
            mOutputSignatures.push_back(signatureDesc);
        }

        mNumInterfacePointer = r->GetNumInterfaceSlots();

    }

    void toString()
    {

    }

protected:
    std::vector<D3D11_SHADER_BUFFER_DESC> mBuffers;
    std::vector<D3D11_SHADER_INPUT_BIND_DESC> mBindings;
    std::vector<D3D11_SIGNATURE_PARAMETER_DESC> mInputSignatures;
    std::vector<D3D11_SIGNATURE_PARAMETER_DESC> mOutputSignatures;

    uint32_t mNumInterfacePointer;

    std::vector<InterfacePointerDesc> mInterfacePointers;
    std::vector<CBufferVariableDesc> mCBufferVariables;

};

/*
Memo: 
Shader includes and macros 
    they are there but not used, I don't need preprocessing for now.

Source hash
    in the absence of a preprocessor, a shader's include files are not tracked.
    this implies when include files change, the shader that includes them does not get recompiled.
    add a space in the shader source to mark its csc(compiled shader cache) outdated.

CompilerOutput::Errors
    not used for the moment, just trigger assert failure on error
*/
bool compileShader(const CompilerInput& input, CompilerOutput& output,  const std::string& srcContent )
{
    /*
    std::string srcContent;
    {
        std::ifstream ifs( input.SrcFileName, std::ifstream::in);

        check(ifs.good());

        ifs.seekg(0, std::ios::end);
        size_t size = static_cast<size_t>( ifs.tellg() );
        ifs.seekg(0);

        srcContent.resize(size);

        ifs.read(&srcContent[0], size);
    }
    */

    // Append additional macros. Allow override?
    std::vector<D3D_SHADER_MACRO> d3dMacros;
    auto inputMacros = input.Macros.getMacroDefines();
    for (auto m : inputMacros)
    {
        D3D_SHADER_MACRO d3dM;
        d3dM.Name = m.first.c_str();
        d3dM.Definition = m.second.c_str();

        d3dMacros.push_back(d3dM);
    }
    // Append a NULL-NULL pair to let shader compiler know where to terminate.
    D3D_SHADER_MACRO NullMacro = { NULL, NULL };
    d3dMacros.push_back(NullMacro);

    ID3DBlob* shaderCode;
    ID3DBlob* errors;

    HRESULT hr = ::D3DCompile(
        srcContent.c_str(),
        srcContent.size(),
        input.SrcFileName.c_str(),
        &d3dMacros[0],
        D3D_COMPILE_STANDARD_FILE_INCLUDE,
        input.EntryPointName.c_str(),
        input.CompilerTarget.toShaderProfileString(),
        input.Flags.getFlags(),
        0,
        &shaderCode,
        &errors
        );

    if (errors)
    {
        ::OutputDebugStringA((char*)errors->GetBufferPointer());

        CompilerOutput::CompilerError cErr;
        cErr.ErrorMsg = std::string((char*)errors->GetBufferPointer(), errors->GetBufferSize());

        output.Errors.push_back(cErr);
        output.Succeeded = false;

        safe_release(errors);
        return false;
    }

    check_hr(hr);

    output.Succeeded = true;

    uint32_t numByteCode = shaderCode->GetBufferSize();
    output.ByteCode.resize(numByteCode);
    memcpy(&output.ByteCode[0], shaderCode->GetBufferPointer(), numByteCode);

    safe_release(shaderCode);
    safe_release(errors);

    //
    // Now inspect the shader code via reflection
    //
    ID3D11ShaderReflection* reflector = nullptr;

    check_hr( 
        ::D3DReflect(&output.ByteCode[0], output.ByteCode.size(), IID_ID3D11ShaderReflection, (void**)&reflector)
        );

    Reflection rf;
    rf.initialize(reflector);

    D3D11_SHADER_DESC shaderDesc;
    reflector->GetDesc(&shaderDesc);

    output.NumInstructions = shaderDesc.InstructionCount;

#if defined(_DEBUG) || defined(DEBUG)
    // Extract shader input/output signatures 
    std::vector<D3D11_SIGNATURE_PARAMETER_DESC> debugInputSignatures;
    std::vector<D3D11_SIGNATURE_PARAMETER_DESC> debugOutputSignatures;

    for (uint32_t i = 0; i < shaderDesc.InputParameters; ++i)
    {
        D3D11_SIGNATURE_PARAMETER_DESC signatureDesc;
        reflector->GetInputParameterDesc(i, &signatureDesc);
        debugInputSignatures.push_back(signatureDesc);
    }

    for (uint32_t i = 0; i < shaderDesc.OutputParameters; ++i)
    {
        D3D11_SIGNATURE_PARAMETER_DESC signatureDesc;
        reflector->GetInputParameterDesc(i, &signatureDesc);
        debugOutputSignatures.push_back(signatureDesc);
    }
#endif

#if defined(_DEBUG) || defined(DEBUG)
    std::vector<D3D11_SHADER_INPUT_BIND_DESC> debugShaderParams;
#endif

    // Extract all shader input resources(cbuffer, tbuffer, sampler, texture, etc)
    for (uint32_t resourceIndex = 0; resourceIndex < shaderDesc.BoundResources; ++resourceIndex)
    {
        D3D11_SHADER_INPUT_BIND_DESC inputBindDesc;
        reflector->GetResourceBindingDesc(resourceIndex, &inputBindDesc);

#if defined(_DEBUG) || defined(DEBUG)
        debugShaderParams.push_back(inputBindDesc);
#endif

        std::string resourceName = inputBindDesc.Name;

        switch (inputBindDesc.Type)
        {
        case D3D_SIT_CBUFFER:
        case D3D_SIT_TBUFFER:
            do 
            {
                // HLSL compiler puts global "standalone" vars into a default cbuffer called $Globals
                bool isGlobalBuffer = resourceName.compare("$Globals") == 0;

                // HLSL compiler puts "uniform" arguments declared in function argument list into another default cbuffer called
                // $Params 
                bool isUniformBuffer = resourceName.compare("$Params") == 0;

                // Get cbuffer bind point
                const uint32_t cBufferIndex = inputBindDesc.BindPoint; 

                // Iterate vars defined in this cbuffer
                ID3D11ShaderReflectionConstantBuffer* cBuffer = reflector->GetConstantBufferByName(resourceName.c_str());
                D3D11_SHADER_BUFFER_DESC cBufferDesc;
                cBuffer->GetDesc(&cBufferDesc);

                // Record this cbuffer
                output.ParamMap.addCBufferDesc(cBufferIndex, cBufferDesc.Name, cBufferDesc.Size, cBufferDesc.Variables);
                
                for (uint32_t varIndex = 0; varIndex < cBufferDesc.Variables; ++varIndex)
                {
                    ID3D11ShaderReflectionVariable* var = cBuffer->GetVariableByIndex(varIndex);

                    D3D11_SHADER_VARIABLE_DESC varDesc;
                    var->GetDesc(&varDesc);

                    // HLSL compiler always drops unused cbuffers/tbuffers/textures/samplers.
                    // However if a cbuffer is partially used, those unused vars are kept.
                    //if (varDesc.uFlags & D3D_SVF_USED)
                    {
                        ShaderParamAllocation param;
                        param.BufferIndex = cBufferIndex;
                        param.ByteOffset  = varDesc.StartOffset;
                        param.ByteSize    = varDesc.Size;
                        output.ParamMap.addParam(varDesc.Name, param);
                    }
                }

            } while (0);

            break;

        case D3D_SIT_TEXTURE:
        case D3D_SIT_SAMPLER:
            do 
            {
                ShaderParamAllocation param;
                param.BufferIndex = 0;
                param.ByteOffset  = inputBindDesc.BindPoint;
                param.ByteSize    = inputBindDesc.BindCount;
                output.ParamMap.addParam(resourceName, param);
                if (inputBindDesc.Type == D3D_SIT_SAMPLER)
                {
                    output.NumTextureSamplers += inputBindDesc.BindCount;
                }
            } while (0);

            break;

        case D3D_SIT_STRUCTURED:
        case D3D_SIT_BYTEADDRESS:
            // TODO
            break;

        case D3D_SIT_UAV_RWTYPED:
        case D3D_SIT_UAV_RWSTRUCTURED:
        case D3D_SIT_UAV_RWBYTEADDRESS: 
        case D3D_SIT_UAV_RWSTRUCTURED_WITH_COUNTER: 
        case D3D_SIT_UAV_APPEND_STRUCTURED: 
        case D3D_SIT_UAV_CONSUME_STRUCTURED:
            // TODO
            break;

        default:
            break;
        }
    }

    output.generateHash();

    return true;
}

bool compileShader(const CompilerInput& input, CompilerOutput& output, const ShaderMacros& addtionalMacros, const std::string& srcContent)
{
    return compileShader(input, output, srcContent);
}

void testShaderCompiler2()
{
    const Path& shaderPath = FileOp::getDefaultPath<FileOp::ShaderResource>();
    FileList result = FileOp::listFiles(shaderPath, L"hlsl");
    auto shaderSrcFiles = result.getList();

    CompilerInput input;
    ShaderMacros macros;
    std::vector<CompilerOutput*> outputs;

    uint32_t debugFlag = 0;

#if defined(_DEBUG) || defined(DEBUG)
    debugFlag |= D3DCOMPILE_DEBUG;
    debugFlag |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

    for (auto srcFilename : shaderSrcFiles)
    {
        // Populate input by parsing source file name
        std::string srcContent;

        if (srcFilename.getNakedName().find(L"_VS") != std::wstring::npos)
        {
            input.SrcFileName = ws2s( srcFilename.getPathName() );
            input.CompilerTarget.setTarget(EShaderStage::VertexShader_SM5);
            input.EntryPointName = "VSMain";
            input.Flags.addFlag(debugFlag);

            CompilerOutput* output = new CompilerOutput();


            FileOp::readFile(input.SrcFileName, srcContent);

            compileShader(input, *output, macros, srcContent);

            outputs.push_back(output);
        }
        else if (srcFilename.getNakedName().find(L"_PS") != std::wstring::npos)
        {
            input.SrcFileName = ws2s( srcFilename.getPathName() );
            input.CompilerTarget.setTarget(EShaderStage::PixelShader_SM5);
            input.EntryPointName = "PSMain";
            input.Flags.addFlag(debugFlag);

            CompilerOutput* output = new CompilerOutput();

            FileOp::readFile(input.SrcFileName, srcContent);

            compileShader(input, *output, macros, srcContent);

            outputs.push_back(output);
        }
        else
        {
            // Ignored 
        }
    }


    for (auto output : outputs)
    {
        if (output)
        {
            delete output;
            output = nullptr;
        }
    }
}

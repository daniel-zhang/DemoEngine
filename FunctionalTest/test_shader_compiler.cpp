#include "catch.hpp"
#include "Core/Util.h"
#include <d3dcompiler.h>
#include <d3d11shader.h> // For shader reflection

#include "Core/ShaderLoader.h"
#include "Renderer/Renderer.h"


TEST_CASE("Shader Load/Compile/Reflection", "[Shader]") {

    SECTION("VertexShader")
    {
        std::string vsFilename = "TestVS.hlsl";
        std::string vsEntry = "VSMain";

        CompilerInput cInput;
        //cInput.Includes;
        //cInput.Macros;
        cInput.CompilerTarget.setTarget(EShaderStage::VertexShader_SM5);
        cInput.SrcFileName = vsFilename;
        cInput.EntryPointName = vsEntry;
#if defined(_DEBUG) || defined(DEBUG)
        cInput.Flags.addFlag(D3DCOMPILE_DEBUG);
        cInput.Flags.addFlag(D3DCOMPILE_SKIP_OPTIMIZATION);
#endif

        CompilerOutput cOutput;
        bool loadResult = ShaderLoader::getInstance()->load(cInput, cOutput);

        REQUIRE(true == loadResult);

        // Verify output
    }

    SECTION("PixelShader")
    {
        std::string psFilename = "TestPS.hlsl";
        std::string psEntry = "PSMain";

        CompilerInput cInput;
        //cInput.Includes;
        //cInput.Macros;
        cInput.CompilerTarget.setTarget(EShaderStage::PixelShader_SM5);
        cInput.SrcFileName = psFilename;
        cInput.EntryPointName = psEntry;
#if defined(_DEBUG) || defined(DEBUG)
        cInput.Flags.addFlag(D3DCOMPILE_DEBUG);
        cInput.Flags.addFlag(D3DCOMPILE_SKIP_OPTIMIZATION);
#endif

        CompilerOutput cOutput;
        bool loadResult = ShaderLoader::getInstance()->load(cInput, cOutput);

        REQUIRE(true == loadResult);

        /////////////////////////////////////////////////////////////
        // Verify shader parameter layout
        /////////////////////////////////////////////////////////////
        REQUIRE(cOutput.ByteCode.size() > 0);
        
        const std::unordered_map<std::string, ShaderParamAllocation>& paramMap = cOutput.ParamMap.getMap();

        std::string paramLayout = "Param Name, BindPoint, Offset, ByteSize\n";
        for (auto p : paramMap )
        {
            const std::string& name            = p.first;
            const ShaderParamAllocation& param = p.second;

            paramLayout += string_format("%s, %i, %i, %i\n", 
                name.c_str(), param.BufferIndex, param.ByteOffset, param.ByteSize);
        }

        REQUIRE(paramLayout.empty() == false);

    }

    SECTION("Dynamic Shader Linkage") 
    {
        std::wstring dir = FileOp::getDefaultPath<FileOp::ShaderResource>().toString();
        std::string srcFullname = ws2s(dir) + "/" + "GuiPS_DSL.hlsl";
        std::string sourceCode;
        FileOp::readFile(srcFullname, sourceCode);

        CompilerInput cInput;
        cInput.CompilerTarget.setTarget(EShaderStage::PixelShader_SM5);
        cInput.SrcFileName = srcFullname;
        cInput.EntryPointName = "PSMain";
        cInput.Flags.addFlag(D3DCOMPILE_DEBUG);
        cInput.Flags.addFlag(D3DCOMPILE_SKIP_OPTIMIZATION);

        CompilerOutput cOutput;

        bool result = compileShader(cInput, cOutput, sourceCode);
        if (!result)
        {
            std::string errStr = cOutput.getErrorString();
            show_error(wstring_format(L"%s", s2ws(errStr).c_str()));
        }

        REQUIRE(result == true);
        //////////////////////////////////////////////////////////////////////////////////////////
        /*
        Application knowledge
            

        Reflection information
            - 

        
        */
        //////////////////////////////////////////////////////////////////////////////////////////

        // initialize dynamic shader linkage
        Renderer::getInstance()->initialize(RendererSettings());
        ID3D11Device* dev = Renderer::getInstance()->getDevice();
        ID3D11DeviceContext* ctx = Renderer::getInstance()->getContext();

        // Linkage
        ID3D11ClassLinkage* classLinkage;
        dev->CreateClassLinkage(&classLinkage);

        // Shader
        ID3D11PixelShader* ps;
        const void* pCode = &cOutput.ByteCode[0];
        const uint32_t codeSize = cOutput.ByteCode.size();
        dev->CreatePixelShader(pCode, codeSize, classLinkage, &ps);

        // Class instances

        // Non-empty instance
        HRESULT hr;
        ID3D11ClassInstance* ciNormalClipOn = nullptr;
        hr = classLinkage->GetClassInstance("gNormalClipOn", 0, &ciNormalClipOn);

        // Empty counter part: buffer bindpoint 2, byte offset 48 
        ID3D11ClassInstance* ciDebugClipOn = nullptr;
        hr = classLinkage->CreateClassInstance("DebugClipOn", 0, 0, 0, 0, &ciDebugClipOn);

        // Non-empty instance
        ID3D11ClassInstance* ciNoTextureColor = nullptr;
        hr = classLinkage->GetClassInstance("gNoTextureColor", 0, &ciNoTextureColor);

        // Non-empty instance
        ID3D11ClassInstance* ciDisabledOn = nullptr;
        hr = classLinkage->GetClassInstance("gDisabledOn", 0, &ciDisabledOn);

        std::vector<ID3D11ClassInstance*> mDynamicCode;

        //mDynamicCode.push_back(ciNormalClipOn);
        mDynamicCode.push_back(ciDebugClipOn);

        mDynamicCode.push_back(ciNoTextureColor);

        mDynamicCode.push_back(ciDisabledOn);

        ctx->PSSetShader(ps, &mDynamicCode[0], mDynamicCode.size());

        safe_release(ciDebugClipOn);
        safe_release(ciDisabledOn);
        safe_release(ciNoTextureColor);
        safe_release(ciNormalClipOn);
        safe_release(classLinkage);
        safe_release(ps);
        Renderer::getInstance()->destroy();
    }
}


#pragma once

#include "RendererCommon.h"
#include "Shader.h"
#include "BaseShaders.h"
#include "GuiShaders.h"

class ShaderLibrary
{
public:
    static ShaderLibrary* getInstance()
    {
        static ShaderLibrary rl;
        return &rl;
    }

public:
    template<typename TShaderType>
    TShaderType* getShader()
    {
        // Get id of required shader
        const UniqueID& dstUID = TShaderType::id();

        // See if already created
        for (auto shader : mShaders)
        {
            if (shader->getUniqueId() == dstUID)
            {
                return (TShaderType*)shader;
            }
        }

        // Not created yet, create now
        TShaderType* shader = new TShaderType();
        shader->initialize();
        mShaders.push_back(shader);

        return shader;
    }

    template<typename TShaderType>
    TShaderType* getShader_RTTI()
    {
        // See if already created
        for (auto shader : mShaders)
        {
            TShaderType* result = dynamic_cast<TShaderType*>(shader);
            if (result)
                return result;
        }

        // Not created yet, create now
        TShaderType* shader = new TShaderType();
        shader->initialize();
        mShaders.push_back(shader);
        return shader;
    }

protected:
    ShaderLibrary() { }
    ShaderLibrary(const ShaderLibrary& );
    ShaderLibrary& operator=(const ShaderLibrary&);

protected:
    std::vector<IShader*> mShaders;
};
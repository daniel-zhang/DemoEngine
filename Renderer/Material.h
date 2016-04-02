#pragma once

#include "Core/Math.h"
#include "Core/Util.h"
#include "VertexDeclaration.h"
#include "Shader.h"

/*
Quick memo
1. Material can be applied to meshes of multiple types
2. To do that, a material maintains a vertex shader map 
3. For simplicity, a material only contains one pixel shader
*/

class Material
{
public:
    Material() :mPixelShader(nullptr){}

    virtual ~Material() {} 

    virtual void create() = 0;

    virtual void destroy() = 0;

    bool checkVertexDeclaration(VertexDeclaration* inVertexDeclaration)
    {
        return (mVertexShaderMap.find(inVertexDeclaration) != mVertexShaderMap.end());
    }

    VertexShader* getVertexShader(VertexDeclaration* inVertexDeclaration)
    {
        auto result = mVertexShaderMap.find(inVertexDeclaration);
        if (result != mVertexShaderMap.end())
        {
            return result->second;
        }
        else
            return nullptr;
    }

    PixelShader* getPixelShader() 
    { 
        return mPixelShader; 
    }

protected:
    std::unordered_map<VertexDeclaration*, VertexShader*> mVertexShaderMap;
    PixelShader* mPixelShader;
};

class PhongMaterial : public Material
{
public:
    PhongMaterial(){}
    ~PhongMaterial(){}

    virtual void create() override;

    virtual void destroy() override;

};
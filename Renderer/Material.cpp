#include "Material.h"
#include "ShaderLibrary.h"

void PhongMaterial::create()
{
    if (!mVertexShaderMap.empty() && mPixelShader != nullptr)
        return;

    VertexShader* vs = ShaderLibrary::getInstance()->getShader_RTTI<BaseVertexShader>();

    mVertexShaderMap.emplace(vs->getVertexDeclaration(), vs);

    mPixelShader = ShaderLibrary::getInstance()->getShader_RTTI<BasePixelShader>();
}

void PhongMaterial::destroy()
{
    mVertexShaderMap.clear();
    mPixelShader = nullptr;
}

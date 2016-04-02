#pragma once
#include "Core/Util.h"

// Prototypes

class Material;

class Mesh;

class VertexShader;
class PixelShader;
class Texture2D;

class ContentMgr
{
public:
    void listAnimations(std::vector<std::string>& resourceNames);
    void listTextures(std::vector<std::string>& resourceNames);
    void listMeshes(std::vector<std::string>& resourceNames);

    void loadTextures(); // WIC
    void loadMesh(std::string filename, Mesh& outMesh); // Inhouse format
    void importMesh(std::string filename, Mesh& outMesh); // FBX SDK

    void saveMesh();
    void saveScene();
};

class Mesh
{
public:
    struct MeshMaterialGroup
    {
        // Identity
        uint32_t Id;
        std::string Name;
        // Underlying data
        class VertexBuffer* pVBuffer
        uint32_t VertOffset;
        uint32_t VertNum;

        Material* pMaterial;
        class VertexDeclaration* getVertDecl();
    };

    void setMaterial(Material* inMat, uint32_t groupID)
    {
        inMat->checkAppliable(this);
        // ...
    }
    std::vector<MeshMaterialGroup>& getSubMeshes();
    
public:
    // Assume all submeshes share the same vert declaration
    VertexDeclaration* getVertexDeclartion();

protected:
    std::vector<MeshMaterialGroup> mSubMeshes;
    std::vector<Material*> mMaterials;
};

class Material
{
public:
    VertexShader* selectVS(VertexDeclaration* vd);
    PixelShader* getPS();
public:
    std::vector<Properties> mProperties;

protected:
    std::vector<class VertexShaderTypes*> mVertexShadersTypes;
    class PixelShaderType* mPixelShaderType;
    std::vector<std::string> mTextureNames;

    void create()
    {
        for (auto st : mVertexShadersTypes)
        {
            Renderer::getVertexShaderByType(st);
        }

        Renderer::getPixelShaderByType(mPixelShaderType);

        for (auto tn : mTextureNames)
        {
            Renderer::getTextureByName(tn);
        }
    }

    std::vector<VertexShader*> mVertexShaders;
    PixelShader* mPixelShaders;
    std::vector<Texture2D*> mTextures;
};

class Renderer
{
public:
    void drawMesh(Mesh* inMesh)
    {
        inMesh->commitStreamData();
        for (auto subM : inMesh->getSubMeshes())
        {
            Material* currMat = subM.pMaterial;
            VertexShader* vs = currMat->selectVS(subM.getVertDecl());
            vs->setInputLayout();
            vs->setStream(subM.getStream());
            vs->collectCBufferData(mesh, cam, viewport);
            vs->bind();

            updateRenderStatesAccordingToProperties(currMat);

            PixelShader* ps = currMat->getPS();
            ps->collectCBufferData(scene.getLights());
            ps->collectTextures(currMat);
            ps->setSamplers();
            ps->bind();

            draw();
        }

    }

};
#pragma once

#include "Material.h"
#include "GeometryBuffer.h"
/*
A simple test mesh
*/

struct MaterialMeshGroup
{
    uint32_t GroupId;
    Material* Mat;

    uint32_t IndexOffset;
    uint32_t IndexNum;

    uint32_t VertexOffset;
    uint32_t VertexNum;
};

class Mesh
{
public:
    Mesh() : 
        mVertexDeclaration(nullptr),
        mVertexBuffer(nullptr),
        mIndexBuffer(nullptr)
    {

    }

    ~Mesh()
    {
        if (mVertexBuffer)
        {
            delete mVertexBuffer;
            mVertexBuffer = nullptr;
        }
        if (mIndexBuffer)
        {
            delete mIndexBuffer;
            mIndexBuffer = nullptr;
        }
    }

    bool applyMaterial(Material* material, uint32_t groupId)
    {
        if (material->checkVertexDeclaration(mVertexDeclaration))
        {
            mData[groupId].Mat = material;
            return true;
        }
        return false;
    }

    std::vector<MaterialMeshGroup>& getRenderData(){ return mData; }
    
    VertexDeclaration* getVertexDeclaration()
    {
        return mVertexDeclaration;
    }

    void setStreamData(GeometryBuffer* vb, GeometryBuffer* ib)
    {
        mVertexBuffer = vb;
        mIndexBuffer = ib;
    }

protected:
    VertexDeclaration* mVertexDeclaration;
    GeometryBuffer* mVertexBuffer;
    GeometryBuffer* mIndexBuffer;

    std::vector<MaterialMeshGroup> mData;
};
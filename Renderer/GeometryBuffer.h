#pragma once

#include "RendererCommon.h"

/*
Covers all usages for vertex/index buffer
*/
class GeometryBuffer
{
public:
    enum EGeometryBufferUsage
    {
        ImmutableVertexBuffer,
        ImmutableIndexBuffer,
        DynamicVertexBuffer,
        DynamicIndexBuffer,
    };

public:
    GeometryBuffer() :
        mBufferUsage(DynamicVertexBuffer),
        mBufferResource(nullptr),
        mBufferByteSize(0),
        mIsInitialized(false)
    {}

    ~GeometryBuffer(){ destroy(); }

    void initialize(EGeometryBufferUsage usage, uint32_t bufferByteSize, uint8_t* initialData = nullptr);

    void destroy();

    // Only used by dynamic resources
    void resize(uint32_t newByteSize);

    // Only used by dynamic resources
    uint8_t* lock();

    // Only used by dynamic resources
    ID3D11Buffer* unlock();

    bool isInitialized() const { return mIsInitialized; }

    ID3D11Buffer* getResource() { return mBufferResource; }

    ID3D11Buffer** getResourcePtr() { return &mBufferResource; }

    uint32_t getBufferByteSize() const { return mBufferByteSize; }

    EGeometryBufferUsage getUsage() const { return mBufferUsage; }

    bool isDynamicResource() const { return (mBufferUsage == EGeometryBufferUsage::DynamicIndexBuffer || mBufferUsage == EGeometryBufferUsage::DynamicVertexBuffer); }

protected:
    bool mIsInitialized;

    EGeometryBufferUsage mBufferUsage;

    uint32_t mBufferByteSize;

    ID3D11Buffer* mBufferResource;
};
#pragma once

#include "RendererCommon.h"

/*
ConstBuffer 
    - maintains a system memory cache to minimize CPU to GPU data transfer operations
    - is agnostic of incoming data type
*/

class ConstBuffer 
{
public:

    ConstBuffer();

    ~ConstBuffer();

public:
    void initialize(uint32_t bufferByteSize);

    void destroy();

    void commit();

    void update(uint32_t byteOffset, void* dataPtr, uint32_t dataByteSize);

    ID3D11Buffer* getResource() { return mBufferResource; }

    ID3D11Buffer** getResourcePtr() { return &mBufferResource; }

    bool isInitialized() const { return mBufferResource != nullptr; }

protected:
    ID3D11Buffer* mBufferResource;

    uint32_t mBufferByteSize;

    uint8_t* mCache;

    bool mIsCacheDirty;
};
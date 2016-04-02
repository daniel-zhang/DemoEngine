#include "ConstBuffer.h"
#include "Renderer.h"

ConstBuffer::ConstBuffer() :
    mBufferResource(nullptr),
    mBufferByteSize(0),
    mCache(nullptr),
    mIsCacheDirty(false)
{
}

ConstBuffer::~ConstBuffer()
{
    destroy();
}

void ConstBuffer::initialize(uint32_t bufferByteSize)
{
    if (isInitialized())
        return;

    mBufferByteSize = bufferByteSize;

    check(mBufferByteSize != 0);
    check(mBufferByteSize % 16 == 0); // 16 byte alignment

    // Create cache
    mCache = new uint8_t[mBufferByteSize];
    memset(mCache, 0, mBufferByteSize);

    // Create buffer resource
    D3D11_BUFFER_DESC desc;
    desc.ByteWidth           = mBufferByteSize;
    desc.Usage               = D3D11_USAGE_DYNAMIC;
    desc.BindFlags           = D3D11_BIND_CONSTANT_BUFFER;
    desc.CPUAccessFlags      = D3D11_CPU_ACCESS_WRITE;
    desc.MiscFlags           = 0;
    desc.StructureByteStride = 0;

    ID3D11Device* dev = Renderer::getInstance()->getDevice();
    check_hr(dev->CreateBuffer(&desc, nullptr, &mBufferResource));
}

void ConstBuffer::destroy()
{
    if (mCache)
    {
        delete[] mCache;
        mCache = nullptr;
    }

    mBufferByteSize = 0;

    mIsCacheDirty = false;

    safe_release(mBufferResource);
}

void ConstBuffer::commit()
{
    // Cache is clean, nothing to commit
    if (mIsCacheDirty == false) 
        return;

    D3D11_MAPPED_SUBRESOURCE subData;

    ID3D11DeviceContext* ctx = Renderer::getInstance()->getContext();

    ctx->Map(mBufferResource, 0, D3D11_MAP_WRITE_DISCARD, 0, &subData);

    memcpy(subData.pData, mCache, mBufferByteSize);

    ctx->Unmap(mBufferResource, 0);

    // Mark cache clean
    mIsCacheDirty = false;
}

void ConstBuffer::update(uint32_t byteOffset, void* dataPtr, uint32_t dataByteSize)
{
    check(byteOffset + dataByteSize <= mBufferByteSize);

    memcpy(mCache + byteOffset, dataPtr, dataByteSize);

    // Mark cache dirty
    mIsCacheDirty = true;
}



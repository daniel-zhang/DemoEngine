#include "GeometryBuffer.h"
#include "Renderer.h"

void GeometryBuffer::initialize(EGeometryBufferUsage usage, uint32_t bufferByteSize, uint8_t* initialData /*= nullptr*/)
{
    if (mIsInitialized)
        return ;

    mBufferByteSize = bufferByteSize;
    mBufferUsage = usage;

    D3D11_BUFFER_DESC desc;
    switch (mBufferUsage)
    {
    case GeometryBuffer::ImmutableVertexBuffer:
        desc.Usage               = D3D11_USAGE_IMMUTABLE;
        desc.BindFlags           = D3D11_BIND_VERTEX_BUFFER;
        desc.ByteWidth           = mBufferByteSize;
        desc.CPUAccessFlags      = 0;
        desc.MiscFlags           = 0;
        desc.StructureByteStride = 0;
        break;

    case GeometryBuffer::ImmutableIndexBuffer:
        desc.Usage               = D3D11_USAGE_IMMUTABLE;
        desc.BindFlags           = D3D11_BIND_INDEX_BUFFER;
        desc.ByteWidth           = mBufferByteSize;
        desc.CPUAccessFlags      = 0;
        desc.MiscFlags           = 0;
        desc.StructureByteStride = 0;

        break;

    case GeometryBuffer::DynamicVertexBuffer:
        desc.Usage               = D3D11_USAGE_DYNAMIC;
        desc.BindFlags           = D3D11_BIND_VERTEX_BUFFER;
        desc.ByteWidth           = mBufferByteSize;
        desc.CPUAccessFlags      = D3D11_CPU_ACCESS_WRITE;
        desc.MiscFlags           = 0;
        desc.StructureByteStride = 0;
        break;

    case GeometryBuffer::DynamicIndexBuffer:
        desc.Usage               = D3D11_USAGE_DYNAMIC;
        desc.BindFlags           = D3D11_BIND_INDEX_BUFFER;
        desc.ByteWidth           = mBufferByteSize;
        desc.CPUAccessFlags      = D3D11_CPU_ACCESS_WRITE;
        desc.MiscFlags           = 0;
        desc.StructureByteStride = 0;
        break;

    default:
        break;
    }

    ID3D11Device* dev = Renderer::getInstance()->getDevice();

    if (initialData) 
    {
        D3D11_SUBRESOURCE_DATA srd;
        srd.pSysMem = initialData;
        srd.SysMemSlicePitch = mBufferByteSize;

        check_hr(dev->CreateBuffer(&desc, &srd, &mBufferResource));
    }
    else
    {
        // Only dynamic resources are allowed to created with empty init data
        check(isDynamicResource() == true);
        check_hr(dev->CreateBuffer(&desc, nullptr, &mBufferResource));
    }

    mIsInitialized = true;
}

void GeometryBuffer::destroy()
{
    mBufferByteSize = 0;

    mIsInitialized = false;

    mBufferUsage = EGeometryBufferUsage::DynamicVertexBuffer;

    safe_release(mBufferResource);
}

void GeometryBuffer::resize(uint32_t newByteSize)
{
    check(isDynamicResource());

    EGeometryBufferUsage usage = mBufferUsage;

    destroy();

    initialize(usage, newByteSize);
}

uint8_t* GeometryBuffer::lock()
{
    check(isDynamicResource());

    ID3D11DeviceContext* ctx = Renderer::getInstance()->getContext();

    D3D11_MAPPED_SUBRESOURCE msr;

    ctx->Map(mBufferResource, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

    return static_cast<uint8_t*>(msr.pData);
}

ID3D11Buffer* GeometryBuffer::unlock()
{
    check(isDynamicResource());

    ID3D11DeviceContext* ctx = Renderer::getInstance()->getContext();

    ctx->Unmap(mBufferResource, 0);

    return mBufferResource;
}


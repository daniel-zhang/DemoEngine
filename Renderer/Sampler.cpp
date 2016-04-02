#include "Sampler.h"
#include "Renderer.h"

Sampler::Sampler() :
    Sampler_MinMagMipPoint(nullptr),
    Sampler_MinMagMipPoint_Tiling(nullptr),
    Sampler_MinMagPointMipLinear(nullptr),
    Sampler_MinPointMagLinearMipPoint(nullptr),
    Sampler_MinPointMagMipLinear(nullptr),
    Sampler_MinLinearMagMipPoint(nullptr),
    Sampler_MinLinearMagPointMipLinear(nullptr),
    Sampler_MinMagLinearMipPoint(nullptr),
    Sampler_MinMagMipLinear(nullptr),
    Sampler_Anisotropic(nullptr),
    Sampler_Anisotropic_Tiling(nullptr),

    mIsInitialized(false)
{
}

bool Sampler::isInitialized() const
{
    return mIsInitialized;
}

void Sampler::initialize()
{
    if (mIsInitialized)
        return;

    ID3D11Device* dev = Renderer::getInstance()->getDevice();
    ID3D11DeviceContext* ctx = Renderer::getInstance()->getContext();

    D3D11_SAMPLER_DESC desc = CD3D11_SAMPLER_DESC(CD3D11_DEFAULT());

    desc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
    check_hr(dev->CreateSamplerState(&desc, &Sampler_MinMagMipPoint));

    desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    check_hr(dev->CreateSamplerState(&desc, &Sampler_MinMagMipPoint_Tiling));
    desc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
    desc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;

    desc.Filter = D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR;
    check_hr(dev->CreateSamplerState(&desc, &Sampler_MinMagPointMipLinear));

    desc.Filter = D3D11_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT;
    check_hr(dev->CreateSamplerState(&desc, &Sampler_MinPointMagLinearMipPoint));

    desc.Filter = D3D11_FILTER_MIN_POINT_MAG_MIP_LINEAR;
    check_hr(dev->CreateSamplerState(&desc, &Sampler_MinPointMagMipLinear));

    desc.Filter = D3D11_FILTER_MIN_LINEAR_MAG_MIP_POINT;
    check_hr(dev->CreateSamplerState(&desc, &Sampler_MinLinearMagMipPoint));

    desc.Filter = D3D11_FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR;
    check_hr(dev->CreateSamplerState(&desc, &Sampler_MinLinearMagPointMipLinear));

    desc.Filter = D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
    check_hr(dev->CreateSamplerState(&desc, &Sampler_MinMagLinearMipPoint));

    desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    check_hr(dev->CreateSamplerState(&desc, &Sampler_MinMagMipLinear));

    desc.Filter = D3D11_FILTER_ANISOTROPIC;
    check_hr(dev->CreateSamplerState(&desc, &Sampler_Anisotropic));

    desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    check_hr(dev->CreateSamplerState(&desc, &Sampler_Anisotropic_Tiling));
    desc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
    desc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;

    mIsInitialized = true;
}

void Sampler::destroy()
{
    safe_release(Sampler_MinMagMipPoint);
    safe_release(Sampler_MinMagMipPoint_Tiling);
    safe_release(Sampler_MinMagPointMipLinear);
    safe_release(Sampler_MinPointMagLinearMipPoint);
    safe_release(Sampler_MinPointMagMipLinear);
    safe_release(Sampler_MinLinearMagMipPoint);
    safe_release(Sampler_MinLinearMagPointMipLinear);
    safe_release(Sampler_MinMagLinearMipPoint);
    safe_release(Sampler_MinMagMipLinear);
    safe_release(Sampler_Anisotropic);
    safe_release(Sampler_Anisotropic_Tiling);
}


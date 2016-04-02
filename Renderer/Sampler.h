#pragma once

#include "RendererCommon.h"

class Sampler 
{
public:
    static Sampler* getInstance()
    {
        static Sampler instance;
        return &instance;
    }

public:
    ~Sampler() { destroy(); }

    bool isInitialized() const;

    void initialize();

    void destroy();

public:
    ID3D11SamplerState* Sampler_MinMagMipPoint;
    ID3D11SamplerState* Sampler_MinMagMipPoint_Tiling;

    ID3D11SamplerState* Sampler_MinMagPointMipLinear;

    ID3D11SamplerState* Sampler_MinPointMagLinearMipPoint;

    ID3D11SamplerState* Sampler_MinPointMagMipLinear;

    ID3D11SamplerState* Sampler_MinLinearMagMipPoint;

    ID3D11SamplerState* Sampler_MinLinearMagPointMipLinear;

    ID3D11SamplerState* Sampler_MinMagLinearMipPoint;

    ID3D11SamplerState* Sampler_MinMagMipLinear;

    ID3D11SamplerState* Sampler_Anisotropic;
    ID3D11SamplerState* Sampler_Anisotropic_Tiling;

private:
    Sampler();
    Sampler(const Sampler&);
    Sampler& operator=(const Sampler&);

private:
    bool mIsInitialized;
};
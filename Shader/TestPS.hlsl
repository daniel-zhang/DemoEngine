cbuffer Buffer1
{
    float gAmbientFactor;
    float4 gMaterialColor;
    float3 gLightDir;
    float gUnusedVarInUsedCBuffer;
};

cbuffer Buffer2
{
    float gUnusedCBufferVar;
};

float gGlobalParameter;

Texture2D gTexture0;

Texture2D gTexture1;

Texture2DArray gTextureArray;

Texture2D gArrayOfTexture[3];

SamplerState gSampler0;

SamplerState gSampler1;

struct PS_INPUT
{
    float3 vNormal : NORMAL;
    float2 vTexcoord : TEXCOORD0;
};

float4 evaluateColor(PS_INPUT pin)
{
    float4 outColor = float4(pin.vNormal, 1.f);

    outColor *= gTexture0.Sample(gSampler0, pin.vTexcoord);

    if (outColor.a < 0.05f)
    {
        discard;
    }

    return outColor;
}

interface IBase
{
    float4 foo(PS_INPUT pin);
};

IBase iBase;

class Bar : IBase
{
    float4 foo(PS_INPUT pin)
    {
        float4 outColor = float4(pin.vNormal, 1.f);

        outColor *= gTexture0.Sample(gSampler0, pin.vTexcoord);

        if (outColor.a < 0.05f)
        {
            // 'discard' does not work within a interface method
            clip(-1);
        }

        return outColor;
    }
};

float4 PSMain( PS_INPUT pin, uniform float uniformArg1, uniform float uniformArg2) : SV_TARGET
{
    // Make use of textures/samplers
    float4 outColor = gTexture0.Sample(gSampler0, pin.vTexcoord);

    outColor *= gTexture1.Sample(gSampler1, pin.vTexcoord);

    [unroll]
    for (int i = 0; i < 3; ++i)
    {
        outColor += gTextureArray.Sample(gSampler0, float3(pin.vTexcoord.x, pin.vTexcoord.y, i)); 
        outColor += gArrayOfTexture[i].Sample(gSampler0, pin.vTexcoord);
    }

    // Make use of $Globals and $Params
    outColor *= (uniformArg1 + uniformArg2 + gGlobalParameter);

    // Make use of cbuffers
    outColor += gMaterialColor;

    float fLighting = saturate( dot( gLightDir, pin.vNormal ) );

    fLighting = max( fLighting, gAmbientFactor );

    outColor = evaluateColor(pin);

    outColor = iBase.foo(pin);
    
    return outColor* fLighting;
}

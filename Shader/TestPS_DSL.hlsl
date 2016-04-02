interface iGetColor 
{
    float3 getColor(float3 value);
};

interface iDrawEffect
{
    float3 applyEffect(float3 value);
};

class FontColor : iGetColor 
{
    float3 getColor(float3 value) 
    {
        return  value * 0.1f;
    }
};

class BorderColor : iGetColor 
{
    float3 mColor; 
    float3 mFactor;

    float3 getColor(float3 value) 
    {
        return mFactor * mColor * value; 
    }
};

class NormalEffect: iDrawEffect
{
    float mColor;
    float3 applyEffect(float3 value)
    {
        return value * mColor;
    }
};

class DisabledEffect: iDrawEffect
{
    float mColor;
    float3 applyEffect(float3 value)
    {
        return value * mColor;
    }
};

iGetColor gGetColorInterface;
iDrawEffect gDrawEffectInterface;
float4 gColor;

cbuffer BufferForInterfance
{
    float4 gUnused;
    FontColor gFontColorGetter;
    BorderColor gBorderColorGetter;
    NormalEffect gNormalEffect;
    DisabledEffect gDisabledEffect;
};

struct PS_INPUT
{
    float3 vNormal : NORMAL;
    float2 vTexcoord : TEXCOORD0;
};

float4 PSMain( PS_INPUT pin) : SV_TARGET
{
    float4 outColor = float4(1,1,1,1);
    outColor = float4(gGetColorInterface.getColor(pin.vNormal), 1);
    outColor = float4(gDrawEffectInterface.applyEffect(outColor.rgb), 1);
    outColor *= gColor;

    return outColor;
}

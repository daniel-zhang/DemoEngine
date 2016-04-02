#include "GuiCommon.hlsl"
/*
struct PixelInput
{
    float4 PosH    : SV_POSITION;
    float2 UV      : TEXCOORD;
    float4 Color   : COLOR;
};*/

Texture2D gGuiTexture;

SamplerState gGuiSampler;

//float4 PSMain(PixelInput pin) : SV_Target
float4 PSMain(VertexOutput pin) : SV_Target
{
    float4 outColor = gGuiTexture.Sample(gGuiSampler, pin.UV.xy);

    outColor = outColor * pin.Color;

    // gamma correct
    outColor.rgb = pow(outColor.rgb, 1.0f / 2.2f);

    return outColor;
}

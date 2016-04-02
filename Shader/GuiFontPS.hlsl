#include "GuiCommon.hlsl"

/*
struct PixelInput
{
    float4 PosH    : SV_POSITION;
    float2 UV      : TEXCOORD;
    float4 Color   : COLOR;
};
*/

Texture2D gFontTexture;

SamplerState gFontSampler;

//float4 PSMain(PixelInput pin) : SV_Target
float4 PSMain(VertexOutput pin) : SV_Target
{
    float4 outColor = pin.Color;

    outColor.a *= gFontTexture.Sample(gFontSampler, pin.UV.xy).a;

    // gamma correct
    outColor.rgb = pow(outColor.rgb, 1.0f / 2.2f);

    return outColor;
}

#include "GuiCommon.hlsl"

cbuffer ShaderParam 
{ 
    float4 gGuiParam;
};

Texture1D gGuiTexture;

SamplerState gGuiSampler;

float4 PSMain(VertexOutput pin) : SV_Target
{
    float Width = gGuiParam.x;
    float Radius = gGuiParam.y;

    float2 StartPos = pin.UV.xy;
    float2 EndPos = pin.UV.zw;

    float2 Diff = float2(StartPos.y - EndPos.y, EndPos.x - StartPos.x);

    float K = 2 / ((2 * Radius + Width)*sqrt(dot(Diff, Diff)));

    float3 E0 = K*float3(StartPos.y - EndPos.y, EndPos.x - StartPos.x, (StartPos.x*EndPos.y - EndPos.x*StartPos.y));
        E0.z += 1;

    float3 E1 = K*float3(EndPos.y - StartPos.y, StartPos.x - EndPos.x, (EndPos.x*StartPos.y - StartPos.x*EndPos.y));
        E1.z += 1;

    float3 Pos = float3(pin.PosH.xy, 1);

    float2 Distance = float2(dot(E0, Pos), dot(E1, Pos));

    if (any(Distance < 0))
    {
        // using discard instead of clip because
        // apparently clipped pixels are written into the stencil buffer but discards are not
        discard;
    }

    float Index = min(Distance.x, Distance.y);

    const float HalfPixelOffset = 1 / 32.f;

    float4 outColor = pin.Color;

    //outColor.a *= gGuiTexture.Sample(gGuiSampler, float2(Index - HalfPixelOffset, 0)).x;
    outColor.a *= gGuiTexture.Sample(gGuiSampler, Index - HalfPixelOffset).x;

    // Simple alpha test to help with overlapping segments
    if (outColor.a < 0.05f)
    {
        discard;
    }

    // gamma correct
    outColor.rgb = pow(outColor.rgb, 1.0f / 2.2f);

    return outColor;
}

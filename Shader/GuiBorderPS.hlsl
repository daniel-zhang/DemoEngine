#include "GuiCommon.hlsl"

cbuffer CustomTilingFactors 
{ 
    float4 gGuiParam;
};

Texture2D gGuiTexture;

SamplerState gGuiSampler;

float4 PSMain(VertexOutput pin) : SV_Target
{
    // Manipulate uv to do custom tiling
    float left   = gGuiParam.x;
    float top    = gGuiParam.y;
    float right  = gGuiParam.z;
    float bottom = gGuiParam.w;

    float tileU = pin.UV.z;
    float tileV = pin.UV.w;

    float2 targetUV = pin.UV.xy;

    if (tileU > 1.f)
    {
        targetUV.x = tileU * (pin.UV.x - left) / (right - left);
        targetUV.x = lerp(left, right, frac(targetUV.x));
    }
    else if (tileV > 1.f)
    {
        targetUV.y = tileV * (pin.UV.y - top) / (bottom - top);
        targetUV.y = lerp(top, bottom, frac(targetUV.y));
    }
    
    float4 outColor = gGuiTexture.Sample(gGuiSampler, targetUV);

    //float4 outColor = gGuiTexture.Sample(gGuiSampler, pin.UV.xy*pin.UV.zw);

    outColor = outColor * pin.Color;

    // gamma correct
    outColor.rgb = pow(outColor.rgb, 1.0f / 2.2f);

    return outColor;
}


#include "GuiCommon.hlsl"

/*
struct VertexInput
{
    float3 PosL     : POSITION;
    float2 UV       : TEXCOORD;
    float4 Color    : COLOR;
};

struct VertexOutput
{
    float4 PosH     : SV_POSITION;
    float2 UV       : TEXCOORD;
    float4 Color    : COLOR;
};
*/

cbuffer PerFrame
{
    float4x4 gView;
    float4x4 gProj;
};

VertexOutput VSMain(VertexInput vin)
{
    VertexOutput vout;

    float4x4 vp = mul(gProj, gView);

    vout.PosH     = mul(vp, float4(vin.PosL, 1.f) );
    vout.UV       = vin.UV;
    vout.Color    = vin.Color;
    vout.ClipRect = vin.ClipRect;

    return vout;
}

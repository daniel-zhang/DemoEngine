
#include "GuiCommon.hlsl"
/*
struct PixelInput
{
    float4 PosH    : SV_POSITION;
    float2 UV      : TEXCOORD;
    float4 Color   : COLOR;
};
*/

//float4 PSMain(PixelInput pin) : SV_Target
float4 PSMain(VertexOutput pin) : SV_Target
{
    return pin.Color;
}

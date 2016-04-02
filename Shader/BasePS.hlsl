
struct PixelInput
{
    float4 PosH    : SV_POSITION;
    float4 PosW    : POSITION;
    float3 NormalW : NORMAL;
    float2 Tex     : TEXCOORD;
    float4 Color   : COLOR;
};

float4 PSMain(PixelInput pin) : SV_Target
{
    return pin.Color;
}

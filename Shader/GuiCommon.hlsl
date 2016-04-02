
struct VertexInput
{
    float3 PosL     : POSITION;
    float4 UV       : TEXCOORD;
    float4 ClipRect : CLIPRECT;
    float4 Color    : COLOR;
};

struct VertexOutput
{
    float4 PosH     : SV_POSITION;
    float4 UV       : TEXCOORD;
    float4 ClipRect : CLIPRECT;
    float4 Color    : COLOR;
};

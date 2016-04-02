
cbuffer PerFrame
{
    float4x4 gView;
    float4x4 gProj;
};
cbuffer PerObject
{
    float4x4 gWorld;
    float4x4 gWorld_InverseTranspose;
};

struct VertexIn
{
    float4 PosL    : POSITION;
    float3 NormalL : NORMAL;
    float2 Tex     : TEXCOORD;
    float3 TangentL: TANGENT;
    float4 Color   : COLOR;
};

struct VertexOut
{
    float4 PosH    : SV_POSITION;
    float4 PosW    : POSITION;
    float3 NormalW : NORMAL;
    float2 Tex     : TEXCOORD;
    float4 Color   : COLOR;
};

VertexOut VSMain(VertexIn vin)
{
    VertexOut vout;

    /*
    float4x4 final = mul(mul(gProj, gView), gWorld);
    vout.PosW = mul(final, vin.PosL);
    */
    vout.PosW = mul(gWorld, vin.PosL);
    vout.PosW = mul(gView, vout.PosW);
    vout.PosW = mul(gProj, vout.PosW);

    vout.NormalW = vin.NormalL;
    vout.Tex = vin.Tex;
    vout.Color = vin.Color;

    vout.PosH = vout.PosW;

    return vout;
}

#include "GuiCommon.hlsl"

cbuffer ShaderParam 
{ 
    float4 gGuiParam;
};

Texture1D gLineFilter;

Texture2D gGuiTexture;

SamplerState gGuiSampler;

// Interfaces
interface IClip
{
    void clipUIRect(VertexOutput pin);

    float4 debugClip(VertexOutput pin, float4 inColor);
};

interface IColor
{
    float4 evaluate(VertexOutput pin);
};

interface IDisabledEffect
{
    float4 makeGrayish(float4 inColor);
};

// Declare interface pointers
IClip iClip;
IColor iColor;
IDisabledEffect iDisabledEffect;

// Interface implementations
class NormalClipOn : IClip
{
    //float mPlaceHolder;
    void clipUIRect(VertexOutput pin)
    {
        clip( float4( 
            pin.PosH.x      - pin.ClipRect.x, 
            pin.ClipRect.z  - pin.PosH.x, 
            pin.PosH.y      - pin.ClipRect.y, 
            pin.ClipRect.w  - pin.PosH.y ) );
    }

    float4 debugClip(VertexOutput pin, float4 inColor)
    {
        return inColor;
    }
};

class DebugClipOn : IClip
{
    //float mPlaceHolder;
    void clipUIRect(VertexOutput pin) {}

    float4 debugClip(VertexOutput pin, float4 inColor)
    {
        float4 Diff = float4( pin.PosH.x - pin.ClipRect.x, pin.ClipRect.z - pin.PosH.x, pin.PosH.y - pin.ClipRect.y, pin.ClipRect.w - pin.PosH.y );
        if( Diff.x < 0 || Diff.y < 0 || Diff.z < 0 || Diff.w < 0 )
        {
            // Shows red where any pixels discarded because part of the element was clipped
            inColor *= float4(1,0,0,1) ;
        }
        return inColor;
    }
};

class NoTextureColor : IColor
{
    //float mPlaceHolder;
    float4 evaluate(VertexOutput pin)
    {
        return pin.Color;
    }
};

class DefaultColor : IColor
{
    //float mPlaceHolder;
    float4 evaluate(VertexOutput pin)
    {
        float4 outColor = pin.Color;
        outColor *= gGuiTexture.Sample(gGuiSampler, pin.UV.xy);
        return outColor;
    }
};

class FontColor : IColor
{
    //float mPlaceHolder;
    float4 evaluate(VertexOutput pin)
    {
        float4 outColor = pin.Color;
        outColor.a *= gGuiTexture.Sample(gGuiSampler, pin.UV.xy).a;
        return outColor;
    }
};

class LineColor : IColor
{
    //float mPlaceHolder;
    float4 evaluate(VertexOutput pin)
    {
        float4 outColor = pin.Color;

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
            clip(-1);
        }

        float Index = min(Distance.x, Distance.y);

        const float HalfPixelOffset = 1 / 32.f;

        outColor.a *= gLineFilter.Sample(gGuiSampler, Index - HalfPixelOffset).x;

        // Simple alpha test to help with overlapping segments
        if (outColor.a < 0.05f)
        {
            clip(-1);
        }

        return outColor;
    }
};

class BorderColor : IColor
{
    //float mPlaceHolder;
    float4 evaluate(VertexOutput pin)
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

    outColor *= pin.Color;

    return outColor;
    }
};

class DisabledOn : IDisabledEffect
{
    //float mPlaceHolder;
    float4 makeGrayish(float4 inColor)
    {
        //desaturate
        float3 LumCoeffs = float3( 0.3, 0.59, .11 );
        float Lum = dot( LumCoeffs, inColor.rgb );
        inColor.rgb = lerp( inColor.rgb, float3(Lum,Lum,Lum), .8 );
        
        float3 Grayish = {.1, .1, .1};

        inColor.rgb = lerp( inColor.rgb, Grayish, clamp( distance( inColor.rgb, Grayish ), 0, .8)  );

        return inColor;
    }
};

class DisabledOff : IDisabledEffect
{
    //float mPlaceHolder;
    float4 makeGrayish(float4 inColor)
    {
        return inColor;
    }
};

/*
cbuffer Instances
{
    NormalClipOn gNormalClipOn;
    DebugClipOn gDebugClipOn;
    NoTextureColor gNoTextureColor;
    DefaultColor gDefaultColor;
    FontColor gFontColor;
    LineColor gLineColor;
    BorderColor gBorderColor;
    DisabledOn gDisabledOn;
    DisabledOff gDisabledOff;
};
*/

float4 PSMain(VertexOutput pin) : SV_Target
{
    //pin.ClipRect = float4(0, 0, 1000, 1000);
    // Clip: enable or not
    iClip.clipUIRect(pin);

    float4 outColor;

    // Evaluate color: 
    // No texture
    // Default texture
    // Font
    // Border
    // Line

    outColor = iColor.evaluate(pin);

    // Disabled effect or not
    outColor = iDisabledEffect.makeGrayish(outColor);

    // gamma correct
    outColor.rgb = pow(abs(outColor.rgb), 1.0f / 2.2f);

    // DebugClip: on or off 
    iClip.debugClip(pin, outColor);

    return outColor;
}
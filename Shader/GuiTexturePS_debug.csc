   serialization::archive                                                                     J   C:/workspace/Win32Playground/DemoEngine/Debug/../Shader//GuiTexturePS.hlsl   PSMain                                     gGuiSampler                    gGuiTexture                         A  DXBCBыйЌWmіЈ   A     8       а  №    RDEFШ              <    џџ     RD11<          (   $          |                                         џџџџ          gGuiSampler gGuiTexture Microsoft (R) HLSL Shader Compiler 10.0.10011.16384 ISGN         h                    t                   }                                       SV_POSITION TEXCOORD CLIPRECT COLOR OSGN,                               SV_Target ЋЋSHEX  P   F   j Z   `     X  p     UU  b 2    b ђ    e  ђ      h     E  Т  CU ђ      F    F~      `     8  ђ      F     F    /  r      F     8  
r      F     @  /Кш>/Кш>/Кш>      r      F     6  r      F     6        :      >  STAT                                                                                                                                                SPDB >  Microsoft C/C++ MSF 7.00
DS                                                                                                                                                                                                                                                                                                                                                                                                                                                                                              Рџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџ8  №џџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџ       <       џџџџ                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                         .1WРV   еэzіBCArr                          мQ3                                                                                                                                                                                                                                                                                                                                                                                                                                                                    ler, pin.UV.xy);

    outColor = outColor * pin.Color;

    // gamma correct
    outColor.rgb = pow(outColor.rgb, 1.0f / 2.2f);

    return outColor;
}
 
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
    C   B            Ђ           ЦZ  M
 eв Т A$ a o 1ћ ЩО  ШQ Ю7  9Ю                                                                                                                                                                                                                                                                                                                                                                                                                                                                                #include "GuiCommon.hlsl"
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
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      
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
                                                                                                                                                                                                ўяўя   Ю   C:\workspace\Win32Playground\DemoEngine\Shader\GuiTexturePS.hlsl  c:\workspace\win32playground\demoengine\shader\guitextureps.hlsl GuiCommon.hlsl guicommon.hlsl #include "GuiCommon.hlsl"
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
    float4 outColor = gGuiTexture.Sample(gGuiSampт0Ќ   к8хgб                                                        	       C   (   т0Јьq.     B   C                  (   т0Ё3A@     B                                                                                                                                                                                                                                                                                                                                                                         N <   
    @ (
    @ (Microsoft (R) HLSL Shader Compiler 10.0.10240.16384   6 =hlslFlags 0x5 hlslTarget ps_5_0 hlslEntry PSMain   .     Є      Ф       Ф     T     PSMain   * >  	 pin                                P     T    Ф      P    T    Ф     P    T    Ф     P    T    Ф     P    T    Ф     P    T    Ф     P    T    Ф     P    T    Ф     P     T    Ф      P  $  T    Ф $    P  (  T    Ф (    P  ,  T    Ф ,    P  0  T    Ф 0    P  4  T    Ф 4    P  8  T    Ф 8    P  <  T    Ф <   > >   <PSMain return value>                                  P     T    Ф      P    T    Ф     P    T    Ф     P    T    Ф    . >    outColor                               P           0 <      P          0 <     P          0 <     P               є   0      cэ§GЕLG\ЖZ     іг*Ъ-|пie\ђојИ  ђ   и                  Ь   T     T                            А     А      и     и      ь     ь                           A  @  $  #  2  1  2  1  2  1             і                    8   d                                                                                                                                                                                                                                                                                                  Ъ18        ќ    џџ   џџ     0   0      8        @       float4 ѓђёF       PosH ё     UV ѓђё      ClipRect ё    0 Color "              @ VertexOutput ё
      
                    ђё
     
          
    ђё
 	    
 
                                                                                                                                                                                                                Ъ18              џџ   џџ                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                D3DSHDR                              `                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        џџџџ	/ё(                 9            e                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                $   0                                                                                                                                                                                                                                                                                                                                                                                                                                                    %        PSMain     Q   џџџџ  џџџџgGuiTexture * Q  џџџџџџџџ    џџџџџџџџ gGuiTexture    Q   џџџџџџ  џџgGuiSampler * Q  џџџџџџџџџџџџ    џџџџ gGuiSampler                                                                                                                                                                                                                                                                                                                                                                    ­К    №­Кџџџџ	/ё                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            џџџџw	1    
 Д  L       ,   `                              ­К         `  ­К         
 Ј      $     PЋ        PSMain none -К.ё ­К         `  ­К                џџџџ            џџџџ    џџџџ         A   C:\workspace\Win32Playground\DemoEngine\Shader\GuiTexturePS.hlsl GuiCommon.hlsl ўяўя                  џџџџџџџџџџ џџџџџџџџџџ                                                                                                                                                         .1WРV   еэzіBCArr   /LinkInfo /names /src/headerblock /src/files/c:\workspace\win32playground\demoengine\shader\guitextureps.hlsl /src/files/guicommon.hlsl    
      ц                    n   	   
      "          мQ3                                                                                                                                                                                                                                                                                           ь   4  g  8         Ќ     @  ф  8       (   P  ,   А                                 	   
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 J"Ѕ          йЁд|
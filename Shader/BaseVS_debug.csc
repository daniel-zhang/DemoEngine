   serialization::archive                                                                      D   C:/workspace/Win32Playground/DemoEngine/Debug/../Shader//BaseVS.hlsl   VSMain                                     gView              @      gProj     @   @      gWorld        @      gWorld_InverseTranspose    @   @                       PerFrame          	   PerObject         F  DXBC pZшНnф:>тТ   F     8   0  р    t    RDEF№           <    ўџ  Л  RD11<          (   $          |                                                           PerFrame PerObject Ћ|      Р                    L                   @             џџџџ    џџџџ    D  @   @             џџџџ    џџџџ    gView float4x4 Ћ                              gProj ЋЋ      @             џџџџ    џџџџ    Ѓ  @   @              џџџџ    џџџџ    gWorld gWorld_InverseTranspose Microsoft (R) HLSL Shader Compiler 10.0.10011.16384 ЋISGNЈ                                                                                       Ё                   POSITION NORMAL TEXCOORD TANGENT COLOR ЋOSGNЌ                                                                                       Ѕ                    SV_POSITION POSITION NORMAL TEXCOORD COLOR ЋSHEXи  P  і   j Y  F         Y  F        _  ђ     _  r    _  2    _  ђ    g  ђ         e  ђ     e  r     e  2     e  ђ     h     8  ђ           F         8  ђ     V     F           ђ      F     F    8  ђ     І     F           ђ      F     F    8  ђ     і     F           ђ      F     F    8  ђ           F          8  ђ     V     F            ђ     F    F    8  ђ     І
     F            ђ     F    F    8  ђ      і     F            ђ      F     F    8  ђ           F         8  ђ     V     F            ђ     F    F    8  ђ     І
     F            ђ     F    F    8  ђ      і     F            ђ      F     F    6  r     F    6  2     F    6  ђ     F    6  ђ      F     6  ђ      F     6  ђ     F     6  ђ     F    6  r     F    6  2     F     >  STAT             	                                                             	                                                                       SPDB >  Microsoft C/C++ MSF 7.00
DS                                                                                                                                                                                                                                                                                                                                                                                                                                                                                              Рџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџ8  рџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџ       <       џџџџ                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                         .1DРV   шЩюKL*ТдІ                          мQ3                                                                                                                                                                                                                                                                                                                                                                                                                                                                     PosW    : POSITION;
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

    returЦZ  Lш u N їN   Ђ GD й* &k  g I Н                                                                                                                                                                                                                                                                                                                                                                                                                                                                            
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
                                                                                                                          ўяўя   џ   C:\workspace\Win32Playground\DemoEngine\Shader\BaseVS.hlsl  c:\workspace\win32playground\demoengine\shader\basevs.hlsl 
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
    float4т0   FЙ}Jgб                                                               =   (   т0гВсИ     <   =                                                                                                                                                                                                                                                                                                                                                                                                                  N <   
    @ (
    @ (Microsoft (R) HLSL Shader Compiler 10.0.10240.16384   6 =hlslFlags 0x5 hlslTarget vs_5_0 hlslEntry VSMain   .     $      4      4  	  Є     VSMain   * >  	 vin                                P     Є    4     P    Є    4    P    Є    4    P    Є    4    P    Є    4    P    Є    4    P    Є    4    P    Є    4     P     Є    4$    P  $  Є    40    P  (  Є    44    P  ,  Є    48    P  0  Є    4@    P  4  Є    4D    P  8  Є    4H    P  <  Є    4L   > >   <VSMain return value>                                  P  4  Є    4@    P  8  Є    4D    P  <  Є    4H    P  @  Є    4L    P  ,  Є    40    P  0  Є    44    P     Є    4     P  $  Є    4$    P  (  Є    4(    P    Є    4    P    Є    4    P    Є    4    P    Є    4    P     Є    4     P    Є    4    P    Є    4    P    Є    4   * >   vout                               P     x   `И        P     x   `И       P     x   `И       P     x   `И       P      4   Є     P   $  4   Є     P   (  4   Є     P   ,  H         P   0  H    $    P   4  \   | 0    P   8  \   | 4    P   <  \   | 8    P   @  \   | <    P      p   h      P     p   h     P     p   h     P     p   h      є         иM6v[вћDЄ+С  ђ            и      >   є  Є   '  Є   '   Ф   '  Ф   '   ф   '  ф   '      '     '      '     '   <  '  <  '   \  '  \  '   x  (  x  (     (    (   И  (  И  (   д  (  д  (   є  (  є  (     (    (   0  (  0  (   L  )  L  )   l  )  l  )     )    )   Ј  )  Ј  )   Ш  )  Ш  )   ф  )  ф  )     )    )      +     +   4  ,  4  ,   H  -  H  -   \  /  \  /   p  1  p  1     1    1     1    1   Ќ  1  Ќ  1   Р  1  Р  1   д  1  д  1    &  %  &  %  &  %  &  %  &  %  &  %  &  %  &  %  &  %  &  %  &  %  &  %  &  %  &  %  &  %  &  %  &  %  &  %  &  %  &  %  &  %                                         і                    4   X   t      Д                                                                                                                                       Ъ18          
 џџ   џџ     4   4      <        @       float4 ѓђё @       float3 ѓђё @       float2 ѓђёZ       PosL ё    NormalL ђё    Tex ђё   $ TangentL ё    0 Color               @ VertexIn ё
      V       PosH ё     PosW ё     NormalW ђё   , Tex ђё    4 Color               D VertexOut 
             @             @ float4x4 
 
    
                                                 Ъ18              џџ   џџ                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 n vout;
}
    =   <           x                                                                                                                                                                                                                                                                                                                                                                                                                                                                                              D3DSHDR и                             `                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        џџџџ	/ё8     Y            Е      u      5                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           <   H                                                                                                                                                                                                                                                                                                                                                                                                                                    %        VSMain     Q       џџџџџџgView   " Q          џџџџџџџџџџџџ gView  Q     @ џџџџџџgProj   " Q      @   џџџџџџџџџџџџ gProj  Q      џџџџџџgWorld  & Q         џџџџџџџџџџџџ gWorld                                                                                                                                                                                                                                                                                                                         ­К    №­Кџџџџ	/ё                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            џџџџw	1    
 Д  L       ,   H                              ­К    и     `  ­К         	 (      4     pД        VSMain none -К.ё ­К    и     `  ­К                џџџџ    и        џџџџ    џџџџ         C:\workspace\Win32Playground\DemoEngine\Shader\BaseVS.hlsl  ўяўя                  џџџџџџџџџџ џџџџџџџџџџ                                                                                                                                                                                 .1DРV   шЩюKL*ТдІh   /LinkInfo /names /src/headerblock /src/files/c:\workspace\win32playground\demoengine\shader\basevs.hlsl                       "      
                 мQ3                                                                                                                                                                                                                                                                                                                                   Ф   д  O  8       /     И  |	  <       (   `  ,   м                  
               	                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                       ёуr)           ЏdЬy
   serialization::archive                                                                     I   C:/workspace/Win32Playground/DemoEngine/Debug/../Shader//GuiBorderPS.hlsl   PSMain                                     gGuiSampler                    gGuiTexture            	   gGuiParam                                CustomTilingFactors          іM  DXBCљu®nЃYцп®Em*ХН   іM     8   і  H  |    ђ  RDEFt     »      <    €€  @  RD11<          (   $          Ь                             ®            €€€€          і                              gGuiSampler gGuiTexture CustomTilingFactors і      а                                €€€€    €€€€    gGuiParam float4 ЂЂЂ                              Microsoft (R) HLSL Shader Compiler 10.0.10011.16384 ISGNМ         h                    t                   }                    Ж                   SV_POSITION TEXCOORD CLIPRECT COLOR OSGN,                               SV_Target ЂЂSHEXМ  P   #  jИ Y  FО         Z   `     X  p     UU  b т    b т    e  т      h     6        
А          6  "      А          6  B      *А          6  В      :А          6       *    6  "     :    6  Т         1  B     @    А?
      *     6  B     
 АA          B     *     
    8       *     
     6  B     
 АA          B     *      *            
     *            
     6  B     
 АA          B      *      *     8  B      *      
        B     *      
      6  2     ж
      1        @    А?      
      6         АA                
          8        
           6  B       АA          B      *      :              
      *              
      6  B       АA          B      *      :      8        *      
         "     
              6  "     :         E  Л¬  АCU т      F     F~      `     8  т      F     F    /  r      F     8  
r      F     @  /Їи>/Їи>/Їи>      r      F     6  r      F     6  В      :      >  STATФ   /                                                                                                                                           SPDB F  Microsoft C/C++ MSF 7.00
DS         #   ∞                                                                                                                                                                                                                                                                                                                                                                                                                                                                                   ј€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€8   ю€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€       <       €€€€                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                         Ф.1ОWјV   \нwvЩЅ@∞)Ё5уьЮ                          №Q3                                                                                                                                                                                                                                                                                                                                                                                                                                                                    uiParam.w;

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

    //float4 outColor ∆Z  M
 e“ ¬ A$ aЕ uГ НЁ Вo }
 }µ »Q Ѕ! Aє                                                                                                                                                                                                                                                                                                                                                                                                                                                                        #include "GuiCommon.hlsl"

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
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             
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
                                                                                                                                                                                                юпюп   х   C:\workspace\Win32Playground\DemoEngine\Shader\GuiBorderPS.hlsl  c:\workspace\win32playground\demoengine\shader\guiborderps.hlsl GuiCommon.hlsl guicommon.hlsl #include "GuiCommon.hlsl"

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
    float bottom = gGв0ђ   ∞M>еg—                                                               С   (   в0П°3A@  В   A   С               B   (   в0yъ№ґA     A   B                                                                                                                                                                                                                                                                                                                                                                      N <   
    @ (
    @ (Microsoft (R) HLSL Shader Compiler 10.0.10240.16384   6 =hlslFlags 0x5 hlslTarget ps_5_0 hlslEntry PSMain   .     ,      (      (    d    †PSMain   * >  	 pin                                P     d    (     P    d    (    P    d    (    P    d    (    P    d    (    P    d    (    P    d    (    P    d    (    P     d    (     P  $  d    ($    P  (  d    ((    P  ,  d    (,    P  0  d    (0    P  4  d    (4    P  8  d    (8    P  <  d    (<   > >  И <PSMain return value>                                  P     d    (     P    d    (    P    d    (    P    d    (   * >@     left                               P      |         * >@     top                                P      Ф    `   . >@     right                                  P      ђ    dP DЉ    . >@     bottom                                 P      ƒ    0   . >@     tileU                                  P      Ў    м† и    . >@     tileV                                  P      м    †   . >   targetUV                               P          М» Ф      P         ј\ (Д ,    P      »        P      H    (    P     \   0 HP $    P     (        . >    outColor                               P      ф   Ш 0 <      P     ф   Ш 0 <     P     ф   Ш 0 <     P     ф   Ш      ф   0      ; Ф)ТxVK#ЭУ  В   ц”* -|яie\тёшЄ  т   А        М      ^   t  d     Аd      |     А|      Ф     АФ      ђ     Ађ      ƒ     Аƒ      Ў     АЎ      м     Ам           А          А     (    А(     @    А@     \    А\     x    Аx     Р    АР     ђ    Ађ     »    А»     №    А№     ф    Аф         А     ,    А,     H    АH     \    А\     `    А`     |    А|     И     АИ      †     А†      Љ     АЉ      Ў     АЎ      р     Ар           А      (  !  А(  !   <  !  А<  !   T  !  АT  !   p  !  Аp  !   М  !  АМ  !   ®  "  А®  "   ђ  "  Ађ  "   ј  "  Ај  "   ƒ  "  Аƒ  "   »  $  А»  $   ф  (  Аф  (     +  А  +   $  +  А$  +   L  +  АL  +   `  -  А`  -   t  -  Аt  -   И  -  АИ  -                                  	      	 @  - 	 @  - 	 @  . 	 @ 3 > 	 @ 3 > 	 @  ? 	 9 ( 7 	 9  8 	 9  8 	 9  8 	 9  8         
    
  
  	 ?  , 	 ?  , 	 ?  - 	 ? 2 = 	 ? 2 = 	 ?  > 	 9 ( 7 	 9  8 	 9  8 	 9  8 	 9  8                  @  ?  $  #  2  1  2  1  2  1             ц                    8   `   А   ђ   ћ                                                                                                                                                                                                                                        18            €€   €€     8   8      @        @       float4 утсF       PosH с     UV утс      ClipRect с    0 Color "              @ VertexOutput с
      
              @       float2 утс
            тс
     
 	         
    тс
     
                                                                                                                                                                              18              €€   €€                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 = gGuiTexture.Sample(gGuiSampler, pin.UV.xy*pin.UV.zw);

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
    і  A          B   В   С               †                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 D3DSHDR М                             `                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        €€€€	/с8            Ќ      Б      9      a            ≠                                                                     А                                                                Б                                                                                                                                                  А                                                                                                                                                                                                                                                       $   0   <   H                                                                                                                                                                                                                                                                                                                                                                                                                            %    М    PSMain     Q       €€€€€€gGuiParam   & Q          €€€€€€€€€€€€ gGuiParam  Q
   €€€€  €€€€gGuiTexture * Q
  €€€€€€€€    €€€€€€€€ gGuiTexture    Q   €€€€€€  €€gGuiSampler * Q  €€€€€€€€€€€€    €€€€ gGuiSampler                                                                                                                                                                                                                                                                                            ≠Ї    р≠Ї€€€€	/с                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            €€€€w	1    
М іЬ  L       ,   `                              ≠Ї    М     `  ≠Ї         
 0      ћ     афЗ        PSMain none -Ї.с ≠Ї    М     `  ≠Ї                €€€€    М        €€€€    €€€€         @   C:\workspace\Win32Playground\DemoEngine\Shader\GuiBorderPS.hlsl GuiCommon.hlsl  юпюп                  €€€€€€€€€€ €€€€€€€€€€                                                                                                                                                         Ф.1ОWјV   \нwvЩЅ@∞)Ё5уьЮЗ   /LinkInfo /names /src/headerblock /src/files/c:\workspace\win32playground\demoengine\shader\guiborderps.hlsl /src/files/guicommon.hlsl    
      n                    "      m   	   
          №Q3                                                                                                                                                                                                                                                                                            л   X  g  8       5  ђ   A  @    @       (   h  ,   ш                                    	   
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                         ё†=/          ©Cґй
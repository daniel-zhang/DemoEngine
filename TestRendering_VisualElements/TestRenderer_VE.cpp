#include "TestRenderer_VE.h"

#include "Renderer/Texture.h"

Texture gTexture1;
Texture gTexture2;
Texture gBtnTexture;

Vector4D toClipRect(const Vector2D& point, const Vector2D& size)
{
    Vector4D clipRect;
    clipRect.x = point.x; // Left
    clipRect.y = point.y; // Top
    clipRect.z = point.x + size.x; // Right
    clipRect.w = point.y + size.y; // Bottom
    return clipRect;
}

#define MAX_FLT 3.402823466e+38F

const Vector4D gInfRect = Vector4D(-MAX_FLT, -MAX_FLT, MAX_FLT, MAX_FLT);

void TestRenderer_VE::initialize(std::vector<HWND>& hwnds)
{
    mRenderer = Renderer::getInstance();
    mRenderer->initialize(RendererSettings());

    std::wstring dir = FileOp::getDefaultPath<FileOp::TextureResource>().toString();

    Image img;
    img.createFromFile(dir + L"Defraggler_icon.png");
    gTexture1.initializeAsImmutableFromImage(img);

    img.destroy();
    img.createFromFile(dir + L"Adium.png");
    gTexture2.initializeAsImmutableFromImage(img);

    img.destroy();
    //img.createFromFile(dir + L"button_1.png");
    img.createFromFile(dir + L"button.png");
    //img.createFromFile(dir + L"button_straight.png");
    gBtnTexture.initializeAsImmutableFromImage(img);

    for (uint32_t i = 0; i < hwnds.size(); ++i)
    {
        HWND& hwnd = hwnds[i];

        // Create a gui render target
        mRenderer->createGuiRenderTarget(hwnd);

        // Hand roll an hwnd element group
        VisualElementGroup& drawGroup = mTestDrawGroups.addUninitializedGroup();
        drawGroup.Hwnd = hwnd;

        // Quad with texture
        VisualElement quad1;
        quad1.ElementType   = VisualElement::QuadElement;
        quad1.LayerId       = 1;
        quad1.PixelPosition = Vector2D(80, 40);
        quad1.PixelSize     = Vector2D(gTexture1.getWidth()/2.f, gTexture1.getHeight()/2.f);
        quad1.Tex           = &gTexture1;
        quad1.ScalingFactor = 1.f;
        quad1.Color         = Color(1, 1,1, .5f);
        quad1.ClipRect      = toClipRect(quad1.PixelPosition, quad1.PixelSize);

        drawGroup.Elements.push_back(quad1);

        // Quad with texture
        VisualElement quad2;
        quad2.ElementType   = VisualElement::QuadElement;
        quad2.LayerId       = 1;
        quad2.PixelPosition = Vector2D(200, 20);
        quad2.PixelSize     = Vector2D(gTexture2.getWidth()/2.f, gTexture2.getHeight()/2.f);
        quad2.Tex           = &gTexture2;
        quad2.ScalingFactor = 1.f;
        quad2.Color         = Color(1, 1, 1, 1);
        quad2.ClipRect      = toClipRect(quad2.PixelPosition, Vector2D(quad2.PixelSize.x, quad2.PixelSize.y/2.f));

        drawGroup.Elements.push_back(quad2);


        // Colored quad with border on top
        VisualElement dQuad;
        dQuad.ElementType   = VisualElement::QuadElement;
        dQuad.LayerId       = 1;
        dQuad.PixelPosition = Vector2D(420, 20);
        dQuad.PixelSize     = Vector2D(300, 100);
        dQuad.ScalingFactor = 1.f;
        dQuad.Color         = Palette::DarkGray;
        dQuad.ClipRect      = toClipRect(dQuad.PixelPosition, dQuad.PixelSize);

        drawGroup.Elements.push_back(dQuad);

        // Border
        VisualElement border1;
        border1.ElementType        = VisualElement::BorderElement;
        border1.LayerId            = 2;
        border1.PixelPosition      = Vector2D(415, 15);
        border1.PixelSize          = Vector2D(310, 310);
        border1.Tex                = &gBtnTexture;
        border1.ScalingFactor      = 1.f;
        border1.Color              = Palette::White;
        border1.ClipRect           = toClipRect(border1.PixelPosition, border1.PixelSize);
        float bordersize           = 4.f / 200.f;
        border1.BorderAttr.Padding = Vector4D(bordersize, bordersize, bordersize, bordersize);

        drawGroup.Elements.push_back(border1);

        // A line of text
        VisualElement textQuad;
        textQuad.ElementType            = VisualElement::TextElement;
        textQuad.LayerId                = 3;
        textQuad.PixelPosition          = Vector2D(430, 50);
        textQuad.PixelSize              = Vector2D(); // Size has to be figured out before rendering
        textQuad.ScalingFactor          = 1.f;
        textQuad.Color                  = Palette::Green;
        textQuad.ClipRect               = toClipRect(textQuad.PixelPosition, Vector2D(1000, 1000));
        textQuad.TextAttr.FontName      = "segoeui.ttf";
        textQuad.TextAttr.FontSize      = 16;
        textQuad.TextAttr.Text          = L"I am sort of tired about all this";

        drawGroup.Elements.push_back(textQuad);

        // Gradient
        VisualElement gradient;
        gradient.ElementType              = VisualElement::GradientElement;
        gradient.LayerId                  = 0;
        gradient.PixelPosition            = Vector2D(20, 20);
        gradient.PixelSize                = Vector2D(200, 200);
        gradient.ScalingFactor            = 1.f;
        gradient.Color                    = Palette::White;
        gradient.ClipRect                 = toClipRect(gradient.PixelPosition, gradient.PixelSize);
        gradient.GradientAttr.Orientation = GradientAttributes::HorizentalGradient;
        gradient.GradientAttr.Stops.push_back(GradientStop(Vector2D(0,0), Palette::Red));
        gradient.GradientAttr.Stops.push_back(GradientStop(Vector2D(30,30), Palette::LightGray));
        gradient.GradientAttr.Stops.push_back(GradientStop(Vector2D(60,60), Palette::Gray));
        gradient.GradientAttr.Stops.push_back(GradientStop(Vector2D(90,90), Palette::DarkGray));
        gradient.GradientAttr.Stops.push_back(GradientStop(Vector2D(120,120), Palette::Gray));
        gradient.GradientAttr.Stops.push_back(GradientStop(Vector2D(150,150), Palette::LightGray));
        gradient.GradientAttr.Stops.push_back(GradientStop(Vector2D(180,180), Palette::Yellow));

        drawGroup.Elements.push_back(gradient);

        // A hardware rendered plain line
        VisualElement plain_line;
        plain_line.ElementType          = VisualElement::LineElement;
        plain_line.LayerId              = 3;
        plain_line.PixelPosition        = Vector2D(80, 40);
        plain_line.PixelSize            = Vector2D();
        plain_line.ScalingFactor        = 1.f;
        plain_line.Color                = Palette::Red;
        plain_line.ClipRect             = gInfRect;
        plain_line.LineAttr.Prefiltered = false;
        plain_line.LineAttr.Thickness   = 1.f; // no effect on plain lines
        plain_line.LineAttr.Points.push_back(Vector2D(10, 110));
        plain_line.LineAttr.Points.push_back(Vector2D(100, 110));
        plain_line.LineAttr.Points.push_back(Vector2D(100, 310));
        plain_line.LineAttr.Points.push_back(Vector2D(10, 110));

        drawGroup.Elements.push_back(plain_line);

        // An anti-aliased line 
        VisualElement aa_line;
        aa_line.ElementType          = VisualElement::LineElement;
        aa_line.LayerId              = 3;
        aa_line.PixelPosition        = Vector2D(80, 40);
        aa_line.PixelSize            = Vector2D();
        aa_line.ScalingFactor        = 1.f;
        aa_line.Color                = Palette::Yellow;
        aa_line.ClipRect             = gInfRect;
        aa_line.LineAttr.Prefiltered = true;
        aa_line.LineAttr.Thickness   = 1.f;
        aa_line.LineAttr.Points.push_back(Vector2D(10, 10));
        aa_line.LineAttr.Points.push_back(Vector2D(260, 63));
        aa_line.LineAttr.Points.push_back(Vector2D(255, 260));
        aa_line.LineAttr.Points.push_back(Vector2D(10, 10));

        drawGroup.Elements.push_back(aa_line);

        // Spline
        VisualElement spline;
        spline.ElementType          = VisualElement::SplineElement;
        spline.LayerId              = 3;
        spline.PixelPosition        = Vector2D(180, 340);
        spline.PixelSize            = Vector2D();
        spline.ScalingFactor        = 1.f;
        spline.Color                = Palette::Yellow;
        spline.ClipRect             = gInfRect;
        spline.SplineAttr.Thickness = 1.f;
        spline.SplineAttr.StartPt   = Vector2D(0, 0);
        spline.SplineAttr.EndPt     = Vector2D(100, 100);
        spline.SplineAttr.StartDir  = Vector2D(200, 0);
        spline.SplineAttr.EndDir    = Vector2D(0, 200);

        drawGroup.Elements.push_back(spline);
    }
}

void TestRenderer_VE::testDraw(float dt)
{
    mRenderer->drawGui(mTestDrawGroups);
}

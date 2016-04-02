#include "TestGuiDraw.h"

void GuiDrawTest::initialize(std::vector<HWND>& hwnds)
{
    mRenderer = Renderer::getInstance();
    mRenderer->initialize(RendererSettings());

    Color colors[] = {Palette::Red, Palette::Green, Palette::White};
    uint32_t numColors = sizeof colors / sizeof Color;
    float posY[] = {10, 100, 200, 300};
    uint32_t numPosY = sizeof posY / sizeof(float);

    for (uint32_t i = 0; i < hwnds.size(); ++i)
    {
        HWND& hwnd = hwnds[i];

        // Create a Gui render target
        mRenderer->createGuiRenderTarget(hwnd);

        // Build  test draw data
        VisualElementGroup& drawGroup = mTestDrawGroups.addUninitializedGroup();

        drawGroup.Hwnd = hwnd;

        Vector2D pos(0, posY[i % numPosY]);

        for (uint32_t j = 0; j < 5; ++j)
        {
            VisualElement debugQuad;

            debugQuad.ElementType   = VisualElement::DebugQuadElement;
            debugQuad.LayerId       = 0;
            debugQuad.PixelPosition = pos;
            debugQuad.PixelSize     = Vector2D(30, 30);
            debugQuad.ScalingFactor = 1.f;
            debugQuad.Color         = colors[j % numColors];

            drawGroup.Elements.push_back(debugQuad);
            pos.x += 35;
        }

        VisualElement textQuad;
        textQuad.ElementType    = VisualElement::TextElement;
        textQuad.LayerId        = 0;
        textQuad.PixelPosition  = Vector2D(50, 90);
        textQuad.PixelSize      = Vector2D();
        textQuad.ScalingFactor  = 1.f;
        textQuad.Color          = Palette::White;
        textQuad.FontName       = "segoeui.ttf";
        textQuad.FontSize       = 9;
        textQuad.Text           = L"Default Window";
        drawGroup.Elements.push_back(textQuad);
    }
}

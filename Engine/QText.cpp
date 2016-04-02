#include "QText.h"
#include "Core/Math.h"

QText::QText() : QWidget()
{
    mScaling   = 1.f;
    mFontColor = Palette::White;
    mFontName  = "segoeui.ttf";
    mFontSize  = 9;
    mText      = L"Empty Text";
}

QText::~QText()
{

}

void QText::computeMinimalSize()
{
    // TODO
}

void QText::layoutChildren(const QRectangle& srcRect, std::vector<QRectangle>& outChildrenRects)
{

}

void QText::drawRecursively(VisualElementGroup& group, QRectangle& drawRect)
{
    // drawRect comes with margins
    mPosition.x = drawRect.Position.x + mMargin.Left;
    mPosition.y = drawRect.Position.y + mMargin.Top;
    mSize.x     = drawRect.Size.x - mMargin.getHorizental();
    mSize.y     = drawRect.Size.y - mMargin.getVertical();
    mLayer      = drawRect.Layer;
    Vector2D positionToRoot = mPosition + drawRect.ToRoot;

    VisualElement textQuad;
    textQuad.ElementType   = VisualElement::TextElement;
    textQuad.LayerId       = mLayer;
    textQuad.ScalingFactor = mScaling;
    textQuad.PixelPosition = positionToRoot;
    textQuad.PixelSize     = mSize;
    textQuad.Color         = mFontColor;
    textQuad.ClipRect = Vector4D(-1000, -1000, 1000, 1000);
    textQuad.TextAttr.FontName      = mFontName;
    textQuad.TextAttr.FontSize      = mFontSize;
    textQuad.TextAttr.Text          = mText;

    group.Elements.push_back(textQuad);
}


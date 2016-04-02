#include "QButton.h"

QButton::QButton() : QWidget()
{
    mBkgColorNormal      = Palette::QBlue;
    mBkgColorHovered     = Palette::QBlue_Dark;
    mBkgColorPressedDown = Palette::QYellow;
    mBkgColor            = mBkgColorNormal;

    mOutlineColorNormal      = Palette::QBlue_Dark;
    mOutlineColorHovered     = Palette::QBlue;
    mOutlineColorPressedDown = Palette::QDark;
    mOutlineColor            = mOutlineColorNormal;


    mPosition = Vector2D(30, 30);
    mSize = Vector2D(200, 100);
}

QButton::~QButton()
{

}

void QButton::computeMinimalSize()
{
    if (mChildWidgets.empty())
    {
        mMinimalSize = mSize;
    }
    else
    {
        mMinimalSize = mChildWidgets[0]->getMinimalSize();
    }
}

void QButton::layoutChildren(const QRectangle& srcRect, std::vector<QRectangle>& outChildrenRects)
{
    // Empty
}

void QButton::drawRecursively(VisualElementGroup& group, QRectangle& drawRect)
{
    // drawRect comes with margins
    mPosition.x = drawRect.Position.x + mMargin.Left;
    mPosition.y = drawRect.Position.y + mMargin.Top;
    mSize.x     = drawRect.Size.x - mMargin.getHorizental();
    mSize.y     = drawRect.Size.y - mMargin.getVertical();
    mLayer      = drawRect.Layer;

    Vector2D ToRootPosition = mPosition + drawRect.ToRoot;

    VisualElement debugQuad;
    debugQuad.ElementType   = VisualElement::QuadElement;
    debugQuad.LayerId       = mLayer;
    debugQuad.PixelPosition = ToRootPosition;
    debugQuad.PixelSize     = mSize;
    debugQuad.ScalingFactor = 1.f;
    debugQuad.Color         = mBkgColor;
    debugQuad.Tex           = nullptr;
    debugQuad.ClipRect = Vector4D(ToRootPosition.x, ToRootPosition.y, ToRootPosition.x + mSize.x, ToRootPosition.y + mSize.y);
    group.Elements.push_back(debugQuad);

    // Line Border
    VisualElement outline;
    outline.ElementType          = VisualElement::LineElement;
    outline.LayerId              = mLayer + 1;
    outline.PixelPosition = ToRootPosition;
    outline.PixelSize            = Vector2D();
    outline.ScalingFactor        = 1.f;
    outline.Color                = mOutlineColor;
    outline.ClipRect = Vector4D(ToRootPosition.x - 1, ToRootPosition.y - 1, ToRootPosition.x + mSize.x + 1, ToRootPosition.y + mSize.y + 1);
    outline.LineAttr.Prefiltered = false;
    outline.LineAttr.Thickness   = 1.f; // no effect on plain lines
    outline.LineAttr.Points.push_back(Vector2D(0, 0));
    outline.LineAttr.Points.push_back(Vector2D(mSize.x - 0, 0));
    outline.LineAttr.Points.push_back(Vector2D(mSize.x - 0, mSize.y - 0));
    outline.LineAttr.Points.push_back(Vector2D(0, mSize.y - 0));
    outline.LineAttr.Points.push_back(Vector2D(0, 0));
    group.Elements.push_back(outline);

    if (!mChildWidgets.empty())
    {
        QRectangle childRect;
        childRect.Position = Vector2D(mPadding.Left, mPadding.Top);
        childRect.Size     = drawRect.Size - Vector2D(mPadding.getHorizental(), mPadding.getVertical());
        childRect.Layer    = mLayer + 1;
        childRect.ToRoot   = drawRect.Position + drawRect.ToRoot;

        mChildWidgets[0]->drawRecursively(group, childRect);
    }
}

bool QButton::onHover()
{
    // Stop onHover event from propagating down
    return true;
}

bool QButton::onMouseEnter()
{
    mUIState = EUIState::Hovered;
    mBkgColor = mBkgColorHovered;
    mOutlineColor = mOutlineColorHovered;

    return true;
}

bool QButton::onMouseLeave()
{
    mUIState = EUIState::Normal;
    mBkgColor = mBkgColorNormal;
    mOutlineColor = mOutlineColorNormal;

    return true;
}

bool QButton::onClick()
{
    return true;
}

bool QButton::onMouseDown()
{
    mUIState = EUIState::PressedDown;
    mBkgColor = mBkgColorPressedDown;
    mOutlineColor = mOutlineColorPressedDown;
    return false;
}

bool QButton::onMouseUp()
{
    mUIState = EUIState::Hovered;
    mBkgColor = mBkgColorHovered;
    mOutlineColor = mOutlineColorHovered;
    return false;
}


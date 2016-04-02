#include "QLinearLayout.h"

void QLinearLayout::computeMinimalSize()
{
    mMinimalSize = Vector2D::zero();
    mTotalMatchParentSpace = 0;
    mTotalWrapContentSpace = 0;

    switch (mOrientation)
    {
    case QLinearLayout::Vertical:
        for (auto& child : mChildWidgets)
        {
            const Vector2D& cMinimalMarginedSize = child->getMinimalMarginedSize();
            mMinimalSize.x = max(mMinimalSize.x, cMinimalMarginedSize.x);
            mMinimalSize.y += cMinimalMarginedSize.y;
            if (child->getHeightLayoutPolicy() == ELayoutPropertyValue::match_parent)
                mTotalMatchParentSpace += cMinimalMarginedSize.y;
            else
                mTotalWrapContentSpace += cMinimalMarginedSize.y;
        }
        break;
    case QLinearLayout::Horizental:
        for (auto& child : mChildWidgets)
        {
            const Vector2D& cMinimalMarginedSize = child->getMinimalMarginedSize();
            mMinimalSize.x += cMinimalMarginedSize.x;
            mMinimalSize.y = max(mMinimalSize.y, cMinimalMarginedSize.y);
            if (child->getWidthLayoutPolicy() == ELayoutPropertyValue::match_parent)
                mTotalMatchParentSpace += cMinimalMarginedSize.x;
            else
                mTotalWrapContentSpace += cMinimalMarginedSize.x;
        }
        break;
    default:
        break;
    }
    // Apply padding 
    mMinimalSize.x += mPadding.getHorizental();
    mMinimalSize.y += mPadding.getVertical();
}

void QLinearLayout::layoutChildren(const QRectangle& srcRect, std::vector<QRectangle>& outChildrenRects)
{
    /*
    const float paddedX      = mPadding.Left + mMargin.Left;
    const float paddedY      = mPadding.Top + mMargin.Top;
    const float paddedWidth  = srcRect.Size.x - mPadding.getHorizental() - mMargin.getHorizental();
    const float paddedHeight = srcRect.Size.y - mPadding.getVertical() - mMargin.getVertical();
    */
    const float paddedX      = mPadding.Left;
    const float paddedY      = mPadding.Top;
    const float paddedWidth  = srcRect.Size.x - mPadding.getHorizental() - mMargin.getHorizental();
    const float paddedHeight = srcRect.Size.y - mPadding.getVertical() - mMargin.getVertical();

    switch (mOrientation)
    {
    case QLinearLayout::Vertical:
    {
        float startY = paddedY;
        const float availableHeightForMatchParent = paddedHeight - mTotalWrapContentSpace;

        for (auto& child : mChildWidgets)
        {
            const float childWidth = child->getMinimalMarginedSize().x;
            const float childHeight = child->getMinimalMarginedSize().y;

            // Create a draw rect for the current child
            QRectangle childRect;
            childRect.Position.y = startY;
            //childRect.ToRoot     = srcRect.Position + srcRect.ToRoot; 
            childRect.ToRoot     = srcRect.Position + srcRect.ToRoot + Vector2D(mMargin.Left, mMargin.Top);
            childRect.Layer      = srcRect.Layer + 1;

            if (child->getWidthLayoutPolicy() == ELayoutPropertyValue::match_parent)
            {
                // Adjust the widget horizontally to match its parent
                childRect.Position.x = paddedX;
                childRect.Size.x = paddedWidth;
            }
            else if (child->getWidthLayoutPolicy() == ELayoutPropertyValue::wrap_content)
            {
                // Adjust the widget horizontally to wrap its parent
                childRect.Position.x = paddedX + (paddedWidth - childWidth) / 2.f;
                childRect.Size.x = childWidth;
            }
            else{}

            if (child->getHeightLayoutPolicy() == ELayoutPropertyValue::match_parent)
            {
                // Calculate how much vertically space to allocate
                float allocatedHeight = availableHeightForMatchParent * childHeight / mTotalMatchParentSpace;
                startY += allocatedHeight;
                childRect.Size.y = allocatedHeight;

            }
            else if (child->getHeightLayoutPolicy() == ELayoutPropertyValue::wrap_content)
            {
                // Adjust the widget vertically to wrap its content
                startY += childHeight;
                childRect.Size.y = childHeight;
            }
            else{}

            outChildrenRects.push_back(childRect);
        }
    }
    break;
    case QLinearLayout::Horizental:
    {
        float startX = paddedX;
        const float availableWidthForMatchParent = paddedWidth - mTotalWrapContentSpace;

        for (auto& child : mChildWidgets)
        {
            const float childWidth = child->getMinimalMarginedSize().x;
            const float childHeight = child->getMinimalMarginedSize().y;

            // Create a draw rect for the current child
            QRectangle childRect;
            childRect.Position.x = startX;
            //childRect.ToRoot = srcRect.Position + srcRect.ToRoot;
            childRect.ToRoot     = srcRect.Position + srcRect.ToRoot + Vector2D(mMargin.Left, mMargin.Top);
            childRect.Layer = srcRect.Layer + 1;

            if (child->getHeightLayoutPolicy() == ELayoutPropertyValue::match_parent)
            {
                // Adjust the widget vertically to match its parent
                childRect.Position.y = paddedY;
                childRect.Size.y = paddedHeight;
            }
            else if (child->getHeightLayoutPolicy() == ELayoutPropertyValue::wrap_content)
            {
                // Adjust the widget vertically to wrap its content
                childRect.Position.y = paddedY + (paddedHeight - childHeight) / 2.f;
                childRect.Size.y = childHeight;
            }
            else {}

            // Move start position x toward the next widget
            if (child->getWidthLayoutPolicy() == ELayoutPropertyValue::match_parent)
            {
                // Calculate how much horizontal space to allocate
                float allocatedWidth = availableWidthForMatchParent * childWidth / mTotalMatchParentSpace;
                startX += allocatedWidth;
                childRect.Size.x = allocatedWidth;
            }
            else if (child->getWidthLayoutPolicy() == ELayoutPropertyValue::wrap_content)
            {
                // Adjust the widget horizontally to wrap its content
                startX += childWidth;
                childRect.Size.x = childWidth;
            }
            else {}

            outChildrenRects.push_back(childRect);
        }
    }
    break;
    default:
        break;
    }
}

void QLinearLayout::drawRecursively(VisualElementGroup& group, QRectangle& drawRect)
{
    // drawRect comes with margins
    mPosition.x             = drawRect.Position.x + mMargin.Left;
    mPosition.y             = drawRect.Position.y + mMargin.Top;
    mSize.x                 = drawRect.Size.x - mMargin.getHorizental();
    mSize.y                 = drawRect.Size.y - mMargin.getVertical();
    mLayer                  = drawRect.Layer;

    // Generate draw rectangles for children
    std::vector<QRectangle> childrenRects;
    layoutChildren(drawRect, childrenRects);

    for (uint32_t i = 0; i < mChildWidgets.size(); ++i)
    {
        if (mChildWidgets[i])
            mChildWidgets[i]->drawRecursively(group, childrenRects[i]);
    }

    Vector2D toRootPosition = mPosition + drawRect.ToRoot;
    VisualElement debugQuad;
    debugQuad.ElementType   = VisualElement::QuadElement;
    debugQuad.LayerId       = mLayer;
    debugQuad.PixelPosition = toRootPosition;
    debugQuad.PixelSize     = mSize;
    debugQuad.ScalingFactor = 1.f;
    debugQuad.Color         = mBkgColor;
    debugQuad.Tex           = nullptr;
    debugQuad.ClipRect      = Vector4D(toRootPosition.x, toRootPosition.y, toRootPosition.x + mSize.x, toRootPosition.y + mSize.y);
    group.Elements.push_back(debugQuad);
}

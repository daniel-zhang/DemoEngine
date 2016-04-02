#pragma once

#include "QWidget.h"

class QLinearLayout : public QWidget
{
public:
    enum ELayoutOrientation
    {
        Vertical = 0,
        Horizental
    };

public:

    QLinearLayout(ELayoutOrientation orientation) : QWidget(), mOrientation(orientation), mBkgColor(Palette::Red){}

    virtual void computeMinimalSize() override;

    virtual void layoutChildren(const QRectangle& srcRect, std::vector<QRectangle>& outChildrenRects) override;

    virtual void drawRecursively(VisualElementGroup& group, QRectangle& drawRect) override;

    void setBkgColor(const Color& c) { mBkgColor = c; }

protected:
    ELayoutOrientation mOrientation;

    // For vertical layout, it is the total minimal height of all children with 'height = match_parent' specified
    // For horizontal layout, it is the total minimal width of all children with 'width = match_parent' specified
    float mTotalMatchParentSpace;

    // For vertical layout, it is the total minimal height of all children with 'height = wrap_content' specified
    // For horizontal layout, it is the total minimal width of all children with 'width = wrap_content' specified
    float mTotalWrapContentSpace;

    Color mBkgColor;

};
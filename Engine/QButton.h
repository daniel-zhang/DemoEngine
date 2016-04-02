#pragma once

#include "QWidget.h"

class QButton : public QWidget
{
public:
    QButton();
    ~QButton();

    void setBkgColor(const Color& c) { mBkgColor = c; }

    virtual void computeMinimalSize() override;

    virtual void layoutChildren(const QRectangle& srcRect, std::vector<QRectangle>& outChildrenRects) override;

    virtual void drawRecursively(VisualElementGroup& group, QRectangle& drawRect) override;

public:
    virtual bool onClick() override;

    virtual bool onMouseEnter() override;

    virtual bool onHover() override;

    virtual bool onMouseLeave() override;

    virtual bool onMouseDown() override;

    virtual bool onMouseUp() override;

protected:
    // Appearance
    Color mBkgColorNormal;
    Color mBkgColorHovered;
    Color mBkgColorPressedDown;
    Color mBkgColor;
    
    Color mOutlineColorNormal;
    Color mOutlineColorHovered;
    Color mOutlineColorPressedDown;
    Color mOutlineColor;
};

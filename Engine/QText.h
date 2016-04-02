#pragma once
#include "QWidget.h"

class QText : public QWidget
{
public:
    QText();
    ~QText();

    virtual void computeMinimalSize() override;

    virtual void layoutChildren(const QRectangle& srcRect, std::vector<QRectangle>& outChildrenRects) override;

    virtual void drawRecursively(VisualElementGroup& group, QRectangle& drawRect) override;
    
public:
    void setText(const std::wstring& text){ mText = text; }

    void setFontName(const std::string& fontName){ mFontName = fontName; }

    void setFontSize(uint32_t fontSize){ mFontSize = fontSize; }

    void setScaling(float factor){ mScaling = factor; }

    void setColor(const Color& c){ mFontColor = c; }

protected:
    std::wstring mText;
    std::string mFontName;
    uint32_t mFontSize;
    float mScaling;
    Color mFontColor;
};
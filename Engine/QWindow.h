#pragma once

#include "QWidget.h"
#include <functional>
#include "Application/MessageHandler.h"

enum EQWindowType
{
    AppWindow, // Window icon appears in task bar
    TooltipWindow, // Not in task bar, always on top
    DockedChildWindow // Does not have a backing native window
};

class QWindow : public QWidget
{
public:
    QWindow();

    QWindow(const std::wstring& title, EQWindowType type, const Vector2D& position, const Vector2D& size, float opacity);

    ~QWindow();

public:
    typedef std::function<bool(void)> TOnCloseListener;

    void setOnCloseListener(TOnCloseListener listener) { mOnCloseListener = listener; }

    class Window* getNativeWindow();

    void initialize();

    void close();

    void resize(int32_t x, int32_t y);

    void minimize();

    EWindowZone simulateOSHitTest(const int32_t x, const int32_t y) const;

public:
    // Widget interface
    virtual void tick(float dt) override {}

    virtual void drawRecursively(VisualElementGroup& group, QRectangle& drawRect) override;

    virtual void setChild(QWidget* child)
    {
        if (mChildWidgets.empty())
        {
            mChildWidgets.push_back(child);
        }
        else
        {
            delete mChildWidgets[0];
            mChildWidgets[0] = child;
        }
    }

protected:
    class Window* mNativeWindow;

protected:
    uint32_t mBorderWidth; // left, top, right, bottom

    uint32_t mTitleBarHeight;

    uint32_t mSysButtonWidth;

    EQWindowType mWndType;

    std::wstring mTitle;

    bool mAcceptInput;

    bool mAlwaysOnTop;

    bool mSizeChangeable;
    
    bool mShowSelfWhenCreated;

    float mOpacity;

    TOnCloseListener mOnCloseListener;
};
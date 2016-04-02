#include "QWindow.h"
#include "Application/Application.h"
#include "Application/Window.h"
#include "QApplication.h"
#include "Renderer/Renderer.h"

QWindow::QWindow() : QWidget()
{
    mNativeWindow     = nullptr;
    mWndType             = EQWindowType::AppWindow;
    mTitle               = L"Qbert App Window";
    mAcceptInput         = true;
    mAlwaysOnTop         = false;
    mSizeChangeable      = true;
    mShowSelfWhenCreated = true;
    mOpacity             = .7f;
    mPosition            = Vector2D(200, 200);
    mSize                = Vector2D(800, 600);

    mBorderWidth = 6;
    mTitleBarHeight = 30;
    mSysButtonWidth = 30;

}

QWindow::QWindow(const std::wstring& title, EQWindowType type, const Vector2D& position, const Vector2D& size, float opacity) : QWindow()
{
    mWndType  = type;
    mTitle    = title;
    mPosition = position;
    mSize     = size;
    mOpacity  = opacity;
}

QWindow::~QWindow()
{

}

void QWindow::initialize()
{
    if (mWndType == EQWindowType::DockedChildWindow)
    {
        mNativeWindow = nullptr;
        return;
    }

    // Construct a native window definition according to QWindow properties.
    WindowDefinition def;
    def.DesiredPosX   = (int32_t)mPosition.x;
    def.DesiredPosY   = (int32_t)mPosition.y;
    def.DesiredWidth  = (int32_t)mSize.x;
    def.DesiredHeight = (int32_t)mSize.y;

    /* 
    def.HasOSWindowBorder      = true;
    def.SupportsTransparency   = false;
    def.AppearsInTaskbar       = (mWndType == EQWindowType::AppWindow ? true : false);
    def.IsTopmostWindow        = mAlwaysOnTop;
    def.AcceptsInput           = mAcceptInput;
    def.ActivateWhenFirstShown = true;
    def.SupportsMinimize       = mSizeChangeable;
    def.SupportsMaximize       = mSizeChangeable;
    def.IsRegularWindow        = true;
    def.HasSizingFrame         = mSizeChangeable;
    */

    def.HasOSWindowBorder      = false;
    def.SupportsTransparency   = true;
    def.AppearsInTaskbar       = (mWndType == EQWindowType::AppWindow ? true : false);
    def.IsTopmostWindow        = mAlwaysOnTop;
    def.AcceptsInput           = mAcceptInput;
    def.ActivateWhenFirstShown = true;
    def.IsRegularWindow        = true;
    def.SupportsMinimize       = mSizeChangeable;
    def.SupportsMaximize       = mSizeChangeable;
    def.HasSizingFrame         = mSizeChangeable;


    def.Title        = mTitle;
    def.Opacity      = mOpacity;
    def.CornerRadius = 0;

    mNativeWindow = Application::getInstance()->addWindow(def, mShowSelfWhenCreated);

    ::RECT rcRect;
    mNativeWindow->getClientRect(rcRect);
    mPosition.x = (float)rcRect.left;
    mPosition.y = (float)rcRect.top;
    mSize.x = (float)(rcRect.right - rcRect.left);
    mSize.y = (float)(rcRect.bottom - rcRect.top);

    Renderer::getInstance()->createGuiRenderTarget(mNativeWindow->getHwnd());
}

void QWindow::close()
{
    // OnCloseListener() returning true indicates we really want to close
    if (mOnCloseListener && !mOnCloseListener())
    {
    }
    else
    {
        if (mNativeWindow)
        {
            Renderer::getInstance()->removeGuiRenderTarget(mNativeWindow->getHwnd());

            Application::getInstance()->removeWindow(mNativeWindow);
        }

        // delete this
        QApplication::getInstance()->removeQWindow(this);
    }
}

void QWindow::resize(int32_t x, int32_t y)
{
    mSize.x = (float)x;
    mSize.y = (float)y;

    //mOSBackingWindow->reshape();
    WINDOWINFO winInfo = { 0 };
    winInfo.cbSize = sizeof winInfo;
    ::GetWindowInfo(mNativeWindow->getHwnd(), &winInfo);

    mPosition.x = (float)winInfo.rcClient.left;
    mPosition.y = (float)winInfo.rcClient.top;

    Renderer::getInstance()->resizeGuiRenderTarget(mNativeWindow->getHwnd(), Vector2D((float)x, (float)y));
}

void QWindow::minimize()
{
    mNativeWindow->minimize();
}

class Window* QWindow::getNativeWindow()
{
    return mNativeWindow;
}

void QWindow::drawRecursively(VisualElementGroup& group, QRectangle& drawRect)
{
    if (mNativeWindow)
    {
        group.Hwnd = mNativeWindow->getHwnd();
    }

    VisualElement titleBar;
    titleBar.ElementType   = VisualElement::QuadElement;
    titleBar.LayerId       = mLayer;
    titleBar.PixelPosition = drawRect.Position;
    titleBar.PixelSize     = Vector2D((float)(mSize.x - 3*mSysButtonWidth), (float)mTitleBarHeight);
    titleBar.ScalingFactor = 1.f;
    titleBar.Color         = Palette::QDark;
    titleBar.Tex           = nullptr;
    titleBar.ClipRect      = drawRect.toClipRect();
    group.Elements.push_back(titleBar);

    VisualElement sysButton;
    sysButton.ElementType   = VisualElement::QuadElement;
    sysButton.LayerId       = mLayer + 1;
    sysButton.ScalingFactor = 1.f;
    sysButton.Tex           = nullptr;
    sysButton.ClipRect      = drawRect.toClipRect();
    sysButton.PixelSize     = Vector2D((float)mSysButtonWidth, (float)mTitleBarHeight);

    // Minimize button
    sysButton.PixelPosition = titleBar.PixelPosition + Vector2D(titleBar.PixelSize.x + mSysButtonWidth*0, 0);
    sysButton.Color = Palette::QBlue_Dark;
    group.Elements.push_back(sysButton);

    // Maximize button
    sysButton.PixelPosition = titleBar.PixelPosition + Vector2D(titleBar.PixelSize.x + mSysButtonWidth*1, 0);
    sysButton.Color = Palette::QYellow;
    group.Elements.push_back(sysButton);

    // Close button
    sysButton.PixelPosition = titleBar.PixelPosition + Vector2D(titleBar.PixelSize.x + mSysButtonWidth*2, 0);
    sysButton.Color = Palette::QGray;
    group.Elements.push_back(sysButton);

    // Line Border
    VisualElement outline;
    outline.ElementType          = VisualElement::LineElement;
    outline.LayerId              = mLayer + 2;
    outline.PixelPosition        = Vector2D(1, 1);
    outline.PixelSize            = Vector2D();
    outline.ScalingFactor        = 1.f;
    outline.Color                = Palette::QBlue_Dark;
    outline.ClipRect             = drawRect.toClipRect();
    outline.LineAttr.Prefiltered = false;
    outline.LineAttr.Thickness   = 1.f; // no effect on plain lines
    outline.LineAttr.Points.push_back(Vector2D(0,         0 ));
    outline.LineAttr.Points.push_back(Vector2D(mSize.x-1, 0));
    outline.LineAttr.Points.push_back(Vector2D(mSize.x-1, mSize.y-1));
    outline.LineAttr.Points.push_back(Vector2D(0,         mSize.y-1));
    outline.LineAttr.Points.push_back(Vector2D(0,         0 ));
    group.Elements.push_back(outline);

    if (!mChildWidgets.empty())
    {
        // We assume QWindow does not have margins
        QRectangle childRect;
        childRect.Position = Vector2D(mPadding.Left, mPadding.Top + (float)mTitleBarHeight);
        childRect.Size     = Vector2D(drawRect.Size.x - mPadding.getHorizental(), drawRect.Size.y - (float)mTitleBarHeight - mPadding.getVertical());
        childRect.Layer    = mLayer + 1;
        childRect.ToRoot   = drawRect.Position + drawRect.ToRoot;

        mChildWidgets[0]->drawRecursively(group, childRect);
    }
}

#define isPointInBorders(x, y, left, right, top, bottom) (!(x < left || x > right || y < top || y > bottom))

EWindowZone QWindow::simulateOSHitTest(const int32_t x, const int32_t y) const
{
    const int32_t width = (int32_t)mSize.x;
    const int32_t height = (int32_t)mSize.y;

    uint32_t borderWidth = mNativeWindow->isMaximized() ? 0 : mBorderWidth;

    // Window
    const int32_t windowLB = borderWidth;
    const int32_t windowTB = borderWidth;
    const int32_t windowRB = width - borderWidth;
    const int32_t windowBB = height - borderWidth;

    // System Button
    static const uint32_t numSysButton = 3;

    // Make sure there's enough room for system buttons
    check((windowRB - windowLB) > (int32_t)(numSysButton * mSysButtonWidth));

    const int32_t sysButtonTB = windowTB;
    const int32_t sysButtonBB = windowTB + mTitleBarHeight;
    int32_t sysButtonRB[numSysButton];
    int32_t sysButtonLB[numSysButton];
    for (uint32_t i = 0; i < numSysButton; ++i)
    {
        sysButtonRB[i] = windowRB - i * mSysButtonWidth;
        sysButtonLB[i] = windowRB - (i + 1) * mSysButtonWidth;
    }

    // Title
    const int32_t titleLB = windowLB;
    const int32_t titleTB = windowTB;
    const int32_t titleRB = sysButtonLB[numSysButton - 1];
    const int32_t titleBB = windowTB + mTitleBarHeight;

    if (isPointInBorders(x, y, windowLB, windowRB, titleBB, windowBB))
        return EWindowZone::ClientArea;

    if (isPointInBorders(x, y, titleLB, titleRB, titleTB, titleBB))
        return EWindowZone::TitleBar;

    if (isPointInBorders(x, y, sysButtonLB[2], sysButtonRB[2], sysButtonTB, sysButtonBB))
        return EWindowZone::MinimizeButton;

    if (isPointInBorders(x, y, sysButtonLB[1], sysButtonRB[1], sysButtonTB, sysButtonBB))
        return EWindowZone::MaximizeButton;

    if (isPointInBorders(x, y, sysButtonLB[0], sysButtonRB[0], sysButtonTB, sysButtonBB))
        return EWindowZone::CloseButton;

    if (isPointInBorders(x, y, 0, windowLB, windowTB, windowBB))
        return EWindowZone::LeftBorder;

    if (isPointInBorders(x, y, windowLB, windowRB, 0, windowTB))
        return EWindowZone::TopBorder;

    if (isPointInBorders(x, y, windowRB, width, windowTB, windowBB))
        return EWindowZone::RightBorder;

    if (isPointInBorders(x, y, windowLB, windowRB, windowBB, height))
        return EWindowZone::BottomBorder;

    if (isPointInBorders(x, y, 0, windowLB, 0, windowTB))
        return EWindowZone::TopLeftBorder;

    if (isPointInBorders(x, y, windowRB, width, 0, windowTB))
        return EWindowZone::TopRightBorder;

    if (isPointInBorders(x, y, 0, windowLB, windowBB, height))
        return EWindowZone::BottomLeftBorder;

    if (isPointInBorders(x, y, windowRB, width, windowBB, height))
        return EWindowZone::BottomRightBorder;

    return EWindowZone::NotInWindow;
}



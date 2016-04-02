#include "Window.h"
#include "Application.h"
#include "Core/Math.h"
#include <Dwmapi.h> // Required dwmapi.lib

Window::Window() : 
    mWindowMode(EScreenMode::Windowed),
    mIsInitialized(false),
    mIsVisible(false),
    mHwnd(NULL),
    mRegionWidth(-1), 
    mRegionHeight(-1)
{
}

Window::~Window()
{
}

void Window::createStandardWindow(HINSTANCE hInst, Window* parent)
{
    int32_t x              = mDefinition.DesiredPosX;
    int32_t y              = mDefinition.DesiredPosY;
    int32_t clientWidth    = mDefinition.DesiredWidth;
    int32_t clientHeight   = mDefinition.DesiredHeight;
    int32_t windowWidth    = clientWidth;
    int32_t windowHeight   = clientHeight;
    uint32_t windowStyle   = 0;
    uint32_t windowExStyle = 0;

    windowStyle = WS_POPUP | WS_OVERLAPPED | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_BORDER | WS_CAPTION;
    windowExStyle = WS_EX_APPWINDOW;

    ::RECT windowRect = { 0, 0, clientWidth, clientHeight };
    // Calculate the overall window size based on client region size and window styles
    // windowRect: passed in as desired client size, passed out as calculated window size
    ::AdjustWindowRectEx(&windowRect, windowStyle, 0, windowExStyle);
    x += windowRect.left;
    y += windowRect.top;
    windowWidth = windowRect.right - windowRect.left;
    windowHeight = windowRect.bottom - windowRect.top;

    mHwnd = ::CreateWindowEx( windowExStyle, Window::getClassName(), mDefinition.Title.c_str(), windowStyle,
        x, y, windowWidth, windowHeight, parent ? parent->getHwnd() : NULL, NULL, hInst, NULL);

    reshape(x, y, clientWidth, clientHeight);

    if (mDefinition.SupportsTransparency)
        setOpacity(mDefinition.Opacity);
}

void Window::createCustomizedWindow(HINSTANCE hInst, Window* parent)
{
    int32_t x              = mDefinition.DesiredPosX;
    int32_t y              = mDefinition.DesiredPosY;
    int32_t clientWidth    = mDefinition.DesiredWidth;
    int32_t clientHeight   = mDefinition.DesiredHeight;
    int32_t windowWidth    = clientWidth;
    int32_t windowHeight   = clientHeight;
    uint32_t windowStyle   = 0;
    uint32_t windowExStyle = 0;

    windowStyle |= WS_POPUP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU;
    if (mDefinition.SupportsMaximize) windowStyle |= WS_MAXIMIZEBOX;
    if (mDefinition.SupportsMinimize) windowStyle |= WS_MINIMIZEBOX;
    if (mDefinition.HasSizingFrame) windowStyle |= WS_THICKFRAME;

    windowExStyle = WS_EX_WINDOWEDGE;
    if (mDefinition.SupportsTransparency) windowExStyle |= WS_EX_LAYERED;
    if (mDefinition.AppearsInTaskbar) windowExStyle |= WS_EX_APPWINDOW;
    else windowExStyle |= WS_EX_TOOLWINDOW;
    if (mDefinition.IsTopmostWindow) windowExStyle |= WS_EX_TOPMOST;
    if (mDefinition.AcceptsInput == false) windowExStyle |= WS_EX_TRANSPARENT;

    mHwnd = ::CreateWindowEx( windowExStyle, Window::getClassName(), mDefinition.Title.c_str(), windowStyle,
        x, y, windowWidth, windowHeight, parent ? parent->getHwnd() : NULL, NULL, hInst, NULL);

    reshape(x, y, clientWidth, clientHeight);

    vcheck(mHwnd != NULL, L"Win32 window creation failed.");

    if (mDefinition.SupportsTransparency)
        setOpacity(mDefinition.Opacity);

    // Border and title bar are removed by intercepting several messages(WM_NCACTIVATE, WM_NCCALCSIZE, WM_NCPAINT)
    // While this trick works, the default window region somehow falls back to the old vista style(round upper corners and 
    // sharp lower corners)
    // Set a rectangle region to look less weird on a win10 system
    updateWindowRegion();
}


void Window::initialize(const WindowDefinition& definition, HINSTANCE hInst, Window* parent)
{
    Application* app = Application::getInstance();

    vcheck(app->isInitialized(), L"Application must be initialized before creating any window.");

    mDefinition = definition;

    if (mDefinition.HasOSWindowBorder)
    {
        createStandardWindow(hInst, parent);
    }
    else
    {
        createCustomizedWindow(hInst, parent);
    }
/*

    // Interpret incoming window definition into native specifications
    int32_t x            = mDefinition.DesiredPosX;
    int32_t y            = mDefinition.DesiredPosY;
    int32_t clientWidth  = mDefinition.DesiredWidth;
    int32_t clientHeight = mDefinition.DesiredHeight;

    int32_t windowWidth  = clientWidth;
    int32_t windowHeight = clientHeight;

    uint32_t windowStyle = 0;
    uint32_t windowExStyle = 0;

    if (mDefinition.HasOSWindowBorder == false)
    {
        windowStyle = WS_POPUP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
        windowExStyle = WS_EX_WINDOWEDGE;
        if (mDefinition.SupportsTransparency)
            windowExStyle |= WS_EX_LAYERED;

        if (mDefinition.AppearsInTaskbar)
            windowExStyle |= WS_EX_APPWINDOW;
        else
            windowExStyle |= WS_EX_TOOLWINDOW;

        if (mDefinition.IsTopmostWindow)
            windowExStyle |= WS_EX_TOPMOST;

        if (mDefinition.AcceptsInput == false)
            windowExStyle |= WS_EX_TRANSPARENT;
    }
    else
    {
        windowStyle = WS_POPUP | WS_OVERLAPPED | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_BORDER | WS_CAPTION;
        windowExStyle = WS_EX_APPWINDOW;

        ::RECT windowRect = { 0, 0, clientWidth, clientHeight };
        // Calculate the overall window size based on client region size and window styles
        // windowRect: passed in as desired client size, passed out as calculated window size
        ::AdjustWindowRectEx(&windowRect, windowStyle, 0, windowExStyle);

        x += windowRect.left;
        y += windowRect.top;
        windowWidth = windowRect.right - windowRect.left;
        windowHeight = windowRect.bottom - windowRect.top;
    }

    mHwnd = ::CreateWindowEx(
        windowExStyle,
        Window::getClassName(),
        mDefinition.Title.c_str(),
        windowStyle,
        x, y, windowWidth, windowHeight,
        parent ? parent->getHwnd() : NULL,
        NULL, 
        hInst, 
        NULL);

    reshape(x, y, clientWidth, clientHeight);

    vcheck(mHwnd != NULL, L"Win32 window creation failed.");

    if (mDefinition.SupportsTransparency)
        setOpacity(mDefinition.Opacity);

#if WINVER > 0x502	// Windows Vista or greater required for DWM
    // Disable DWM Rendering and Nonclient Area painting if not showing the OS window border
    // This prevents the standard windows frame from ever being drawn
    if (!mDefinition.HasOSWindowBorder)
    {
        const DWMNCRENDERINGPOLICY RenderingPolicy = DWMNCRP_DISABLED;
        ::DwmSetWindowAttribute(mHwnd, DWMWA_NCRENDERING_POLICY, &RenderingPolicy, sizeof(RenderingPolicy));

        const BOOL bEnableNCPaint = false;
        ::DwmSetWindowAttribute(mHwnd, DWMWA_ALLOW_NCPAINT, &bEnableNCPaint, sizeof(bEnableNCPaint));
    }
#endif	// WINVER

    if (mDefinition.IsRegularWindow && !mDefinition.HasOSWindowBorder)
    {
        setWindowRegion(clientWidth, clientHeight);
        windowStyle |= WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU;

        if (mDefinition.SupportsMaximize) windowStyle |= WS_MAXIMIZEBOX;
        if (mDefinition.SupportsMinimize) windowStyle |= WS_MINIMIZEBOX;
        if (mDefinition.HasSizingFrame) windowStyle |= WS_THICKFRAME;

        ::SetWindowLongPtr(mHwnd, GWL_STYLE, windowStyle);

        setWindowRegion(clientWidth, clientHeight);
    }

*/
    if (mDefinition.IsRegularWindow)
    {
        // TODO registerDragDrop();
    }

    mIsInitialized = true;
}

void Window::close() 
{
}


void Window::reshape(int32_t newX, int32_t newY, int32_t newWidth, int32_t newHeight)
{
    WINDOWINFO winInfo = { 0 };
    winInfo.cbSize = sizeof winInfo;
    ::GetWindowInfo(mHwnd, &winInfo);

    ::RECT windowRect = { 0, 0, newWidth, newHeight };
    //if (mDefinition.HasOSWindowBorder)
    {
        ::AdjustWindowRectEx(&windowRect, winInfo.dwStyle, FALSE, winInfo.dwExStyle);
        newWidth = windowRect.right - windowRect.left;
        newHeight = windowRect.bottom - windowRect.top;
    }

    // MoveWindow(...) triggers a WM_SIZE
    ::MoveWindow(mHwnd, newX, newY, newWidth, newHeight, true);
}

void Window::setOpacity(float opacity)
{
    ::SetLayeredWindowAttributes(mHwnd, 0, (uint8_t)std::trunc(opacity * 255.0f), LWA_ALPHA);
}

void Window::updateWindowRegion()
{
    RECT rcWindow;
    GetWindowRect(mHwnd, &rcWindow);

    int32_t windowWidth = rcWindow.right - rcWindow.left;
    int32_t windowHeight = rcWindow.bottom - rcWindow.top;

    HRGN region;
    if (mDefinition.CornerRadius > 0)
    {
        region = ::CreateRoundRectRgn(0, 0, windowWidth, windowHeight, mDefinition.CornerRadius, mDefinition.CornerRadius);
    }
    else
    {
        region = ::CreateRectRgn(0, 0, windowWidth, windowHeight);
    }

    ::SetWindowRgn(mHwnd, region, FALSE);
}

HRGN Window::makeWindowRegionObject3() const
{
    RECT rcWindow;
    GetWindowRect(mHwnd, &rcWindow);

    HRGN region;
    region = ::CreateRectRgn(0, 0, rcWindow.right - rcWindow.left, rcWindow.bottom - rcWindow.top);
    return region;
}

HRGN Window::makeWindowRegionObject2() const
{
    WINDOWINFO winInfo = { 0 };
    winInfo.cbSize = sizeof winInfo;
    ::GetWindowInfo(mHwnd, &winInfo);

    int32_t borderSize = winInfo.cxWindowBorders;
    int32_t windowWidth = winInfo.rcWindow.right - winInfo.rcWindow.left;
    int32_t windowHeight = winInfo.rcWindow.bottom - winInfo.rcWindow.top;

    HRGN region;
    region = ::CreateRectRgn(borderSize, borderSize, windowWidth - borderSize, windowHeight - borderSize);
    return region;
}

HRGN Window::makeWindowRegionObject() const
{
    HRGN region;
    if (mRegionWidth != -1 && mRegionHeight != -1)
    {
        if (isMaximized())
        {
            int32_t borderSize = getBorderSize();
            region = ::CreateRectRgn(borderSize, borderSize, mRegionWidth - borderSize, mRegionHeight - borderSize);
        }
        else
        {
            if (mDefinition.CornerRadius > 0)
            {
                region = ::CreateRoundRectRgn(0, 0, mRegionWidth, mRegionHeight, mDefinition.CornerRadius, mDefinition.CornerRadius);
            }
            else
            {

                region = ::CreateRectRgn(0, 0, mRegionWidth, mRegionHeight);
            }
        }
    }
    else
    {
        ::RECT winRect;
        ::GetWindowRect(mHwnd, &winRect);
        region = ::CreateRectRgn(0, 0, winRect.right - winRect.left, winRect.bottom - winRect.top);
    }

    return region;
}

bool Window::isMaximized() const
{
    return !!::IsZoomed(mHwnd);
}

bool Window::isMinimized() const
{
    return !!::IsIconic(mHwnd);
}

int32_t Window::getBorderSize() const
{
    ::WINDOWINFO winInfo = { 0 };
    winInfo.cbSize = sizeof winInfo;
    ::GetWindowInfo(mHwnd, &winInfo);

    return winInfo.cxWindowBorders;
}


void Window::show()
{
    if (mIsVisible == false)
    {
        mIsVisible = true;
        // Do not activate windows that do not take input; e.g. tool-tips and cursor decorators
        // Also don't activate if a window wants to appear without activated.
        const bool shouldActivate = mDefinition.AcceptsInput && mDefinition.ActivateWhenFirstShown;
        /* SW_SHOWNA 8
        Displays the window in its current size and position.
        This value is similar to SW_SHOW, except that the window is not activated.*/
        ::ShowWindow(mHwnd, shouldActivate ? SW_SHOWNORMAL : SW_SHOWNA);

        ::UpdateWindow(mHwnd);
    }
}

void Window::hide()
{
    if (mIsVisible)
    {
        mIsVisible = false;
        ::ShowWindow(mHwnd, SW_HIDE);
    }
}

void Window::bringToFront()
{
    if (::IsIconic(mHwnd))
    {
        ::ShowWindow(mHwnd, SW_RESTORE);
    }
    else
    {
        ::SetActiveWindow(mHwnd);
    }
}

void Window::destroy()
{
    ::DestroyWindow(mHwnd);
}

void Window::minimize()
{
    ::ShowWindow(mHwnd, SW_MINIMIZE);
}

void Window::maximize()
{
    ::ShowWindow(mHwnd, SW_MAXIMIZE);
}

void Window::restore()
{
    ::ShowWindow(mHwnd, SW_RESTORE);
}

bool Window::isVisible() const
{
    return mIsVisible;
}

void Window::setFocus()
{
    if (::GetFocus() != mHwnd)
    {
        ::SetFocus(mHwnd);
    }
}

void Window::enable(bool shouldEnable) const
{
    // Enables or disables this window.  When disabled, a window will receive no input.       
    ::EnableWindow(mHwnd, shouldEnable);
}

bool Window::isPointInside(int32_t x, int32_t y) const
{
    bool result = false;

    HRGN region = makeWindowRegionObject();
    result = !!::PtInRegion(region, x, y);
    DeleteObject(region);

    return result;
}

bool Window::isRegularWindow() const
{
    return mDefinition.IsRegularWindow;
}

void Window::setTitle(const std::wstring& inTitle)
{
    ::SetWindowText(mHwnd, inTitle.c_str());
}

bool Window::isForegroundWindow() const
{
    return ::GetForegroundWindow() == mHwnd;
}

void Window::moveWindowTo(int32_t x, int32_t y)
{
    RECT winRect;
    ::GetWindowRect(mHwnd, &winRect);

    POINT clientPos;
    clientPos.x = 0;
    clientPos.y = 0;
    ::ClientToScreen(mHwnd, &clientPos);

    const int32_t xMoveDistance = x - clientPos.x;
    const int32_t yMoveDistance = y - clientPos.y;

    x = winRect.left + xMoveDistance;
    y = winRect.top + yMoveDistance;

    ::MoveWindow(mHwnd, x, y, winRect.right - winRect.left, winRect.bottom - winRect.top, true);
}

void Window::getFullScreenInfo(int32_t& x, int32_t& y, int32_t& width, int32_t& height) const
{
    bool bTrueFullscreen = (mWindowMode == EScreenMode::Fullscreen);

    // Grab current monitor data for sizing
    HMONITOR Monitor = ::MonitorFromWindow(mHwnd, bTrueFullscreen ? MONITOR_DEFAULTTOPRIMARY : MONITOR_DEFAULTTONEAREST);
    MONITORINFO MonitorInfo;
    MonitorInfo.cbSize = sizeof(MONITORINFO);
    GetMonitorInfo(Monitor, &MonitorInfo);

    x = MonitorInfo.rcMonitor.left;
    y = MonitorInfo.rcMonitor.top;
    width = MonitorInfo.rcMonitor.right - x;
    height = MonitorInfo.rcMonitor.bottom - y;
}

const WindowDefinition& Window::getDefinition() const
{
    return mDefinition;
}

bool Window::isInitialized() const
{
    return mIsInitialized;
}

void Window::getClientRect(::RECT& outRect) const
{
    WINDOWINFO winInfo = { 0 };
    winInfo.cbSize = sizeof winInfo;
    ::GetWindowInfo(mHwnd, &winInfo);
    outRect = winInfo.rcClient;
}


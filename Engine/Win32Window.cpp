#include "Win32Window.h"
#include "Win32Application.h"

#include "Core/Math.h"
#include "Core/Util.h"

#include <Dwmapi.h> // Required dwmapi.lib


/*
Win32 Quick Reference

    ----SetWindowRgn----
    Sets the window region of a window. The window region determines the area within the window where the system 
    permits drawing. The system does not display any portion of a window that lies outside of the window region.

    When this function is called, the system sends the WM_WINDOWPOSCHANGING and WM_WINDOWPOSCHANGED messages to the window.

    The coordinates of a window's window region are relative to the upper-left corner of the window, not the 
    client area of the window.

    ----GetWindowRect---- 
    Retrieves the dimensions of the bounding rectangle of the specified window. The dimensions are given in screen 
    coordinates that are relative to the upper-left corner of the screen.

    ----ClientToScreen----
    converts the client-area coordinates of a specified point to screen coordinates.

    ----MoveWindow----
    Changes the position and dimensions of the specified window. For a top-level window, the position and dimensions 
    are relative to the upper-left corner of the screen. For a child window, they are relative to the upper-left corner
    of the parent window's client area.

    If the bRepaint parameter is TRUE, the system sends the WM_PAINT message to the window procedure immediately after 
    moving the window (that is, the MoveWindow function calls the UpdateWindow function). If bRepaint is FALSE, the 
    application must explicitly invalidate or redraw any parts of the window and parent window that need redrawing.

    MoveWindow sends the WM_WINDOWPOSCHANGING, WM_WINDOWPOSCHANGED, WM_MOVE, WM_SIZE, and WM_NCCALCSIZE messages 
    to the window.

    ----UpdateWindow---- 
    updates the client area of the specified window by sending a WM_PAINT message to the window if the window's update 
    region is not empty. The function sends a WM_PAINT message directly to the window procedure of the specified window, 
    bypassing the application queue. If the update region is empty, no message is sent.

    ----ShowWindow---- 
    Sets the specified window's show state.
*/


LPCWSTR Win32Window::ClassName = L"Win32Window";

Win32Window::Win32Window() : 
    mHwnd(NULL), 
    mOwningApp(nullptr), 
    mRegionWidth(-1), mRegionHeight(-1),
    mIsVisible(false),
    mIsInitialized(false),
    mWindowMode(EWindowMode::Windowed)
{}


void Win32Window::initialize(Win32Application* owningApp, const Win32WindowDefinition& winDef, HINSTANCE hInst, Win32Window* parent )
{
    mOwningApp = owningApp;
    mDefinition = winDef;

    int32_t x            = (int32_t)std::trunc(mDefinition.DesiredPosX);
    int32_t y            = (int32_t)std::trunc(mDefinition.DesiredPosY);
    int32_t clientWidth  = (int32_t)std::trunc(mDefinition.DesiredWidth);
    int32_t clientHeight = (int32_t)std::trunc(mDefinition.DesiredHeight);
    int32_t windowWidth  = clientWidth;
    int32_t windowHeight = clientHeight;

    // Resolve window definition
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

        ::RECT windowRect = { 0, 0, clientWidth, clientHeight};
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
        Win32Window::ClassName,
        mDefinition.Title.c_str(),
        windowStyle,
        x, y, windowWidth, windowHeight,
        parent ? static_cast<HWND>(parent->mHwnd) : NULL,
        NULL, hInst, NULL);

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
        vcheck_hr(
            ::DwmSetWindowAttribute(mHwnd, DWMWA_NCRENDERING_POLICY, &RenderingPolicy, sizeof(RenderingPolicy)),
            L"Failed to disable DWM rendering"
            );

        const BOOL bEnableNCPaint = false;
        vcheck_hr(
            ::DwmSetWindowAttribute(mHwnd, DWMWA_ALLOW_NCPAINT, &bEnableNCPaint, sizeof(bEnableNCPaint)),
            L"Failed to disable non-client area painting"
            );
    }
#endif	// WINVER

    if (mDefinition.IsRegularWindow && !mDefinition.HasOSWindowBorder)
    {
        windowStyle |= WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU;
        if (mDefinition.SupportsMaximize)
            windowStyle |= WS_MAXIMIZEBOX;

        if (mDefinition.SupportsMinimize)
            windowStyle |= WS_MINIMIZEBOX;

        if (mDefinition.HasSizingFrame)
            windowStyle |= WS_THICKFRAME;

        ::SetWindowLong(mHwnd, GWL_STYLE, windowStyle);

        setWindowRegion(clientWidth, clientHeight);
    }

    if (mDefinition.IsRegularWindow)
    {
        // TODO registerDragDrop();
    }
    mIsInitialized = true;
}

void Win32Window::reshape(int32_t newX, int32_t newY, int32_t newWidth, int32_t newHeight)
{
    WINDOWINFO winInfo = { 0 };
    winInfo.cbSize = sizeof winInfo;

    ::GetWindowInfo(mHwnd, &winInfo);
    ::RECT windowRect = { 0, 0, newWidth, newHeight };
    if (mDefinition.HasOSWindowBorder)
    {
        ::AdjustWindowRectEx(&windowRect, winInfo.dwStyle, FALSE, winInfo.dwExStyle);
        newWidth = windowRect.right - windowRect.left;
        newHeight = windowRect.bottom - windowRect.top;
    }

    // MoveWindow(...) triggers a WM_SIZE
    ::MoveWindow(mHwnd, newX, newY, newWidth, newHeight, true);
}

void Win32Window::setOpacity(float opacity)
{
    ::SetLayeredWindowAttributes(mHwnd, 0, (uint8_t)Math::trunc(opacity * 255.0f), LWA_ALPHA);
}

void Win32Window::setWindowRegion(int32_t width, int32_t height)
{
    mRegionWidth = width;
    mRegionHeight = height;

    HRGN region = makeWindowRegionObject();

    ::SetWindowRgn(mHwnd, region, FALSE);
}

HRGN Win32Window::makeWindowRegionObject() const
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
                region = ::CreateRoundRectRgn(0, 0, mRegionWidth, mRegionHeight, mDefinition.CornerRadius, mDefinition.CornerRadius);
            else
                region = ::CreateRectRgn(0, 0, mRegionWidth, mRegionHeight);
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

bool Win32Window::isMaximized() const
{
    bool maximized = !!::IsZoomed(mHwnd);
    return maximized;
}

int32_t Win32Window::getBorderSize() const
{
    ::WINDOWINFO winInfo = { 0 };
    winInfo.cbSize = sizeof winInfo;
    ::GetWindowInfo(mHwnd, &winInfo);

    return winInfo.cxWindowBorders;
}


void Win32Window::show()
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

void Win32Window::hide()
{
    if (mIsVisible)
    {
        mIsVisible = false;
        ::ShowWindow(mHwnd, SW_HIDE);
    }
}

void Win32Window::bringToFront()
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


void Win32Window::destroy()
{
    ::DestroyWindow(mHwnd);
}

void Win32Window::minimize()
{
    ::ShowWindow(mHwnd, SW_MINIMIZE);
}

void Win32Window::maximize()
{
    ::ShowWindow(mHwnd, SW_MAXIMIZE);
}

void Win32Window::restore()
{
    ::ShowWindow(mHwnd, SW_RESTORE);
}

bool Win32Window::isVisible() const
{
    return mIsVisible;
}

void Win32Window::setFocus()
{
    if (::GetFocus() != mHwnd)
    {
        ::SetFocus(mHwnd);
    }
}

void Win32Window::enable(bool shouldEnable) const
{
    // Enables or disables this window.  When disabled, a window will receive no input.       
    ::EnableWindow(mHwnd, shouldEnable);
}

bool Win32Window::isPointInside(int32_t x, int32_t y) const
{
    bool result = false;

    HRGN region = makeWindowRegionObject();
    result = !!::PtInRegion(region, x, y);
    DeleteObject(region);

    return result;
}

bool Win32Window::isRegularWindow() const
{
    return mDefinition.IsRegularWindow;
}

void Win32Window::setTitle(const std::wstring& inTitle)
{
    ::SetWindowText(mHwnd, inTitle.c_str());
}

bool Win32Window::isForegroundWindow() const
{
    return ::GetForegroundWindow() == mHwnd;
}

void Win32Window::moveWindowTo(int32_t x, int32_t y)
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

void Win32Window::getFullScreenInfo(int32_t& x, int32_t& y, int32_t& width, int32_t& height) const
{
    bool bTrueFullscreen = (mWindowMode== EWindowMode::Fullscreen);

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

const Win32WindowDefinition& Win32Window::getDefinition() const
{
    return mDefinition;
}

bool Win32Window::isInitialized() const
{
    return mIsInitialized;
}


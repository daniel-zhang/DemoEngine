#pragma once

#include "Core/Util.h"
#include <functional>

struct WindowDefinition
{
    WindowDefinition() :
        DesiredPosX(300),
        DesiredPosY(100),
        DesiredWidth(800),
        DesiredHeight(400),

        HasOSWindowBorder(true),
        SupportsTransparency(true),
        AppearsInTaskbar(true),
        IsTopmostWindow(true),
        AcceptsInput(true),
        ActivateWhenFirstShown(true),
        SupportsMinimize(true),
        SupportsMaximize(true),
        IsRegularWindow(true),
        HasSizingFrame(true),

        Title(L"Default Window"),
        Opacity(1.f),
        CornerRadius(0)
    {
    }

    int32_t DesiredPosX;
    int32_t DesiredPosY;
    int32_t DesiredWidth;
    int32_t DesiredHeight;

    bool HasOSWindowBorder;
    bool SupportsTransparency;
    bool AppearsInTaskbar;
    bool IsTopmostWindow;
    bool AcceptsInput;
    bool ActivateWhenFirstShown;

    bool SupportsMinimize;
    bool SupportsMaximize;

    bool IsRegularWindow;
    bool HasSizingFrame;

    std::wstring Title;
    float Opacity;
    int32_t CornerRadius;
};

enum EScreenMode
{
    /** The window is in true fullscreen mode */
    Fullscreen,
    /** The window has no border and takes up the entire area of the screen */
    WindowedFullscreen,
    /** The window has a border and may not take up the entire screen area */
    Windowed
};

class Window
{
public:
    static LPCWSTR getClassName(){ return L"DemoEngineNativeWindow"; }

    typedef std::function<bool(void)> TOnCloseListener;

public:
    Window();

    ~Window();

    void initialize(const WindowDefinition& definition, HINSTANCE hInst, Window* parent);

    void close();

    HWND getHwnd() const { return mHwnd; }

public:
    // Wrappers for native window operations
    void reshape(int32_t newX, int32_t newY, int32_t newWidth, int32_t newHeight);

    void moveWindowTo(int32_t x, int32_t y);

    void enable(bool shouldEnable) const;

    void show();

    void hide();

    void bringToFront();

    void destroy();

    void minimize();

    void maximize();

    void restore();

    void updateWindowRegion();

    void setWindowMode(EScreenMode NewWindowMode); // TODO

    void setFocus();

    void setOpacity(float opacity);

    void setTitle(const std::wstring& inTitle);

    bool isInitialized() const;

    bool isVisible() const;

    bool isMaximized() const;

    bool isMinimized() const;

    bool isPointInside(int32_t x, int32_t y) const;

    bool isRegularWindow() const;

    bool isForegroundWindow() const;

    void getFullScreenInfo(int32_t& x, int32_t& y, int32_t& width, int32_t& height) const;

    EScreenMode getWindowMode() const; //TODO

    int32_t getBorderSize() const;

    void getClientRect(::RECT& outRect) const;

    const WindowDefinition& getDefinition() const;

private:
    HRGN makeWindowRegionObject() const;
    HRGN makeWindowRegionObject2() const;
    HRGN makeWindowRegionObject3() const;

    void createStandardWindow(HINSTANCE hInst, Window* parent);

    void createCustomizedWindow(HINSTANCE hInst, Window* parent);

private:
    WindowDefinition mDefinition;

    EScreenMode mWindowMode;

    bool mIsInitialized;

    bool mIsVisible;

    HWND mHwnd;

    int32_t mRegionWidth;

    int32_t mRegionHeight;
};
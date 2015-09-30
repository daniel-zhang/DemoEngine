#pragma once

#include <Windows.h>
#include "Core/CommonTypes.h"

struct Win32WindowDefinition 
{
    float DesiredPosX;
    float DesiredPosY;
    float DesiredWidth;
    float DesiredHeight;

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
    bool SizeWillChangeOften;
    int32_t ExpectedMaxWidth;
    int32_t ExpectedMaxHeight;

    std::wstring Title;
    float Opacity;
    int32_t CornerRadius;
};
enum EWindowMode
{
    /** The window is in true fullscreen mode */
    Fullscreen,
    /** The window has no border and takes up the entire area of the screen */
    WindowedFullscreen,
    /** The window has a border and may not take up the entire screen area */
    Windowed
};

class Win32Application;

/*
The native window that is backing our game window. 
*/

class Win32Window
{
public:
    ~Win32Window(){}

public:
    void destroy();

    void reshape(int32_t newX, int32_t newY, int32_t newWidth, int32_t newHeight);

    void moveWindowTo(int32_t x, int32_t y);

    void enable(bool shouldEnable) const;

    void show();

    void hide();

    void bringToFront();

    void minimize();

    void maximize();

    void restore();

    void setWindowRegion(int32_t width, int32_t height);

    void setWindowMode(EWindowMode NewWindowMode); // TODO

    void setFocus();

    void setOpacity(float opacity);

    void setTitle(const std::wstring& inTitle);
    
    bool isInitialized() const;

    bool isVisible() const;

    bool isMaximized() const;

    bool isPointInside(int32_t x, int32_t y) const;

    bool isRegularWindow() const;

    bool isForegroundWindow() const;

    HWND getHwnd() const { return mHwnd; }

    void getFullScreenInfo(int32_t& x, int32_t& y, int32_t& width, int32_t& height) const;

    EWindowMode getWindowMode() const; //TODO

    int32_t getBorderSize() const;

    const Win32WindowDefinition& getDefinition() const;

protected:
    Win32Window();

    // Only allow access from Win32Application
    friend class Win32Application;
    static Win32Window* createUninitialized() { return new Win32Window(); }
    void initialize (Win32Application* owningApp, const Win32WindowDefinition& winDef, HINSTANCE hInst, Win32Window* parent);

    static LPCWSTR ClassName;

protected:
    HRGN makeWindowRegionObject() const;

protected:
    Win32WindowDefinition mDefinition;
    Win32Application* mOwningApp;
    HWND mHwnd;
    int32_t mRegionWidth;
    int32_t mRegionHeight;

    bool mIsInitialized;
    bool mIsVisible;
    EWindowMode mWindowMode;
};


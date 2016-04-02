#pragma once

#include "Core/Util.h"
#include "Win32Window.h"
#include "MessageHandler.h"

#include <Windows.h>
#include <set>

class ModifierKeysState
{
public:

    ModifierKeysState(const bool bInIsLeftShiftDown,
        const bool bInIsRightShiftDown,
        const bool bInIsLeftControlDown,
        const bool bInIsRightControlDown,
        const bool bInIsLeftAltDown,
        const bool bInIsRightAltDown)
        : mIsLeftShiftDown(bInIsLeftShiftDown),
        mIsRightShiftDown(bInIsRightShiftDown),
        mIsLeftControlDown(bInIsLeftControlDown),
        mIsRightControlDown(bInIsRightControlDown),
        mIsLeftAltDown(bInIsLeftAltDown),
        mIsRightAltDown(bInIsRightAltDown)
    {}

    bool isShiftDown() const { return mIsLeftShiftDown || mIsRightShiftDown; }
    bool isLeftShiftDown() const { return mIsLeftShiftDown; }
    bool isRightShiftDown() const { return mIsRightShiftDown; }
    bool isControlDown() const { return mIsLeftControlDown || mIsRightControlDown; }
    bool isLeftControlDown() const { return mIsLeftControlDown; }
    bool isRightControlDown() const { return mIsRightControlDown; }
    bool isAltDown() const { return mIsLeftAltDown || mIsRightAltDown; }
    bool isLeftAltDown() const { return mIsLeftAltDown; }
    bool isRightAltDown() const { return mIsRightAltDown; }

private:
    bool mIsLeftShiftDown;
    bool mIsRightShiftDown;
    bool mIsLeftControlDown;
    bool mIsRightControlDown;
    bool mIsLeftAltDown;
    bool mIsRightAltDown;
};

enum EPopUpOrientation
{
    Horizontal,
    Vertical,
};

struct Win32Rect
{
    int32_t Left;
    int32_t Top;
    int32_t Right;
    int32_t Bottom;
};

struct PhysicalMonitorInfo
{
    std::wstring Name;
    std::wstring ID;
    int32_t NativeWidth;
    int32_t NativeHeight;
    bool IsPrimary;
};

struct DisplayMetrics
{
    DisplayMetrics() {}

    int32_t PrimaryDisplayWidth;

    int32_t PrimaryDisplayHeight;

    std::vector<PhysicalMonitorInfo> MonitorInfo;

    /** Area of the primary display not covered by task bars or other docked widgets */
    Win32Rect PrimaryDisplayWorkAreaRect;

    /** Virtual display coordinate range (includes all active displays) */
    Win32Rect VirtualDisplayRect;
};

struct Win32Message
{
    Win32Message(Win32Window* win32Window, HWND hwnd, uint32_t msg, WPARAM wparam, LPARAM lparam, 
    int32_t mouseX = 0, int32_t mouseY = 0) :
        Wnd(win32Window),
        Hwnd(hwnd), 
        Msg(msg), 
        WParam(wparam), 
        LParam(lparam),
        MouseX(mouseX),
        MouseY(mouseY) 
    {}

    /** Native window that received the message */
    Win32Window* Wnd;
    /** Window handle */
    HWND Hwnd;
    /** Message code */
    uint32_t Msg;
    /** Message data */
    WPARAM WParam;
    LPARAM LParam;
    /** Mouse coordinates */
    int32_t MouseX;
    int32_t MouseY;
};

class Win32Application
{
public:
    ~Win32Application();
    
    static void initialize(MessageHandler* msgHandler, const HINSTANCE instanceHandle, const HICON hIcon)
    {
        if (mAppInstance == nullptr)
            mAppInstance = new Win32Application(msgHandler, instanceHandle, hIcon);
    }

    static Win32Application* getInstance() { return mAppInstance; }

public:
    void setMessageHandler(MessageHandler* msgHandler){ mMsgHandler = msgHandler; }

    Win32Window* createWin32Window();

    void addWin32Window(const Win32WindowDefinition& definition, Win32Window* wnd, Win32Window* parent);

    bool pumpMsg();

public:
    void setCapture(const Win32Window* win32Window);

    void* getCapture();

    ModifierKeysState getModifierKeys() const;

    void getDisplayMetrics(DisplayMetrics& outDisplayMetrics) const;

    void getInitialDisplayMetrics(DisplayMetrics& outDisplayMetrics) const;

private:
    // Make singleton
    Win32Application(const Win32Application&);
    Win32Application& operator=(const Win32Application&);
    Win32Application(MessageHandler* msgHandler, const HINSTANCE instanceHandle, const HICON hIcon);

private:
    void registerWin32Window(const HINSTANCE hInstance, const HICON hIcon);

    static LRESULT CALLBACK wndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    int32_t processMsg(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    Win32Window* getWin32WindowByHwnd(HWND hwnd);

    bool removeWin32Window(const Win32Window* wnd);

    static bool isInputMessage(uint32_t msg);

private:
    static Win32Application* mAppInstance;

    HINSTANCE mInstanceHandle;
    HICON mIconHandle;
    MessageHandler* mMsgHandler;

    DisplayMetrics mInitialDisplayMetrics;
    std::vector<Win32Window*> mWin32Windows;
    std::set<int32_t> mPressedModifierKeys;
    bool mIsInModalSizingLoop;
};
#pragma once

#include "Core/Util.h"
#include "MessageHandler.h"
#include <set>

/* 
The current application framework is unnecessarily complicated. This is a simplified and standalone version, with roughly
the same interface and a much more reasonable implementation.
*/

class ModifierKeysState
{
public:
    ModifierKeysState(){}

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

struct Win32Rect
{
    int32_t Left;
    int32_t Top;
    int32_t Right;
    int32_t Bottom;
};

struct DisplayInfo
{
    DisplayInfo() {}

    int32_t PrimaryDisplayWidth;

    int32_t PrimaryDisplayHeight;

    /** Area of the primary display not covered by task bars or other docked widgets */
    Win32Rect PrimaryDisplayWorkAreaRect;

    /** Virtual display coordinate range (includes all active displays) */
    Win32Rect VirtualDisplayRect;
};

class Application : public MessageHandler
{
public:
    static Application* getInstance()
    {
        static Application app;
        return &app;
    }

    ~Application();

private:
    // Make singleton
    Application();
    Application(const Application&); 
    Application& operator=(const Application&);

public:
    bool isInitialized() const { return mIsInitialized; }

    void initialize(const HICON hIcon = nullptr, MessageHandler* msgHandler = nullptr);

    bool hasWindows() { return !mWindows.empty(); }

    bool pumpMsg();
    
    Window* addWindow(const WindowDefinition& definition, bool showImmediately = true, Window* parent = nullptr);

    Window* addWindow(Window* window, bool showImmediately = true);

    void removeWindow(Window* window);

    Window* getWindowByHwnd(HWND hwnd);

    void setCapture(Window* window);

    Window* getCapture();
    
    ModifierKeysState getModifierKeys() const;

    bool isInModalLoop() const { return mIsInModalSizingLoop; }

public:
    // MessageHandler interface implementation

private:
    static LRESULT CALLBACK wndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    void registerWndProc();

    void extractDisplayInfo();

    int32_t processMsg(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
    MessageHandler* mMsgHandler;

    std::vector<Window*> mWindows;

    bool mIsInitialized;
    bool mIsActivated;
    bool mIsInModalSizingLoop;

    HINSTANCE mThreadInstance;
    HICON mIcon;
    HCURSOR mCursorArrow;
    HCURSOR mCursorCross;
    HCURSOR mCursorNESW;
    HCURSOR mCursorNWSE;
    HCURSOR mCursorNS;
    HCURSOR mCursorWE;

    DisplayInfo mDisplayInfo;
    std::set<int32_t> mPressedModifierKeys;
};

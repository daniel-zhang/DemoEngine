#pragma once

#include "Window.h"

enum EMouseButtons
{
    Left = 0,
    Middle,
    Right,
    Thumb01,
    Thumb02,

    Invalid,
};

enum EWindowActivation
{
    Activate = 0,
    ActivateByMouse,
    Deactivate
};

/**
* The Window Zone is the window area we are currently over to send back to the operating system
* for operating system compliance.
*/
enum EWindowZone
{
    NotInWindow       = 0,
    TopLeftBorder     = 1,
    TopBorder         = 2,
    TopRightBorder    = 3,
    LeftBorder        = 4,
    ClientArea        = 5,
    RightBorder       = 6,
    BottomLeftBorder  = 7,
    BottomBorder      = 8,
    BottomRightBorder = 9,
    TitleBar          = 10,
    MinimizeButton    = 11,
    MaximizeButton    = 12,
    CloseButton       = 13,
    SysMenu           = 14,

    /** No zone specified */
    Unspecified = 15,
};

enum EWindowAction
{
    ClickedNonClientArea = 1,
    Maximize             = 2,
    Restore              = 3,
    WindowMenu           = 4,
};

class MessageHandler
{
public:
    virtual ~MessageHandler() {}

    virtual bool enableUserInput(Window* wnd) const
    {
        return false;
    }

    virtual void onKeyChar(const TCHAR character, const bool isRepeat)
    {
    }

    virtual void onKeyDown(const int32_t keyCode, const uint32_t characterCode, const bool isRepeat)
    {
    }

    virtual void onKeyUp(const int32_t keyCode, const uint32_t characterCode, const bool isRepeat)
    {
    }

    virtual void onMouseDown(Window* wnd, const EMouseButtons button)
    {
    }

    virtual void onMouseUp(const EMouseButtons button)
    {
    }

    virtual void onMouseDoubleClick(Window* wnd, const EMouseButtons button)
    {
    }

    virtual void onMouseWheel(const float delta)
    {
    }

    virtual void onMouseMove()
    {
    }

    virtual void onRawMouseMove(const int32_t x, const int32_t y)
    {
    }

    virtual void onCursorSet()
    {
    }

    virtual void onWindowResized(Window* wnd, const int32_t width, const int32_t height, bool bWasMinimized = false)
    {

    }

    virtual void onOSPaint(Window* wnd)
    {
    }

    virtual void onWindowResizing(Window* wnd)
    {

    }

    virtual void enterModalSizeMove(Window* wnd)
    {

    }

    virtual void exitModalSizeMove(Window* wnd)
    {

    }

    virtual void onModalTimerEvent(Window* wnd)
    {

    }

    virtual void onWindowMoved(Window* wnd, const int32_t x, const int32_t y)
    {

    }

    virtual void onWindowActivationChanged(Window* wnd, const EWindowActivation activationType)
    {

    }

    virtual void onApplicationActivationChanged(const bool isActive)
    {
    }

    virtual EWindowZone getWindowZoneForPoint(Window* wnd, const int32_t x, const int32_t y)
    {
        //return EWindowZone::NotInWindow;
        return EWindowZone::ClientArea;
    }

    virtual void onWindowClose(Window* wnd)
    {

    }

    virtual bool onWindowAction(Window* wnd, const EWindowAction inActionType)
    {
        return true;
    }
};


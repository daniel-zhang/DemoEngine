#pragma once

#include "MessageHandler.h"
#include "Application/Application.h"
#include "Core/StringFunction.h"

class DebugMessageHandler : public MessageHandler
{
public:
    DebugMessageHandler()
    {
        OutputDebugString(L">> DebugMessageHandler Started\n\n");
    }
    ~DebugMessageHandler()
    {
        OutputDebugString(L">> DebugMessageHandler Ended\n\n");
    }

    virtual bool enableUserInput(Window* wnd) const
    {
        return true;
    }

    virtual void onKeyChar(const TCHAR character, const bool isRepeat)
    {
        OutputDebugString(wstring_format(
            L">> key char, char: %c, repeat: %s\n", character, isRepeat ? L"Yes" : L"No"
            ).c_str());
    }

    virtual void onKeyDown(const int32_t keyCode, const uint32_t characterCode, const bool isRepeat)
    {
        OutputDebugString(wstring_format(
            L">> key down, keycode: %i, charcode: %c, repeat: %s\n", keyCode, characterCode, (isRepeat ? L"Yes" : L"No")
            ).c_str());
    }

    virtual void onKeyUp(const int32_t keyCode, const uint32_t characterCode, const bool isRepeat)
    {
        OutputDebugString(wstring_format(
            L">> key up, keycode: %i, charcode: %c, repeat: %s\n", keyCode, characterCode, isRepeat ? L"Yes" : L"No"
            ).c_str());
    }

    virtual void onMouseDown(Window* wnd, const EMouseButtons button)
    {
        OutputDebugString(wstring_format(
            L">> Mouse down on %s\n", getIdStr(wnd).c_str() 
            ).c_str());
    }

    virtual void onMouseUp(const EMouseButtons button)
    {
        OutputDebugString( L">> Mouse up\n" );
    }

    virtual void onMouseDoubleClick(Window* wnd, const EMouseButtons button)
    {
        OutputDebugString(wstring_format(
            L">> Mouse double click on %s\n", getIdStr(wnd).c_str() 
            ).c_str());
    }

    virtual void onMouseWheel(const float delta)
    {
        OutputDebugString(wstring_format(
            L">> Mouse wheel: %f\n", delta
            ).c_str());
    }

    virtual void onMouseMove()
    {
    }

    virtual void onRawMouseMove(const int32_t x, const int32_t y)
    {
    }

    virtual void onCursorSet()
    {
        //OutputDebugString(L">> onCursorSet() called\n");
    }

    virtual void onWindowResized(Window* wnd, const int32_t width, const int32_t height, bool bWasMinimized = false)
    {
        OutputDebugString(wstring_format(
            L">> %s size changed to (%i, %i)\n", getIdStr(wnd).c_str(), width, height
            ).c_str());
    }

    virtual void onOSPaint(Window* wnd)
    {
        OutputDebugString(wstring_format(
            L">> onOSPaint() called on %s\n", getIdStr(wnd).c_str()
            ).c_str());
    }

    virtual void onWindowResizing(Window* wnd)
    {
        OutputDebugString(wstring_format(
            L">> onResizingWindow() called on %s\n", getIdStr(wnd).c_str()
            ).c_str());
    }

    virtual void enterModalSizeMove(Window* wnd)
    {
        OutputDebugString(wstring_format(
            L">> beginReshapingWindow() called on %s\n", getIdStr(wnd).c_str()
            ).c_str());
    }

    virtual void exitModalSizeMove(Window* wnd)
    {
        OutputDebugString(wstring_format(
            L">> finishedReshapingWindow() called on %s\n", getIdStr(wnd).c_str()
            ).c_str());
    }

    virtual void onWindowMoved(Window* wnd, const int32_t x, const int32_t y)
    {
        OutputDebugString(wstring_format(
            L">> %s moved to (%i, %i)\n", getIdStr(wnd).c_str(), x, y
            ).c_str());
    }

    virtual void onWindowActivationChanged(Window* wnd, const EWindowActivation activationType)
    {
        OutputDebugString(wstring_format(
            L">> %s: %s\n", getIdStr(wnd).c_str(), getActivationStr(activationType)
            ).c_str());
    }

    virtual void onApplicationActivationChanged(const bool isActive)
    {
        OutputDebugString(wstring_format(L">> Application: %s", isActive?L"activated\n":L"deactivated\n").c_str());
    }

    virtual EWindowZone getWindowZoneForPoint(Window* wnd, const int32_t x, const int32_t y)
    {
        //return EWindowZone::NotInWindow;
        OutputDebugString(wstring_format(
            L">> OS query window zone for: %s\n", getIdStr(wnd).c_str()
            ).c_str());

        //return EWindowZone::ClientArea;
        return EWindowZone::TitleBar;
    }

    virtual void onWindowClose(Window* wnd) override
    {
        OutputDebugString( wstring_format( 
            L">> User tried to close: %s\n", getIdStr(wnd).c_str() 
            ).c_str() );

        Application::getInstance()->removeWindow(wnd);
    }

    virtual bool onWindowAction(Window* wnd, const EWindowAction inActionType)
    {
        OutputDebugString( wstring_format( 
            L">> onWindowAction() for %s as: %s\n", getIdStr(wnd).c_str(), getActionStr(inActionType) 
            ).c_str() );

        return true;
    }

private:
    std::wstring getIdStr(Window* wnd)
    {
        return wstring_format(L"\"%s\"(hwnd %i)", wnd->getDefinition().Title.c_str(), wnd->getHwnd());
    }

    const wchar_t* getMouseButtonStr(EMouseButtons e)
    {
        switch (e)
        {
        case Left: return L"LMB";
        case Middle: return L"MMB";
        case Right: return L"RMB";
        default: return L"InvalidMouseButton";
        }
    }

    const wchar_t* getActivationStr(EWindowActivation e)
    {
        switch (e)
        {
        case Activate: return L"Activate";
        case ActivateByMouse: return L"ActivateByMouse";
        case Deactivate: return L"Deactivate";
        default: return L"Invalid action";
        }
    }

    const wchar_t* getWndZoneStr(EWindowZone e)
    {
        switch (e)
        {
        case NotInWindow: return L"NotInWindow";

        case TopLeftBorder: return L"TopLeftBorder"; return L"TopLeftBorder";

        case TopBorder: return L"TopBorder"; return L"TopBorder";

        case TopRightBorder: return L"TopRightBorder";

        case LeftBorder: return L"LeftBorder";

        case ClientArea: return L"ClientArea";

        case RightBorder: return L"RightBorder";

        case BottomLeftBorder: return L"BottomLeftBorder";

        case BottomBorder: return L"BottomBorder";

        case BottomRightBorder: return L"BottomRightBorder";

        case TitleBar: return L"TitleBar";

        case MinimizeButton: return L"MinimizeButton";

        case MaximizeButton: return L"MaximizeButton";

        case CloseButton: return L"CloseButton";

        case SysMenu: return L"SysMenu";

        case Unspecified: 
        default:
            return L"Unspecified";
        }
    }

    const wchar_t* getActionStr(EWindowAction e)
    {
        switch (e)
        {
        case ClickedNonClientArea: return L"ClickedNonClientArea";

        case Maximize: return L"Maximize";

        case Restore: return L"Restore";

        case WindowMenu: return L"WindowMenu";

        default:
            return L"Unspecified";
        }
    }
};
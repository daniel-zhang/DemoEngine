#include "Application.h"

Application::Application() : 
mExit(false),
mIsActivated(false)
{
    // Initialize the backing win32 app
    HINSTANCE hInstance = GetModuleHandle(NULL);

    Win32Application::initialize(this, hInstance, nullptr);

    mNativeApp = Win32Application::getInstance();

    // Initialize anything else
}

Application::~Application() {}

void Application::addWindow(Window* window, bool showImmediately)
{
    // Add/arrange incoming window to the root array
    mWindows.push_back(window);

    // Extract a native window definition from the passed in window
    Win32WindowDefinition win32Def;

    Vector2D pos  = window->getDesiredScreenPostion();
    Vector2D size = window->getDesiredScreenSize();

    win32Def.DesiredPosX            = pos.x;
    win32Def.DesiredPosY            = pos.y;
    win32Def.DesiredWidth           = size.x;
    win32Def.DesiredHeight          = size.y;

    win32Def.HasOSWindowBorder      = window->hasOSWindowBorder();
    win32Def.SupportsTransparency   = window->supportsTransparency();
    win32Def.AppearsInTaskbar       = window->appearsInTaskbar();
    win32Def.IsTopmostWindow        = window->isTopmostWindow();
    win32Def.AcceptsInput           = window->acceptsInput();
    win32Def.ActivateWhenFirstShown = window->activateWhenFirstShown();
    
    win32Def.SupportsMinimize       = window->hasMinimizeBox();
    win32Def.SupportsMaximize       = window->hasMaximizeBox();

    win32Def.IsRegularWindow        = window->isRegularWindow();
    win32Def.HasSizingFrame         = window->hasSizingFrame();
    win32Def.SizeWillChangeOften    = window->sizeWillChangeOften();
    win32Def.ExpectedMaxWidth       = window->getExpectedMaxWidth();
    win32Def.ExpectedMaxHeight      = window->getExpectedMaxHeight();

    win32Def.Title                  = window->getTitle();
    win32Def.Opacity                = window->getOpacity();
    win32Def.CornerRadius           = window->getCornerRadius();

    // Now create the backing native window(not a working one yet)
    Win32Window* nativeWindow = mNativeApp->createWin32Window();

    // Attach the created native window to incoming window
    window->setNativeWindow(nativeWindow);

    // Finally kick start the native window
    mNativeApp->addWin32Window(win32Def, nativeWindow, nullptr);

    if (showImmediately)
    {
        window->show();
    }
}

void Application::destroy()
{
    // Clean up anything

    // Terminate run loop on next tick
    mExit = true;
}

Window* Application::findWindow(const Win32Window* nativeWindow) const
{
    for (auto wnd : mWindows)
    {
        if (wnd->getNativeWindow() == nativeWindow)
        {
            return wnd;
        }
    }
    return nullptr;
}

void Application::onWindowActivationChanged(const Win32Window* wnd, const EWindowActivation activationType)
{
    Window* window = findWindow(wnd);
    if (window)
    {
        if (activationType != EWindowActivation::Deactivate)
        {
            window->bringToFront();
        }
    }
}

void Application::onApplicationActivationChanged(const bool isActive)
{
    mIsActivated = isActive;
}

void Application::onWindowClose(const Win32Window* wnd)
{
    Window* window = findWindow(wnd);

    // Give the window a chance to decide what to do
    window->tryClose();
}

void Application::closeWindow(Window* window)
{
    // Clean up window related resource(i.e. swapchain/backbuffer)
    // TODO: don't have any backbuffer yet

    // Now destroy the backing window. Note the backing win32 window is also removed from win32 app within this call
    window->close();

    // Remove this window from windows array
    for (auto i = mWindows.begin(); i != mWindows.end(); ++i)
    {
        if (*i == window)
        {
            mWindows.erase(i);
            break;
        }
    }

    // Call dtor
    delete window;

    // If we just removed the last window, close app
    if (mWindows.empty())
    {
        destroy();
    }
}

bool Application::shouldProcessUserInputMessages(const Win32Window* wnd) const
{
    return true;
}


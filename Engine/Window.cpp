#include "Window.h"
#include "Win32Window.h"
#include "Application.h"

Window* Window::buildBorderlessWindow()
{
    Window* window = new Window();

    window->mDesiredScreenPos = Vector2D(600, 100);
    window->mDesiredScreenSize= Vector2D(400, 300);

    window->mHasOSWindowBorder = false;
    window->mSupportsTransparency = true;
    window->mAppearsInTaskbar = true;
    window->mIsTopmostWindow = true;
    window->mAcceptsInput = true;
    window->mActivateWhenFirstShown = true;

    window->mSupportsMinimize = true;
    window->mSupportsMaximize = true;

    window->mIsRegularWindow = true;
    window->mHasSizingFrame = true;
    window->mSizeWillChangeOften = false;
    window->mExpectedMaxWidth = 800;
    window->mExpectedMaxHeight = 600;

    window->mTitle = L"Borderless Window";
    window->mOpacity = 1.f;
    window->mCornerRadius = 3;

    return window;
}

Window* Window::buildDefaultWindow()
{
    Window* window = new Window();

    window->mDesiredScreenPos = Vector2D(600, 100);
    window->mDesiredScreenSize= Vector2D(400, 300);

    window->mHasOSWindowBorder = true;
    window->mSupportsTransparency = true;
    window->mAppearsInTaskbar = true;
    window->mIsTopmostWindow = true;
    window->mAcceptsInput = true;
    window->mActivateWhenFirstShown = true;

    window->mSupportsMinimize = true;
    window->mSupportsMaximize = true;

    window->mIsRegularWindow = true;
    window->mHasSizingFrame = true;
    window->mSizeWillChangeOften = false;
    window->mExpectedMaxWidth = 800;
    window->mExpectedMaxHeight = 600;

    window->mTitle = L"Default Window";
    window->mOpacity = 1.f;
    window->mCornerRadius = 10;

    return window;

}

Window::Window() : mNativeWindow(nullptr)
{

}

Window::~Window()
{

}

void Window::bringToFront()
{
    if (mNativeWindow)
        mNativeWindow->bringToFront();
}

void Window::show()
{
    if (mNativeWindow)
        mNativeWindow->show();
}

void Window::close()
{
    if (mNativeWindow)
        mNativeWindow->destroy();
}

void Window::setOnCloseListener(TOnCloseListener listener)
{
    mOnCloseListener = listener;
}

void Window::tryClose()
{
    // OnCloseListener() returning true indicates we really want to close
    if ( mOnCloseListener && !mOnCloseListener() )
    {
    }
    else
    {
        Application::getInstance()->closeWindow(this);
    }
}



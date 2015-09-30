#pragma once
#include "Core/Util.h"
#include "Core/Math.h"
#include <functional>

class Win32Window;

/*

*/

class Window
{
public:
    // Builder patterns

    static Window* buildBorderlessWindow();
    static Window* buildDefaultWindow();

    Window();
    ~Window();

public:
    // Appearance property getters
    Vector2D getDesiredScreenPostion(){ return mDesiredScreenPos; }
    Vector2D getDesiredScreenSize(){ return mDesiredScreenSize; }

    bool hasOSWindowBorder(){ return mHasOSWindowBorder; }
    bool supportsTransparency(){ return mSupportsTransparency; }
    bool appearsInTaskbar(){ return mAppearsInTaskbar; }
    bool isTopmostWindow(){ return mIsTopmostWindow; }
    bool acceptsInput(){ return mAcceptsInput; }
    bool activateWhenFirstShown(){ return mActivateWhenFirstShown; }

    bool hasMinimizeBox(){ return mSupportsMinimize; }
    bool hasMaximizeBox(){ return mSupportsMaximize; }

    bool isRegularWindow(){ return mIsRegularWindow; }
    bool hasSizingFrame(){ return mHasSizingFrame; }
    bool sizeWillChangeOften(){ return mSizeWillChangeOften; }
    int32_t getExpectedMaxWidth(){ return mExpectedMaxWidth; }
    int32_t getExpectedMaxHeight(){ return mExpectedMaxHeight; }

    std::wstring getTitle(){ return mTitle; }
    float getOpacity(){ return mOpacity; }
    int32_t getCornerRadius(){ return mCornerRadius; }

public:
    // Wrappers for native window
    void bringToFront();

    void show();

    void close();

    // Local methods
    void setNativeWindow(Win32Window* nativeWindow) { mNativeWindow = nativeWindow; }

    Win32Window* getNativeWindow() const { return mNativeWindow; }

    typedef std::function<bool(void)> TOnCloseListener;
    void setOnCloseListener(TOnCloseListener listener);

    void tryClose();

private:
    // Appearance properties
    Vector2D mDesiredScreenPos;
    Vector2D mDesiredScreenSize;

    bool mHasOSWindowBorder;
    bool mSupportsTransparency;
    bool mAppearsInTaskbar;
    bool mIsTopmostWindow;
    bool mAcceptsInput;
    bool mActivateWhenFirstShown;

    bool mSupportsMinimize;
    bool mSupportsMaximize;

    bool mIsRegularWindow;
    bool mHasSizingFrame;
    bool mSizeWillChangeOften;
    int32_t mExpectedMaxWidth;
    int32_t mExpectedMaxHeight;

    std::wstring mTitle;
    float mOpacity;
    int32_t mCornerRadius;

private:
    Win32Window* mNativeWindow;
    TOnCloseListener mOnCloseListener;
};
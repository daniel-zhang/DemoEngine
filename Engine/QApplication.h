#pragma once

#include "Application/MessageHandler.h"
#include "Renderer/VisualElementGroups.h"

class IModalHandler
{
public:
    virtual void modalTick() = 0;
};

class QApplication : public MessageHandler
{
    // Make singleton
public:
    static QApplication* getInstance()
    {
        static QApplication instance;
        return &instance;
    }
    ~QApplication();

private:
    QApplication();
    QApplication(const QApplication&);
    QApplication& operator=(const QApplication&);

public:
    void initialize(class UITestInterface* uiTestInterface);

    bool aboutToExit();

    void addQWindow(class QWindow* inQWindow);

    bool removeQWindow(class QWindow* inQWindow);

    void tick(float dt);

    const VisualElementGroups& getDrawData();

    void setModalHandler(IModalHandler* imh){ mModalHandler = imh; }

    //
    // MessageHandler Interface
    //
public: 
    virtual bool enableUserInput(Window* wnd) const override;

    virtual void onKeyChar(const TCHAR character, const bool isRepeat) override;

    virtual void onKeyDown(const int32_t keyCode, const uint32_t characterCode, const bool isRepeat) override;

    virtual void onKeyUp(const int32_t keyCode, const uint32_t characterCode, const bool isRepeat) override;

    virtual void onMouseDown(Window* wnd, const EMouseButtons button) override;

    virtual void onMouseUp(const EMouseButtons button) override;

    virtual void onMouseDoubleClick(Window* wnd, const EMouseButtons button) override;

    virtual void onMouseWheel(const float delta) override;

    virtual void onMouseMove() override;

    virtual void onRawMouseMove(const int32_t x, const int32_t y) override;

    virtual void onCursorSet() override;

    virtual void onOSPaint(Window* wnd) override;

    virtual void onWindowMoved(Window* wnd, const int32_t x, const int32_t y) override;

    virtual void onWindowResized(Window* wnd, const int32_t width, const int32_t height, bool bWasMinimized = false) override;

    virtual void enterModalSizeMove(Window* wnd) override;

    virtual void onWindowResizing(Window* wnd) override;

    virtual void exitModalSizeMove(Window* wnd) override;

    virtual void onModalTimerEvent(Window* wnd) override;

    virtual void onWindowActivationChanged(Window* wnd, const EWindowActivation activationType) override;

    virtual void onApplicationActivationChanged(const bool isActive) override;

    virtual EWindowZone getWindowZoneForPoint(Window* wnd, const int32_t x, const int32_t y) override;

    virtual void onWindowClose(Window* wnd) override;

    virtual bool onWindowAction(Window* wnd, const EWindowAction inActionType) override;

protected:
    class QWindow* findQWindow(Window* nativeWindow) const;

    class QWindow* getActiveQWindow() const;

    uint32_t getQWindowIndex(class QWindow* qw) const;

    void queryCursorPosition();

    bool findWidgetsUnderCursor(std::vector<class QWidget*>& outList);
    bool _findWidgetsUnderCursor(class QWidget* widget, const Vector2D& cursorPos, std::vector<class QWidget*>& outList) const;

    void computeMinimalLayouts(class QWidget* node);

protected:
    class Application* mNativeAppInstance;

    // Handler for native timer event fired from a modal loop 
    IModalHandler* mModalHandler;

    // Top-level QWindows are arranged in back to front order, activated window is always at the end of the array
    // This hierarchy makes it easier to detect which window is under cursor when there are multiple candidates
    std::vector<class QWindow*> mQWindows;

    // Keep track of the chain of hovered widgets to detect when to fire state-dependent events like mouse enter/leave
    std::vector<class QWidget*> mHoveredPath;

    std::vector<class QWidget*> mClickedPath;

    std::vector<class QWidget*> mFocusedPath;

    VisualElementGroups mDrawData;

    Vector2D mCursorPos;

    bool mIsMouseButtonDown;

    EMouseButtons mPressedMouseButton;
};
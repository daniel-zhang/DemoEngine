#include "QApplication.h"
#include "QWindow.h"
#include "Application/Application.h"

#include "UITestInterface.h"

QApplication::QApplication() : mNativeAppInstance(nullptr), mModalHandler(nullptr)
{
}

QApplication::~QApplication()
{

}

void QApplication::initialize(UITestInterface* uiTestInterface)
{
    mNativeAppInstance = Application::getInstance();
    mNativeAppInstance->initialize(nullptr, this);

    //
    // UI Test
    //
    std::vector<QWindow*> rootQWindows;
    uiTestInterface->createUI(rootQWindows);
    for (uint32_t i = 0; i < rootQWindows.size(); ++i)
    {
        addQWindow(rootQWindows[i]);
        rootQWindows[i]->initialize();
    }
}

bool QApplication::aboutToExit()
{
    return mQWindows.empty();
}

void QApplication::addQWindow(class QWindow* inQWindow)
{
    mQWindows.push_back(inQWindow);
}

bool QApplication::removeQWindow(class QWindow* inQWindow)
{
    for (uint32_t i = 0; i < mQWindows.size(); ++i)
    {
        if (mQWindows[i] == inQWindow)
        {
            mQWindows.erase(mQWindows.begin() + i);

            // Clear hovered/focused paths if belonged to this window
            if (!mHoveredPath.empty() && mHoveredPath[0] == inQWindow)
            {
                mHoveredPath.clear();
            }

            if (!mFocusedPath.empty() && mFocusedPath[0] == inQWindow)
            {
                mFocusedPath.clear();
            }

            safe_delete(inQWindow);

            return true;
        }
    }
    for (auto iter = mQWindows.begin(); iter != mQWindows.end(); ++iter)
    {
        if (*iter == inQWindow)
        {
            mQWindows.erase(iter);
            if (inQWindow) 
                delete inQWindow;
            return true;
        }
    }

    return false;
}

void QApplication::tick(float dt)
{
    Application::getInstance()->pumpMsg();
}

const VisualElementGroups& QApplication::getDrawData()
{
    mDrawData.clearContent();

    for (uint32_t i = 0; i < mQWindows.size(); ++i)
    {
        // Pass 1, each QWidget gets a chance to compute its minimal size
        computeMinimalLayouts(mQWindows[i]);

        // Pass 2, each QWidget either draws itself or layouts its children based on their minimal size, allowed space and layout policy.
        QRectangle drawRect(Vector2D::zero(), mQWindows[i]->getSize(), 0);
        mQWindows[i]->drawRecursively(mDrawData.addUninitializedGroup(), drawRect);
    }

    return mDrawData;
}

bool QApplication::enableUserInput(Window* wnd) const
{
    return true;
}

void QApplication::onKeyChar(const TCHAR character, const bool isRepeat)
{

}

void QApplication::onKeyDown(const int32_t keyCode, const uint32_t characterCode, const bool isRepeat)
{

}

void QApplication::onKeyUp(const int32_t keyCode, const uint32_t characterCode, const bool isRepeat)
{

}

void QApplication::onMouseDown(Window* wnd, const EMouseButtons button)
{
    mIsMouseButtonDown = true;
    mPressedMouseButton = button;
    switch (button)
    {
    case EMouseButtons::Left:
        findWidgetsUnderCursor(mClickedPath);
        for (auto& w : mClickedPath)
        {
            if ( w->onMouseDown())
                break;
        }
        break;

    case EMouseButtons::Middle:
        break;

    case EMouseButtons::Right:
        break;

    case EMouseButtons::Thumb01:
        break;

    case EMouseButtons::Thumb02:
        break;

    default:
        break;
    }
}

void QApplication::onMouseUp(const EMouseButtons button)
{
    mIsMouseButtonDown = false;
    switch (button)
    {
    case EMouseButtons::Left:
    {
        findWidgetsUnderCursor(mHoveredPath);

        for (auto& w : mHoveredPath)
        {
            if (w->onMouseUp())
                break;
        }

        uint32_t nPathItems = min(mHoveredPath.size(), mClickedPath.size());
        for (uint32_t i = 0; i < nPathItems; ++i)
        {
            if (mHoveredPath[i] != mClickedPath[i])
                break;
            if (mClickedPath[i]->onClick())
                break;
        }
    }
        break;

    case EMouseButtons::Middle:
        break;

    case EMouseButtons::Right:
        break;

    case EMouseButtons::Thumb01:
        break;

    case EMouseButtons::Thumb02:
        break;

    default:
        break;
    }
}

void QApplication::onMouseDoubleClick(Window* wnd, const EMouseButtons button)
{

}

void QApplication::onMouseWheel(const float delta)
{

}

void QApplication::onMouseMove()
{
    // Figure out current hovered path
    std::vector<QWidget*> currentHoveredPath;
    if ( !findWidgetsUnderCursor(currentHoveredPath))
    {
        return;
    }

    // Fire mouse leave event by comparing current hovered and last hovered paths
    std::vector<QWidget*>& lastHoveredPath = mHoveredPath;
    for (uint32_t i = 0; i < lastHoveredPath.size(); ++i)
    {
        bool mouseExit = true;
        for (uint32_t j = 0; j < currentHoveredPath.size(); ++j)
        {
            if (lastHoveredPath[i] == currentHoveredPath[j])
                mouseExit = false;
        }
        if (mouseExit && lastHoveredPath[i]->onMouseLeave())
        {
            // Stop iterating down the path if this widget does not want to propagate MouseLeave event
            break;
        }
    }

    // Fire mouse enter event and hover event by comparing current hovered and last hovered paths
    bool propagateMouseEnter = true;
    bool propagateHover = true;
    for (uint32_t i = 0; i < currentHoveredPath.size(); ++i)
    {
        bool mouseEnter = true;
        for (uint32_t j = 0; j < lastHoveredPath.size(); ++j)
        {
            if (currentHoveredPath[i] == lastHoveredPath[j])
                mouseEnter = false;
        }

        if (mouseEnter && propagateMouseEnter)
            propagateMouseEnter = !currentHoveredPath[i]->onMouseEnter();

        if (!mouseEnter && propagateHover)
            propagateHover = !currentHoveredPath[i]->onHover();
    }

    lastHoveredPath = currentHoveredPath;
}

void QApplication::onRawMouseMove(const int32_t x, const int32_t y)
{

}

void QApplication::onCursorSet()
{

}

void QApplication::onWindowResized(Window* wnd, const int32_t width, const int32_t height, bool bWasMinimized /*= false*/)
{
    QWindow* qWindow = findQWindow(wnd);
    if (qWindow)
    {
        if (bWasMinimized)
        {
            qWindow->minimize();
        }
        else
        {
            qWindow->resize(width, height);
        }
    }
}

void QApplication::onOSPaint(Window* wnd)
{
}

void QApplication::onWindowResizing(Window* wnd)
{
    QWindow* qWindow = findQWindow(wnd);
    if (qWindow)
    {
        if (!wnd->isMinimized())
        {
            ::RECT rcClient;
            wnd->getClientRect(rcClient);
            qWindow->resize(rcClient.right - rcClient.left, rcClient.bottom - rcClient.top);
        }
    }
}

UINT_PTR IDT_TIMER1 = 1000;
void QApplication::enterModalSizeMove(Window* wnd)
{
    ::SetTimer(wnd->getHwnd(), IDT_TIMER1, 1000/60, nullptr);
}

void QApplication::exitModalSizeMove(Window* wnd)
{
    ::KillTimer(wnd->getHwnd(), IDT_TIMER1);
}

void QApplication::onModalTimerEvent(Window* wnd)
{
    if (mModalHandler)
        mModalHandler->modalTick();
}

void QApplication::onWindowMoved(Window* wnd, const int32_t x, const int32_t y)
{
    QWindow* qWindow = findQWindow(wnd);
    if (qWindow)
    {
        qWindow->setPosition(Vector2D( (float)x, (float)y) );
    }
}

void QApplication::onWindowActivationChanged(Window* wnd, const EWindowActivation activationType)
{
    switch (activationType)
    {
    case EWindowActivation::Activate:
    case EWindowActivation::ActivateByMouse:
    {
        QWindow* toBeActivated = findQWindow(wnd);
        // Skip activation messages emitted from ::CreateWindow(), at which point the QWindow is not fully initilized
        if (!toBeActivated)
        {
            break;
        }
        uint32_t index = getQWindowIndex(toBeActivated);

        QWindow* currentActivated = getActiveQWindow();
        // TODO, notify currentActivated that it is going to be deactivated
        // TODO, notify toBeActivated
        mQWindows.erase(mQWindows.begin() + index);
        mQWindows.push_back(toBeActivated);
    }
        break;

    case EWindowActivation::Deactivate:
    default:
        break;
    }

}

void QApplication::onApplicationActivationChanged(const bool isActive)
{

}

EWindowZone QApplication::getWindowZoneForPoint(Window* wnd, const int32_t x, const int32_t y)
{
    QWindow* qWindow = findQWindow(wnd);
    if (!qWindow)
        return EWindowZone::Unspecified;

    return qWindow->simulateOSHitTest(x, y);

    // A temporary workaround to move borderless window
    //return EWindowZone::TitleBar;
}

void QApplication::onWindowClose(Window* wnd)
{
    QWindow* qWindow = findQWindow(wnd);
    if (qWindow)
    {
        qWindow->close();
    }
}

bool QApplication::onWindowAction(Window* wnd, const EWindowAction inActionType)
{
    return true;
}

class QWindow* QApplication::findQWindow(Window* nativeWindow) const
{
    for (uint32_t i = 0; i < mQWindows.size(); ++i)
    {
        if (nativeWindow == mQWindows[i]->getNativeWindow())
        {
            return mQWindows[i];
        }
    }
    return nullptr;
}

QWindow* QApplication::getActiveQWindow() const
{
    return mQWindows.empty() ? nullptr : mQWindows[mQWindows.size() - 1];
}

uint32_t QApplication::getQWindowIndex(class QWindow* qw) const
{
    for (uint32_t i = 0; i < mQWindows.size(); ++i)
    {
        if (qw == mQWindows[i])
        {
            return i;
        }
    }
    return -1;
}

void QApplication::queryCursorPosition()
{
    ::POINT p;
    ::GetCursorPos(&p);

    mCursorPos.x = (float)p.x;
    mCursorPos.y = (float)p.y;
}

bool QApplication::findWidgetsUnderCursor(std::vector<QWidget*>& outList) 
{
    outList.clear();

    queryCursorPosition();

    // For multiple top level windows under cursor, find out the top most one
    QWindow* topMostWindowUnderCursor = nullptr;
    uint32_t indexTopMost = 0;
    for (uint32_t i = 0; i < mQWindows.size(); ++i)
    {
        if (mQWindows[i]->isPointInside(mCursorPos) && i >= indexTopMost)
        {
            indexTopMost = i;
            topMostWindowUnderCursor = mQWindows[i];
        }
    }
    // Mouse not in any window
    if (!topMostWindowUnderCursor)
    {
        return false;
    }

    // Find out current hovered path
    bool val = _findWidgetsUnderCursor(topMostWindowUnderCursor, mCursorPos, outList);
    return val;
}

bool QApplication::_findWidgetsUnderCursor(QWidget* widget, const Vector2D& cursorPos, std::vector<QWidget*>& outList) const
{
    if (widget->isPointInside(cursorPos))
    {
        outList.push_back(widget);

        // Transform cursor position into local coordinates
        Vector2D pos = cursorPos - widget->getPosition();

        const QWidget::TWidgetList& children = widget->getChildren();

        for (auto& child : children)
        {
            // We don't handle overlapped children here, just bail out on the first child under cursor
            if ( _findWidgetsUnderCursor(child, pos, outList) )
            {
                break;
            }
        }
        return true;
    }
    return false;
}

void QApplication::computeMinimalLayouts(class QWidget* node)
{
    if (!node)
        return;

    for (auto& child : node->getChildren())
        computeMinimalLayouts(child);

    node->computeMinimalSize();
}


#include "QWidget.h"

QWidget::QWidget() :
    mParent(nullptr),
    mLayer(0),
    mUIState(EUIState::Normal)
{

}

QWidget::~QWidget()
{
    for (uint32_t i = 0; i < mChildWidgets.size(); ++i)
    {
        if (mChildWidgets[i])
        {
            delete mChildWidgets[i];
            mChildWidgets[i] = nullptr;
        }
    }
}

QWidget& QWidget::add(QWidget* child)
{
    mChildWidgets.push_back(child);
    child->setParent(this);
    return *child;
}

void QWidget::remove(QWidget* child)
{
    for (auto iter = mChildWidgets.begin(); iter != mChildWidgets.end(); ++iter)
    {
        if (*iter == child)
        {
            mChildWidgets.erase(iter);
            delete child;
        }
    }
}

void QWidget::tick(float dt)
{

}

void QWidget::drawRecursively(VisualElementGroup& group, QRectangle& drawRect)
{
    // TODO
    for (uint32_t i = 0; i < mChildWidgets.size(); ++i)
    {
        mChildWidgets[i]->drawRecursively(group, drawRect);
    }
}

QWidget& QWidget::debugSetPosition(const Vector2D& pos)
{
    mPosition = pos;
    return *this;
}

QWidget& QWidget::debugSetSize(const Vector2D& size)
{
    mSize = size;
    return *this;
}

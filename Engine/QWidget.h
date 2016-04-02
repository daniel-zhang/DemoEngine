#pragma once

#include "Core/Util.h"
#include "Core/Math.h"
#include "Renderer/VisualElementGroups.h"

struct QSize
{
    QSize() :Width(0), Height(0){}
    QSize(int32_t w, int32_t h) :Width(w), Height(h){}

    void zero(){ Width = Height = 0; }
    int32_t Width;
    int32_t Height;
};

struct QRectangle
{
    QRectangle():Layer(0){}

    QRectangle(const Vector2D& inPos, const Vector2D& inSize, uint32_t layer = 0) :Position(inPos), Size(inSize), Layer(layer){}

    QRectangle(float x, float y, float width, float height, uint32_t layer = 0) : Position(x, y), Size(width, height), Layer(layer){}

    Vector4D toClipRect() const { return Vector4D(Position.x + ToRoot.x, Position.y + ToRoot.y, Size.x, Size.y); }

    Vector2D Position; // Position in local coordinate

    Vector2D Size;

    Vector2D ToRoot; // Position of local coordinate in the root coordinate

    uint32_t Layer;
};

struct QPadding
{
    QPadding() : Left(0.f), Right(0.f), Top(0.f), Bottom(0.f){}

    QPadding(float val) : Left(val), Right(val), Top(val), Bottom(val){}

    QPadding(float l, float r, float t, float b):Left(l), Right(r), Top(t), Bottom(b){}

    float getHorizental() const { return Left + Right; }

    float getVertical() const { return Top+ Bottom; }

    float Left, Right, Top, Bottom;
};
typedef QPadding QMargin;

enum ELayoutPropertyValue
{
    match_parent = 0,
    wrap_content,
};

class QWidget
{
public:
    QWidget();

    virtual ~QWidget();

    typedef std::vector<QWidget*> TWidgetList;

public:
    //template<typename T> T* cast(QWidget* w) const { return static_cast<T>(w); }

    virtual QWidget& add(QWidget* child);

    QWidget& debugSetPosition(const Vector2D& pos);

    QWidget& debugSetSize(const Vector2D& size);

    virtual void remove(QWidget* child);

    virtual void tick(float dt);

    //////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////
    void setParent(QWidget* parent) { mParent = parent; }

    QWidget* getParent() const { return mParent; }

    const TWidgetList& getChildren () const { return mChildWidgets; }

    // Layout
    ELayoutPropertyValue getWidthLayoutPolicy() const{ return mWidthLayoutProperty; }

    ELayoutPropertyValue getHeightLayoutPolicy() const { return mHeightLayoutProperty; }

    QWidget& setWidthLayout(ELayoutPropertyValue layoutVal) { mWidthLayoutProperty = layoutVal; return *this; }

    QWidget& setHeightLayout(ELayoutPropertyValue layoutVal) { mHeightLayoutProperty = layoutVal; return *this; }

    bool isPointInside(const Vector2D& inPoint) const { return !( inPoint.x < mPosition.x || inPoint.x >(mPosition.x + mSize.x) || inPoint.y < mPosition.y || inPoint.y >(mPosition.y + mSize.y)); }

    void setPosition(const Vector2D& pos) { mPosition = pos; }

    void setSize(const Vector2D& size) { mSize = size; }

    void setPadding(const QPadding& padding) { mPadding = padding; }

    void setMargin(const QMargin& margin) { mMargin = margin; }

    // Get position relative to this widget's parent. For top level widget(QWindow), this returns the screen position
    const Vector2D& getPosition() const { return mPosition; }

    const Vector2D& getSize() const { return mSize; }

    const Vector2D& getMinimalSize() const { return mMinimalSize; }

    Vector2D getMinimalMarginedSize() const { return Vector2D(mMinimalSize.x + mMargin.getHorizental(), mMinimalSize.y + mMargin.getVertical()); }

    const QMargin& getMargin() const { return mMargin; }

    const QPadding& getPadding() const { return mPadding; }

    Vector2D getPaddedPosition() const { return Vector2D(mPosition.x + mPadding.Left, mPosition.y + mPadding.Top); }

    Vector2D getPaddedSize() const { return Vector2D(mSize.x - mPadding.getHorizental(), mSize.y - mPadding.getVertical()); }

    Vector2D getMarginedPosition() const { return Vector2D(mPosition.x - mMargin.Left, mPosition.y - mMargin.Top); }

    Vector2D getMarginedSize() const { return Vector2D(mSize.x + mMargin.getHorizental(), mSize.y + mMargin.getVertical()); }

    virtual void computeMinimalSize(){}

    virtual void layoutChildren(const QRectangle& srcRect, std::vector<QRectangle>& outChildrenRects) {}
    
    virtual void drawRecursively(VisualElementGroup& group, QRectangle& drawRect);


    // Memo
    // return true to notify QApp that the event is processed and should not be propagated further 
    // return false to allow QApp to tunnel the event down
    virtual bool onClick() { return false; }

    virtual bool onMouseEnter(){ return false; }

    virtual bool onHover(){ return false; }

    virtual bool onMouseLeave(){ return false; }

    //virtual void onDrag(const Vector2D& delta){}

    virtual bool onMouseDown(){ return false; }

    virtual bool onMouseUp(){ return false; }


protected:
    enum EUIState
    {
        Normal = 0,
        Hovered,
        PressedDown
    };

protected:
    uint32_t mLayer;

    QWidget* mParent;
    TWidgetList mChildWidgets;

    Vector2D mPosition;
    Vector2D mSize;
    QPadding mPadding;
    QMargin mMargin;

    Vector2D mMinimalSize;
    ELayoutPropertyValue mWidthLayoutProperty;
    ELayoutPropertyValue mHeightLayoutProperty;


    QSize mActualSize;
    EUIState mUIState;
};
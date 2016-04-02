#pragma once
#include "Core/Util.h"
#include "Core/Math.h"
#include "Texture.h"

struct QuadAttributes
{
    uint32_t Placeholder;
};

struct TextAttributes
{
    // Exclusive attributes for text element
    // 1) Renderer does not handle layout tasks like mixed fonts, newline or soft wrap, which implies VisualElement::Text should neither 
    //    contain a newline, nor expect a render-performed soft wrap. These are supposed to be taken care of by GUI.
    // 2) One exception is kerning. Applying kerning is cleaner and more efficient at render time. Beside GUI is still able to build 
    //    complex layout structures by having a text element contain only one character(which effectively bypasses render time kerning).
    std::string FontName;
    uint32_t FontSize;
    std::wstring Text;
};

struct BorderAttributes
{
    Vector4D Padding; // left, top, right, bottom, in percentage
};

struct LineAttributes
{
    std::vector<Vector2D> Points;
    float Thickness;
    bool Prefiltered;
};

struct SplineAttributes
{
    Vector2D StartPt;
    Vector2D StartDir;
    Vector2D EndPt;
    Vector2D EndDir;
    float Thickness;
};

struct GradientStop
{
    GradientStop(const Vector2D& pos, const Color& color) : Position(pos), Color(color)
    {}
    Vector2D Position;
    Color Color;
};
struct GradientAttributes
{
    enum GradientOrientation
    {
        HorizentalGradient,
        VerticalGradient
    };
    std::vector<GradientStop> Stops;
    GradientOrientation Orientation;
};

// The fundamental description of a visible GUI element
 struct VisualElement 
 {
     VisualElement():
         ElementType(QuadElement),
         LayerId(0),
         ScalingFactor(1.f),
         Tex(nullptr),
         GammaCorrection(true),
         DisabledEffect(false)
    {
    }

    enum EElementType
    {
        QuadElement,
        TextElement,
        BorderElement,
        LineElement, // Anti-aliased line or normal line
        SplineElement,
        GradientElement,
    };
    
    // Common attributes
    EElementType ElementType;

    uint32_t LayerId;

    Vector2D PixelPosition;

    Vector2D PixelSize;

    float ScalingFactor; // Not implemented

    Color Color;

    Texture* Tex;

    Vector4D ClipRect;  

    //TODO
    bool GammaCorrection;
    bool DisabledEffect;

    // Element type specific attributes
    QuadAttributes QuadAttr;

    TextAttributes TextAttr;

    BorderAttributes BorderAttr;

    LineAttributes LineAttr;

    SplineAttributes SplineAttr;

    GradientAttributes GradientAttr;
};

// Elements within the same group are painted on the same render target, identified by hwnd
struct VisualElementGroup
 {
     HWND Hwnd;
     std::vector<VisualElement> Elements;
 };

// Gui system uses this structure to describe what it wants to draw, renderer then interprets this description into graphics
 class VisualElementGroups
 {
 public:
     VisualElementGroups(){}
     ~VisualElementGroups(){}

     VisualElementGroup& addUninitializedGroup()
     {
         mGroups.push_back(VisualElementGroup());
         return mGroups[mGroups.size() - 1];
     }

     const std::vector<VisualElementGroup>& getContent() const
     {
         return mGroups;
     }

     void clearContent()
     {
         mGroups.clear();
     }

 protected:
     std::vector<VisualElementGroup> mGroups;
 };
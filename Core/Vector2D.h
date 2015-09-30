#pragma once

#include <DirectXMath.h>
using namespace DirectX;

class Vector2D : public XMFLOAT2
{
public:
    Vector2D() : XMFLOAT2(0.f, 0.f){}
    Vector2D(float _x, float _y) : XMFLOAT2(_x, _y){}

    Vector2D operator+(const Vector2D& rhs) const;
    Vector2D operator*(float scale) const;  // Scalar multiplication
};

inline Vector2D Vector2D::operator+(const Vector2D& rhs) const
{
    XMVECTOR left = XMLoadFloat2(this);
    XMVECTOR right = XMLoadFloat2(&rhs);
    Vector2D result;
    XMStoreFloat2(&result, XMVectorAdd(left, right));
    return result;
}

inline Vector2D Vector2D::operator*(float scale) const
{
    XMVECTOR left = XMLoadFloat2(this);
    Vector2D result;
    XMStoreFloat2(&result, XMVectorScale(left, scale));
    return result;
}

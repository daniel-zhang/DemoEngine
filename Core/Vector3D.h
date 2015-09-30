#pragma once
#include <DirectXMath.h>
using namespace DirectX;

class Vector3D : public XMFLOAT3
{
public:
    static Vector3D zero() { return Vector3D(0.f, 0.f, 0.f); }
    static Vector3D unit() { return Vector3D(1.f, 1.f, 1.f); }
    static Vector3D unitX(){ return Vector3D(1.f, 0.f, 0.f); }
    static Vector3D unitY(){ return Vector3D(0.f, 1.f, 0.f); }
    static Vector3D unitZ(){ return Vector3D(0.f, 0.f, 1.f); }

public:
    Vector3D() : XMFLOAT3(0.f, 0.f, 0.f){}
    Vector3D(float _x, float _y, float _z) : XMFLOAT3(_x, _y, _z){}
    Vector3D(const Vector3D& rhs) : XMFLOAT3(rhs.x, rhs.y, rhs.z){}
    Vector3D(FXMVECTOR v){ XMStoreFloat3(this, v); }

    void fromXMVECTOR(FXMVECTOR v){ XMStoreFloat3(this, v); }
    XMVECTOR toXMVECTOR() const { return XMLoadFloat3(this); }

    Vector3D operator+(const Vector3D& rhs) const;
    Vector3D operator-(const Vector3D& rhs) const;
    Vector3D operator*(const Vector3D& rhs) const;  // Component-wise multiplication
    Vector3D operator*(float scale)         const;  // Scalar multiplication
    Vector3D operator/(const Vector3D& rhs) const;
    Vector3D operator/(float scale)         const;
    float    dot(const Vector3D& rhs)       const;
    Vector3D cross(const Vector3D& rhs)     const;
    void     cross(const Vector3D& rhs, Vector3D& result) const;

    Vector3D& operator=(const Vector3D& rhs);
    Vector3D& operator+=(const Vector3D& rhs);
    Vector3D& operator-=(const Vector3D& rhs);
    Vector3D& operator*=(const Vector3D& rhs);
    Vector3D& operator*=(float scale);
    Vector3D& operator/=(const Vector3D& rhs);
    Vector3D& operator/=(float scale);

    bool operator==(const Vector3D& rhs) const;
    bool operator!=(const Vector3D& rhs) const;

    // TODO: normalize, clamp, length, dist, interpolations, min/max, transform ...
    float length()          const;
    float squaredLength()   const;
    void normalize();
    void clamp(const Vector3D& vLower, const Vector3D& vUpper);

    // Static methods
    static float distance(const Vector3D& v0, const Vector3D& v1);
    static float squaredDistance(const Vector3D& v0, const Vector3D& v1);
    static Vector3D lerp(const Vector3D& v0, const Vector3D& v1, float t);
};

/******************************************************************************************************
Inline implementations: Vector3D
******************************************************************************************************/
inline Vector3D Vector3D::operator+(const Vector3D& rhs) const
{
    XMVECTOR left = XMLoadFloat3(this);
    XMVECTOR right = XMLoadFloat3(&rhs);
    Vector3D result;
    XMStoreFloat3(&result, XMVectorAdd(left, right));
    return result;
}

inline Vector3D Vector3D::operator-(const Vector3D& rhs) const
{
    XMVECTOR left = XMLoadFloat3(this);
    XMVECTOR right = XMLoadFloat3(&rhs);
    Vector3D result;
    XMStoreFloat3(&result, XMVectorSubtract(left, right));
    return result;
}

inline Vector3D Vector3D::operator*(const Vector3D& rhs) const
{
    XMVECTOR left = XMLoadFloat3(this);
    XMVECTOR right = XMLoadFloat3(&rhs);
    Vector3D result;
    XMStoreFloat3(&result, XMVectorMultiply(left, right));
    return result;
}

inline Vector3D Vector3D::operator*(float scale) const
{
    XMVECTOR left = XMLoadFloat3(this);
    Vector3D result;
    XMStoreFloat3(&result, XMVectorScale(left, scale));
    return result;
}

inline Vector3D Vector3D::operator/(const Vector3D& rhs) const
{
    XMVECTOR left = XMLoadFloat3(this);
    XMVECTOR right = XMLoadFloat3(&rhs);
    Vector3D result;
    XMStoreFloat3(&result, XMVectorDivide(left, right));
    return result;
}

inline Vector3D Vector3D::operator/(float scale) const
{
    XMVECTOR left = XMLoadFloat3(this);
    Vector3D result;
    XMStoreFloat3(&result, XMVectorScale(left, 1.f / scale));
    return result;
}

inline float Vector3D::dot(const Vector3D& rhs) const
{
    XMVECTOR left = XMLoadFloat3(this);
    XMVECTOR right = XMLoadFloat3(&rhs);
    XMVECTOR result = XMVector3Dot(left, right);
    return XMVectorGetX(result);
}

inline Vector3D Vector3D::cross(const Vector3D& rhs) const
{
    XMVECTOR left = XMLoadFloat3(this);
    XMVECTOR right = XMLoadFloat3(&rhs);
    Vector3D result;
    XMStoreFloat3(&result, XMVector3Cross(left, right));
    return result;
}

inline void Vector3D::cross(const Vector3D& rhs, Vector3D& result) const
{
    XMVECTOR left = XMLoadFloat3(this);
    XMVECTOR right = XMLoadFloat3(&rhs);
    XMStoreFloat3(&result, XMVector3Cross(left, right));
}

inline Vector3D& Vector3D::operator=(const Vector3D& rhs)
{
    x = rhs.x;
    y = rhs.y;
    z = rhs.z;
    return *this;
}

inline Vector3D& Vector3D::operator+=(const Vector3D& rhs)
{
    XMVECTOR left = XMLoadFloat3(this);
    XMVECTOR right = XMLoadFloat3(&rhs);
    XMStoreFloat3(this, XMVectorAdd(left, right));
    return *this;
}

inline Vector3D& Vector3D::operator-=(const Vector3D& rhs)
{
    XMVECTOR left = XMLoadFloat3(this);
    XMVECTOR right = XMLoadFloat3(&rhs);
    XMStoreFloat3(this, XMVectorSubtract(left, right));
    return *this;
}

inline Vector3D& Vector3D::operator*=(const Vector3D& rhs)
{
    XMVECTOR left = XMLoadFloat3(this);
    XMVECTOR right = XMLoadFloat3(&rhs);
    XMStoreFloat3(this, XMVectorMultiply(left, right));
    return *this;
}

inline Vector3D& Vector3D::operator*=(float scale)
{
    XMVECTOR left = XMLoadFloat3(this);
    XMStoreFloat3(this, XMVectorScale(left, scale));
    return *this;
}

inline Vector3D& Vector3D::operator/=(const Vector3D& rhs)
{
    XMVECTOR left = XMLoadFloat3(this);
    XMVECTOR right = XMLoadFloat3(&rhs);
    XMStoreFloat3(this, XMVectorDivide(left, right));
    return *this;
}

inline Vector3D& Vector3D::operator/=(float scale)
{
    XMVECTOR left = XMLoadFloat3(this);
    XMStoreFloat3(this, XMVectorScale(left, 1.f / scale));
    return *this;
}

inline bool Vector3D::operator==(const Vector3D& rhs) const
{
    XMVECTOR left = XMLoadFloat3(this);
    XMVECTOR right = XMLoadFloat3(&rhs);
    return XMVector3Equal(left, right) ? true : false;
}

inline bool Vector3D::operator!=(const Vector3D& rhs) const
{
    XMVECTOR left = XMLoadFloat3(this);
    XMVECTOR right = XMLoadFloat3(&rhs);
    return XMVector3NotEqual(left, right) ? true : false;
}

inline void Vector3D::normalize()
{
    XMStoreFloat3(this, XMVector3Normalize(XMLoadFloat3(this)));
}

inline float Vector3D::length() const
{
    return XMVectorGetX(XMVector3Length(XMLoadFloat3(this)));
}

inline float Vector3D::squaredLength() const
{
    return XMVectorGetX(XMVector3LengthSq(XMLoadFloat3(this)));
}

inline void Vector3D::clamp(const Vector3D& vLower, const Vector3D& vUpper)
{
    XMVECTOR vthis = XMLoadFloat3(this);
    XMVECTOR vmin = XMLoadFloat3(&vLower);
    XMVECTOR vmax = XMLoadFloat3(&vUpper);

    XMStoreFloat3(this, XMVectorClamp(vthis, vmin, vmax));
}

inline float Vector3D::distance(const Vector3D& v0, const Vector3D& v1)
{
    XMVECTOR x0 = XMLoadFloat3(&v0);
    XMVECTOR x1 = XMLoadFloat3(&v1);
    XMVECTOR V = XMVectorSubtract(x1, x0);
    XMVECTOR X = XMVector3Length(V);
    return XMVectorGetX(X);
}

inline float Vector3D::squaredDistance(const Vector3D& v0, const Vector3D& v1)
{
    XMVECTOR x0 = XMLoadFloat3(&v0);
    XMVECTOR x1 = XMLoadFloat3(&v1);
    XMVECTOR V = XMVectorSubtract(x1, x0);
    XMVECTOR X = XMVector3LengthSq(V);
    return XMVectorGetX(X);
}

inline Vector3D Vector3D::lerp(const Vector3D& v0, const Vector3D& v1, float t)
{
    XMVECTOR x0 = XMLoadFloat3(&v0);
    XMVECTOR x1 = XMLoadFloat3(&v1);
    XMVECTOR X = XMVectorLerp(x0, x1, t);

    Vector3D result;
    XMStoreFloat3(&result, X);
    return result;
}


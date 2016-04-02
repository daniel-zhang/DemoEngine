#pragma once

#include <DirectXMath.h>

class Vector4D : public DirectX::XMFLOAT4
{
public:
    static Vector4D zero() { return Vector4D(0.f, 0.f, 0.f, 0.f); }
    static Vector4D unit() { return Vector4D(1.f, 1.f, 1.f, 1.f); }
    static Vector4D unitX(){ return Vector4D(1.f, 0.f, 0.f, 0.f); }
    static Vector4D unitY(){ return Vector4D(0.f, 1.f, 0.f, 0.f); }
    static Vector4D unitZ(){ return Vector4D(0.f, 0.f, 1.f, 0.f); }
    static Vector4D unitW(){ return Vector4D(0.f, 0.f, 0.f, 1.f); }

public:
    Vector4D() : DirectX::XMFLOAT4(0.f, 0.f, 0.f, 0.f)
    {}

    Vector4D(float _x, float _y, float _z, float _w) : DirectX::XMFLOAT4(_x, _y, _z, _w)
    {}

    Vector4D(DirectX::FXMVECTOR v)
    {
        DirectX::XMStoreFloat4(this, v);
    }

    void fromXMVECTOR(DirectX::FXMVECTOR v)
    {
        DirectX::XMStoreFloat4(this, v);
    }

    DirectX::XMVECTOR toXMVECTOR() const 
    { 
        return DirectX::XMLoadFloat4(this); 
    }

    // Const operations
    Vector4D operator+(const Vector4D& rhs) const;
    Vector4D operator-(const Vector4D& rhs) const;
    Vector4D operator*(const Vector4D& rhs) const;  // Component multiply
    Vector4D operator*(float value)         const;  // Scalar multiply
    Vector4D operator/(const Vector4D& rhs) const;  // Component divide
    Vector4D operator/(float value)         const;  // Scalar divide
    bool operator==(const Vector4D& rhs) const;
    bool operator!=(const Vector4D& rhs) const;
    float dot(const Vector4D& rhs)       const;

    // Non-const operations
    Vector4D& operator=(const Vector4D& rhs);
    Vector4D& operator+=(const Vector4D& rhs);
    Vector4D& operator-=(const Vector4D& rhs);
    Vector4D& operator*=(const Vector4D& rhs);
    Vector4D& operator*=(float value);
    Vector4D& operator/=(const Vector4D& rhs);
    Vector4D& operator/=(float value);

    // TODO: normalize, clamp, length, dist, interpolations, min/max, transform ...
    float length() const;
    float squaredLength() const;
    Vector4D& normalize();

};

inline Vector4D Vector4D::operator+(const Vector4D& rhs) const
{
    DirectX::XMVECTOR left = DirectX::XMLoadFloat4(this);
    DirectX::XMVECTOR right = DirectX::XMLoadFloat4(&rhs);
    Vector4D result;
    DirectX::XMStoreFloat4(&result, DirectX::XMVectorAdd(left, right));
    return result;
}

inline Vector4D Vector4D::operator-(const Vector4D& rhs) const
{
    DirectX::XMVECTOR left = DirectX::XMLoadFloat4(this);
    DirectX::XMVECTOR right = DirectX::XMLoadFloat4(&rhs);
    Vector4D result;
    DirectX::XMStoreFloat4(&result, DirectX::XMVectorSubtract(left, right));
    return result;
}

inline Vector4D Vector4D::operator*(const Vector4D& rhs) const
{
    DirectX::XMVECTOR left = DirectX::XMLoadFloat4(this);
    DirectX::XMVECTOR right = DirectX::XMLoadFloat4(&rhs);
    Vector4D result;
    DirectX::XMStoreFloat4(&result, DirectX::XMVectorMultiply(left, right));
    return result;
}

inline Vector4D Vector4D::operator*(float value) const
{
    DirectX::XMVECTOR left = DirectX::XMLoadFloat4(this);
    Vector4D result;
    DirectX::XMStoreFloat4(&result, DirectX::XMVectorScale(left, value));
    return result;
}

inline Vector4D Vector4D::operator/(const Vector4D& rhs) const
{
    DirectX::XMVECTOR left = DirectX::XMLoadFloat4(this);
    DirectX::XMVECTOR right = DirectX::XMLoadFloat4(&rhs);
    Vector4D result;
    DirectX::XMStoreFloat4(&result, DirectX::XMVectorDivide(left, right));
    return result;
}

inline Vector4D Vector4D::operator/(float value) const
{
    DirectX::XMVECTOR left = DirectX::XMLoadFloat4(this);
    Vector4D result;
    DirectX::XMStoreFloat4(&result, DirectX::XMVectorScale(left, 1.f / value));
    return result;
}

inline bool Vector4D::operator==(const Vector4D& rhs) const
{
    DirectX::XMVECTOR left = DirectX::XMLoadFloat4(this);
    DirectX::XMVECTOR right = DirectX::XMLoadFloat4(&rhs);
    return DirectX::XMVector4Equal(left, right) ? true : false;
}

inline bool Vector4D::operator!=(const Vector4D& rhs) const
{
    DirectX::XMVECTOR left = DirectX::XMLoadFloat4(this);
    DirectX::XMVECTOR right = DirectX::XMLoadFloat4(&rhs);
    return DirectX::XMVector4NotEqual(left, right) ? true : false;
}

inline float Vector4D::dot(const Vector4D& rhs) const
{
    DirectX::XMVECTOR left = DirectX::XMLoadFloat4(this);
    DirectX::XMVECTOR right = DirectX::XMLoadFloat4(&rhs);
    DirectX::XMVECTOR result = DirectX::XMVector4Dot(left, right);
    return DirectX::XMVectorGetX(result);
}

inline Vector4D& Vector4D::operator=(const Vector4D& rhs)
{
    x = rhs.x;
    y = rhs.y;
    z = rhs.z;
    w = rhs.w;
    return *this;
}

inline Vector4D& Vector4D::operator+=(const Vector4D& rhs)
{
    DirectX::XMVECTOR left = DirectX::XMLoadFloat4(this);
    DirectX::XMVECTOR right = DirectX::XMLoadFloat4(&rhs);
    DirectX::XMStoreFloat4(this, DirectX::XMVectorAdd(left, right));
    return *this;
}

inline Vector4D& Vector4D::operator-=(const Vector4D& rhs)
{
    DirectX::XMVECTOR left = DirectX::XMLoadFloat4(this);
    DirectX::XMVECTOR right = DirectX::XMLoadFloat4(&rhs);
    DirectX::XMStoreFloat4(this, DirectX::XMVectorSubtract(left, right));
    return *this;
}

inline Vector4D& Vector4D::operator*=(const Vector4D& rhs)
{
    DirectX::XMVECTOR left = DirectX::XMLoadFloat4(this);
    DirectX::XMVECTOR right = DirectX::XMLoadFloat4(&rhs);
    DirectX::XMStoreFloat4(this, DirectX::XMVectorMultiply(left, right));
    return *this;
}

inline Vector4D& Vector4D::operator*=(float value)
{
    DirectX::XMVECTOR left = DirectX::XMLoadFloat4(this);
    DirectX::XMStoreFloat4(this, DirectX::XMVectorScale(left, value));
    return *this;
}

inline Vector4D& Vector4D::operator/=(const Vector4D& rhs)
{
    DirectX::XMVECTOR left = DirectX::XMLoadFloat4(this);
    DirectX::XMVECTOR right = DirectX::XMLoadFloat4(&rhs);
    DirectX::XMStoreFloat4(this, DirectX::XMVectorDivide(left, right));
    return *this;
}

inline Vector4D& Vector4D::operator/=(float value)
{
    DirectX::XMVECTOR left = DirectX::XMLoadFloat4(this);
    DirectX::XMStoreFloat4(this, DirectX::XMVectorScale(left, 1.f / value));
    return *this;
}

inline Vector4D& Vector4D::normalize()
{
    DirectX::XMStoreFloat4(this, DirectX::XMVector4Normalize(DirectX::XMLoadFloat4(this)));
    return *this;
}

inline float Vector4D::length() const
{
    return DirectX::XMVectorGetX(DirectX::XMVector4Length(DirectX::XMLoadFloat4(this)));
}

inline float Vector4D::squaredLength() const
{
    return DirectX::XMVectorGetX(DirectX::XMVector4LengthSq(DirectX::XMLoadFloat4(this)));
}

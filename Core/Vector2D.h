#pragma once

#include <DirectXMath.h>

class Vector2D : public DirectX::XMFLOAT2
{
public:
    static Vector2D zero() { return Vector2D(0.f, 0.f); }

    static Vector2D unit() { return Vector2D(1.f, 1.f); }

    static Vector2D unitX(){ return Vector2D(1.f, 0.f); }

    static Vector2D unitY(){ return Vector2D(0.f, 1.f); }

public:
    Vector2D() : DirectX::XMFLOAT2(0.f, 0.f)
    {}

    Vector2D(float _x, float _y) : DirectX::XMFLOAT2(_x, _y)
    {}

    Vector2D(const Vector2D& rhs) : DirectX::XMFLOAT2(rhs.x, rhs.y)
    {}

    Vector2D(DirectX::FXMVECTOR v)
    {
        DirectX::XMStoreFloat2(this, v);
    }

    void fromXMVECTOR(DirectX::FXMVECTOR v)
    {
        DirectX::XMStoreFloat2(this, v);
    }

    DirectX::XMVECTOR toXMVECTOR() const
    {
        return DirectX::XMLoadFloat2(this);
    }

    // Const operations
    Vector2D operator+(const Vector2D& rhs) const;
    Vector2D operator-(const Vector2D& rhs) const;
    Vector2D operator*(const Vector2D& rhs) const;  // Component multiply
    Vector2D operator*(float value) const;          // Scalar multiply
    Vector2D operator/(const Vector2D& rhs) const;  // Component divide 
    Vector2D operator/(float value) const;          // Scalar divide
    bool operator==(const Vector2D& rhs) const;
    bool operator!=(const Vector2D& rhs) const;

    float dot(const Vector2D& rhs) const;
    Vector2D cross(const Vector2D& rhs) const;
    void cross(const Vector2D& rhs, Vector2D& result) const;

    // Non-const operations
    Vector2D& operator=(const Vector2D& rhs);
    Vector2D& operator+=(const Vector2D& rhs);
    Vector2D& operator-=(const Vector2D& rhs);
    Vector2D& operator*=(const Vector2D& rhs);
    Vector2D& operator*=(float value);
    Vector2D& operator/=(const Vector2D& rhs);
    Vector2D& operator/=(float value);

    float length() const;
    float squaredLength() const;
    Vector2D& normalize();
};

inline Vector2D Vector2D::operator+(const Vector2D& rhs) const
{
    DirectX::XMVECTOR left = DirectX::XMLoadFloat2(this);
    DirectX::XMVECTOR right = DirectX::XMLoadFloat2(&rhs);
    Vector2D result;
    DirectX::XMStoreFloat2(&result, DirectX::XMVectorAdd(left, right));
    return result;
}

inline Vector2D Vector2D::operator-(const Vector2D& rhs) const
{
    DirectX::XMVECTOR left = DirectX::XMLoadFloat2(this);
    DirectX::XMVECTOR right = DirectX::XMLoadFloat2(&rhs);
    Vector2D result;
    DirectX::XMStoreFloat2(&result, DirectX::XMVectorSubtract(left, right));
    return result;
}

inline Vector2D Vector2D::operator*(const Vector2D& rhs) const
{
    DirectX::XMVECTOR left = DirectX::XMLoadFloat2(this);
    DirectX::XMVECTOR right = DirectX::XMLoadFloat2(&rhs);
    Vector2D result;
    DirectX::XMStoreFloat2(&result, DirectX::XMVectorMultiply(left, right));
    return result;
}

inline Vector2D Vector2D::operator*(float value) const
{
    DirectX::XMVECTOR left = DirectX::XMLoadFloat2(this);
    Vector2D result;
    DirectX::XMStoreFloat2(&result, DirectX::XMVectorScale(left, value));
    return result;
}

inline Vector2D Vector2D::operator/(const Vector2D& rhs) const
{
    DirectX::XMVECTOR left = DirectX::XMLoadFloat2(this);
    DirectX::XMVECTOR right = DirectX::XMLoadFloat2(&rhs);
    Vector2D result;
    DirectX::XMStoreFloat2(&result, DirectX::XMVectorDivide(left, right));
    return result;
}

inline Vector2D Vector2D::operator/(float value) const
{
    DirectX::XMVECTOR left = DirectX::XMLoadFloat2(this);
    Vector2D result;
    DirectX::XMStoreFloat2(&result, DirectX::XMVectorScale(left, 1.f / value));
    return result;
}

inline bool Vector2D::operator==(const Vector2D& rhs) const
{
    DirectX::XMVECTOR left = DirectX::XMLoadFloat2(this);
    DirectX::XMVECTOR right = DirectX::XMLoadFloat2(&rhs);
    return DirectX::XMVector2Equal(left, right) ? true : false;
}

inline bool Vector2D::operator!=(const Vector2D& rhs) const
{
    DirectX::XMVECTOR left = DirectX::XMLoadFloat2(this);
    DirectX::XMVECTOR right = DirectX::XMLoadFloat2(&rhs);
    return DirectX::XMVector2NotEqual(left, right) ? true : false;
}

inline float Vector2D::dot(const Vector2D& rhs) const
{
    DirectX::XMVECTOR left = DirectX::XMLoadFloat2(this);
    DirectX::XMVECTOR right = DirectX::XMLoadFloat2(&rhs);
    DirectX::XMVECTOR result = DirectX::XMVector2Dot(left, right);
    return DirectX::XMVectorGetX(result);
}

inline Vector2D Vector2D::cross(const Vector2D& rhs) const
{
    DirectX::XMVECTOR left = DirectX::XMLoadFloat2(this);
    DirectX::XMVECTOR right = DirectX::XMLoadFloat2(&rhs);
    Vector2D result;
    DirectX::XMStoreFloat2(&result, DirectX::XMVector2Cross(left, right));
    return result;
}

inline void Vector2D::cross(const Vector2D& rhs, Vector2D& result) const
{
    DirectX::XMVECTOR left = DirectX::XMLoadFloat2(this);
    DirectX::XMVECTOR right = DirectX::XMLoadFloat2(&rhs);
    DirectX::XMStoreFloat2(&result, DirectX::XMVector2Cross(left, right));
}

inline Vector2D& Vector2D::operator=(const Vector2D& rhs)
{
    x = rhs.x;
    y = rhs.y;
    return *this;
}

inline Vector2D& Vector2D::operator+=(const Vector2D& rhs)
{
    DirectX::XMVECTOR left = DirectX::XMLoadFloat2(this);
    DirectX::XMVECTOR right = DirectX::XMLoadFloat2(&rhs);
    DirectX::XMStoreFloat2(this, DirectX::XMVectorAdd(left, right));
    return *this;
}

inline Vector2D& Vector2D::operator-=(const Vector2D& rhs)
{
    DirectX::XMVECTOR left = DirectX::XMLoadFloat2(this);
    DirectX::XMVECTOR right = DirectX::XMLoadFloat2(&rhs);
    DirectX::XMStoreFloat2(this, DirectX::XMVectorSubtract(left, right));
    return *this;
}

inline Vector2D& Vector2D::operator*=(const Vector2D& rhs)
{
    DirectX::XMVECTOR left = DirectX::XMLoadFloat2(this);
    DirectX::XMVECTOR right = DirectX::XMLoadFloat2(&rhs);
    DirectX::XMStoreFloat2(this, DirectX::XMVectorMultiply(left, right));
    return *this;
}

inline Vector2D& Vector2D::operator*=(float value)
{
    DirectX::XMVECTOR left = DirectX::XMLoadFloat2(this);
    DirectX::XMStoreFloat2(this, DirectX::XMVectorScale(left, value));
    return *this;
}

inline Vector2D& Vector2D::operator/=(const Vector2D& rhs)
{
    DirectX::XMVECTOR left = DirectX::XMLoadFloat2(this);
    DirectX::XMVECTOR right = DirectX::XMLoadFloat2(&rhs);
    DirectX::XMStoreFloat2(this, DirectX::XMVectorDivide(left, right));
    return *this;
}

inline Vector2D& Vector2D::operator/=(float value)
{
    DirectX::XMVECTOR left = DirectX::XMLoadFloat2(this);
    DirectX::XMStoreFloat2(this, DirectX::XMVectorScale(left, 1.f / value));
    return *this;
}

inline float Vector2D::length() const
{
    return DirectX::XMVectorGetX(DirectX::XMVector2Length(DirectX::XMLoadFloat2(this)));
}

inline float Vector2D::squaredLength() const
{
    return DirectX::XMVectorGetX(DirectX::XMVector2LengthSq(DirectX::XMLoadFloat2(this)));
}

inline Vector2D& Vector2D::normalize()
{
    DirectX::XMStoreFloat2(this, DirectX::XMVector2Normalize(DirectX::XMLoadFloat2(this)));
    return *this;
}


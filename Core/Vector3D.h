#pragma once
#include <DirectXMath.h>

class Vector3D : public DirectX::XMFLOAT3
{
public:
    static Vector3D zero() { return Vector3D(0.f, 0.f, 0.f); }
    static Vector3D unit() { return Vector3D(1.f, 1.f, 1.f); }
    static Vector3D unitX(){ return Vector3D(1.f, 0.f, 0.f); }
    static Vector3D unitY(){ return Vector3D(0.f, 1.f, 0.f); }
    static Vector3D unitZ(){ return Vector3D(0.f, 0.f, 1.f); }

public:
    Vector3D() : DirectX::XMFLOAT3(0.f, 0.f, 0.f)
    {}

    Vector3D(float _x, float _y, float _z) : DirectX::XMFLOAT3(_x, _y, _z)
    {}

    Vector3D(const Vector3D& rhs) : DirectX::XMFLOAT3(rhs.x, rhs.y, rhs.z)
    {}

    Vector3D(DirectX::FXMVECTOR v)
    { 
        DirectX::XMStoreFloat3(this, v); 
    }

    void fromXMVECTOR(DirectX::FXMVECTOR v)
    { 
        DirectX::XMStoreFloat3(this, v); 
    }

    DirectX::XMVECTOR toXMVECTOR() const 
    { 
        return DirectX::XMLoadFloat3(this); 
    }

    // Const operations
    Vector3D operator+(const Vector3D& rhs) const;
    Vector3D operator-(const Vector3D& rhs) const;
    Vector3D operator*(const Vector3D& rhs) const;  // Component multiply
    Vector3D operator*(float value)         const;  // Scalar multiply
    Vector3D operator/(const Vector3D& rhs) const;  // Component divide
    Vector3D operator/(float value)         const;  // Scalar divide
    bool operator==(const Vector3D& rhs) const;
    bool operator!=(const Vector3D& rhs) const;
    float    dot(const Vector3D& rhs)       const;
    Vector3D cross(const Vector3D& rhs)     const;
    void     cross(const Vector3D& rhs, Vector3D& result) const;

    // Non-const operations
    Vector3D& operator=(const Vector3D& rhs);
    Vector3D& operator+=(const Vector3D& rhs);
    Vector3D& operator-=(const Vector3D& rhs);
    Vector3D& operator*=(const Vector3D& rhs);
    Vector3D& operator*=(float value);
    Vector3D& operator/=(const Vector3D& rhs);
    Vector3D& operator/=(float value);

    // TODO: normalize, clamp, length, dist, interpolations, min/max, transform ...
    float length() const;
    float squaredLength() const;
    Vector3D& normalize();
    Vector3D& clamp(const Vector3D& vLower, const Vector3D& vUpper);

    // Static methods
    static float distance(const Vector3D& v0, const Vector3D& v1);
    static float squaredDistance(const Vector3D& v0, const Vector3D& v1);
    static Vector3D lerp(const Vector3D& v0, const Vector3D& v1, float t);
};

inline Vector3D Vector3D::operator+(const Vector3D& rhs) const
{
    DirectX::XMVECTOR left = DirectX::XMLoadFloat3(this);
    DirectX::XMVECTOR right = DirectX::XMLoadFloat3(&rhs);
    Vector3D result;
    DirectX::XMStoreFloat3(&result, DirectX::XMVectorAdd(left, right));
    return result;
}

inline Vector3D Vector3D::operator-(const Vector3D& rhs) const
{
    DirectX::XMVECTOR left = DirectX::XMLoadFloat3(this);
    DirectX::XMVECTOR right = DirectX::XMLoadFloat3(&rhs);
    Vector3D result;
    DirectX::XMStoreFloat3(&result, DirectX::XMVectorSubtract(left, right));
    return result;
}

inline Vector3D Vector3D::operator*(const Vector3D& rhs) const
{
    DirectX::XMVECTOR left = DirectX::XMLoadFloat3(this);
    DirectX::XMVECTOR right = DirectX::XMLoadFloat3(&rhs);
    Vector3D result;
    DirectX::XMStoreFloat3(&result, DirectX::XMVectorMultiply(left, right));
    return result;
}

inline Vector3D Vector3D::operator*(float value) const
{
    DirectX::XMVECTOR left = DirectX::XMLoadFloat3(this);
    Vector3D result;
    DirectX::XMStoreFloat3(&result, DirectX::XMVectorScale(left, value));
    return result;
}

inline Vector3D Vector3D::operator/(const Vector3D& rhs) const
{
    DirectX::XMVECTOR left = DirectX::XMLoadFloat3(this);
    DirectX::XMVECTOR right = DirectX::XMLoadFloat3(&rhs);
    Vector3D result;
    DirectX::XMStoreFloat3(&result, DirectX::XMVectorDivide(left, right));
    return result;
}

inline Vector3D Vector3D::operator/(float value) const
{
    DirectX::XMVECTOR left = DirectX::XMLoadFloat3(this);
    Vector3D result;
    DirectX::XMStoreFloat3(&result, DirectX::XMVectorScale(left, 1.f / value));
    return result;
}

inline bool Vector3D::operator==(const Vector3D& rhs) const
{
    DirectX::XMVECTOR left = DirectX::XMLoadFloat3(this);
    DirectX::XMVECTOR right = DirectX::XMLoadFloat3(&rhs);
    return DirectX::XMVector3Equal(left, right) ? true : false;
}

inline bool Vector3D::operator!=(const Vector3D& rhs) const
{
    DirectX::XMVECTOR left = DirectX::XMLoadFloat3(this);
    DirectX::XMVECTOR right = DirectX::XMLoadFloat3(&rhs);
    return DirectX::XMVector3NotEqual(left, right) ? true : false;
}

inline float Vector3D::dot(const Vector3D& rhs) const
{
    DirectX::XMVECTOR left = DirectX::XMLoadFloat3(this);
    DirectX::XMVECTOR right = DirectX::XMLoadFloat3(&rhs);
    DirectX::XMVECTOR result = DirectX::XMVector3Dot(left, right);
    return DirectX::XMVectorGetX(result);
}

inline Vector3D Vector3D::cross(const Vector3D& rhs) const
{
    DirectX::XMVECTOR left = DirectX::XMLoadFloat3(this);
    DirectX::XMVECTOR right = DirectX::XMLoadFloat3(&rhs);
    Vector3D result;
    DirectX::XMStoreFloat3(&result, DirectX::XMVector3Cross(left, right));
    return result;
}

inline void Vector3D::cross(const Vector3D& rhs, Vector3D& result) const
{
    DirectX::XMVECTOR left = DirectX::XMLoadFloat3(this);
    DirectX::XMVECTOR right = DirectX::XMLoadFloat3(&rhs);
    DirectX::XMStoreFloat3(&result, DirectX::XMVector3Cross(left, right));
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
    DirectX::XMVECTOR left = DirectX::XMLoadFloat3(this);
    DirectX::XMVECTOR right = DirectX::XMLoadFloat3(&rhs);
    DirectX::XMStoreFloat3(this, DirectX::XMVectorAdd(left, right));
    return *this;
}

inline Vector3D& Vector3D::operator-=(const Vector3D& rhs)
{
    DirectX::XMVECTOR left = DirectX::XMLoadFloat3(this);
    DirectX::XMVECTOR right = DirectX::XMLoadFloat3(&rhs);
    DirectX::XMStoreFloat3(this, DirectX::XMVectorSubtract(left, right));
    return *this;
}

inline Vector3D& Vector3D::operator*=(const Vector3D& rhs)
{
    DirectX::XMVECTOR left = DirectX::XMLoadFloat3(this);
    DirectX::XMVECTOR right = DirectX::XMLoadFloat3(&rhs);
    DirectX::XMStoreFloat3(this, DirectX::XMVectorMultiply(left, right));
    return *this;
}

inline Vector3D& Vector3D::operator*=(float value)
{
    DirectX::XMVECTOR left = DirectX::XMLoadFloat3(this);
    DirectX::XMStoreFloat3(this, DirectX::XMVectorScale(left, value));
    return *this;
}

inline Vector3D& Vector3D::operator/=(const Vector3D& rhs)
{
    DirectX::XMVECTOR left = DirectX::XMLoadFloat3(this);
    DirectX::XMVECTOR right = DirectX::XMLoadFloat3(&rhs);
    DirectX::XMStoreFloat3(this, DirectX::XMVectorDivide(left, right));
    return *this;
}

inline Vector3D& Vector3D::operator/=(float value)
{
    DirectX::XMVECTOR left = DirectX::XMLoadFloat3(this);
    DirectX::XMStoreFloat3(this, DirectX::XMVectorScale(left, 1.f / value));
    return *this;
}

inline Vector3D& Vector3D::normalize()
{
    DirectX::XMStoreFloat3(this, DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(this)));
    return *this;
}

inline float Vector3D::length() const
{
    return DirectX::XMVectorGetX(DirectX::XMVector3Length(DirectX::XMLoadFloat3(this)));
}

inline float Vector3D::squaredLength() const
{
    return DirectX::XMVectorGetX(DirectX::XMVector3LengthSq(DirectX::XMLoadFloat3(this)));
}

inline Vector3D& Vector3D::clamp(const Vector3D& vLower, const Vector3D& vUpper)
{
    DirectX::XMVECTOR vthis = DirectX::XMLoadFloat3(this);
    DirectX::XMVECTOR vmin = DirectX::XMLoadFloat3(&vLower);
    DirectX::XMVECTOR vmax = DirectX::XMLoadFloat3(&vUpper);

    DirectX::XMStoreFloat3(this, DirectX::XMVectorClamp(vthis, vmin, vmax));
    return *this;
}

inline float Vector3D::distance(const Vector3D& v0, const Vector3D& v1)
{
    DirectX::XMVECTOR x0 = DirectX::XMLoadFloat3(&v0);
    DirectX::XMVECTOR x1 = DirectX::XMLoadFloat3(&v1);
    DirectX::XMVECTOR V = DirectX::XMVectorSubtract(x1, x0);
    DirectX::XMVECTOR X = DirectX::XMVector3Length(V);
    return DirectX::XMVectorGetX(X);
}

inline float Vector3D::squaredDistance(const Vector3D& v0, const Vector3D& v1)
{
    DirectX::XMVECTOR x0 = DirectX::XMLoadFloat3(&v0);
    DirectX::XMVECTOR x1 = DirectX::XMLoadFloat3(&v1);
    DirectX::XMVECTOR V = DirectX::XMVectorSubtract(x1, x0);
    DirectX::XMVECTOR X = DirectX::XMVector3LengthSq(V);
    return DirectX::XMVectorGetX(X);
}

inline Vector3D Vector3D::lerp(const Vector3D& v0, const Vector3D& v1, float t)
{
    DirectX::XMVECTOR x0 = DirectX::XMLoadFloat3(&v0);
    DirectX::XMVECTOR x1 = DirectX::XMLoadFloat3(&v1);
    DirectX::XMVECTOR X = DirectX::XMVectorLerp(x0, x1, t);

    Vector3D result;
    DirectX::XMStoreFloat3(&result, X);
    return result;
}


#pragma once

#include <DirectXMath.h>
using namespace DirectX;

#include "Vector3D.h"
#include "Vector4D.h"
#include "Quaternion.h"

class Matrix : public XMFLOAT4X4
{
public:
    Matrix() :
        XMFLOAT4X4(
        1.f, 0, 0, 0,
        0, 1.f, 0, 0,
        0, 0, 1.f, 0,
        0, 0, 0, 1.f) {}

    Matrix(float m00, float m01, float m02, float m03,
        float m10, float m11, float m12, float m13,
        float m20, float m21, float m22, float m23,
        float m30, float m31, float m32, float m33) :
        XMFLOAT4X4(m00, m01, m02, m03,
        m10, m11, m12, m13,
        m20, m21, m22, m23,
        m30, m31, m32, m33) {}

    explicit Matrix(const Vector3D& r0, const Vector3D& r1, const Vector3D& r2) :
        XMFLOAT4X4(r0.x, r0.y, r0.z, 0,
        r1.x, r1.y, r1.z, 0,
        r2.x, r2.y, r2.z, 0,
        0, 0, 0, 1.f) {}
    explicit Matrix(const Vector4D& r0, const Vector4D& r1, const Vector4D& r2, const Vector4D& r3) :
        XMFLOAT4X4(r0.x, r0.y, r0.z, r0.w,
        r1.x, r1.y, r1.z, r1.w,
        r2.x, r2.y, r2.z, r2.w,
        r3.x, r3.y, r3.z, r3.w) {}

    void fromXMMATRIX(CXMMATRIX M){ XMStoreFloat4x4(this, M); }
    XMMATRIX toXMMATRIX() const { return XMLoadFloat4x4(this); }

    // Operators 
    Matrix operator*(const Matrix& M) const;
    Matrix& operator*= (const Matrix& M);

    bool decompose(Vector3D& outScale, Quaternion& outQuat, Vector3D& outTranslation) const;
    void transpose();
    void inverse();
    float determinant();

    void makeOrthoProjLH(float width, float height, float nearZ, float farZ)
    {
        //fromXMMATRIX(XMMatrixOrthographicLH(width, height, nearZ, farZ));
        // Construct a x-right, y-down, UpperLeft-origin orthogonal projection matrix
        float left = 0.0f; // .5f
        float right = left + width;
        float top = 0.0f; //.5f
        float bottom = top + height;
        fromXMMATRIX(DirectX::XMMatrixOrthographicOffCenterLH(left, right, bottom, top, nearZ, farZ));
    }

    void makePerspectiveProjLH(float angleY, float ratio, float nearZ, float farZ)
    {
        fromXMMATRIX(DirectX::XMMatrixPerspectiveFovLH(angleY, ratio, nearZ, farZ));
    }
};

inline Matrix Matrix::operator*(const Matrix& M) const
{
    XMMATRIX m0 = XMLoadFloat4x4(this);
    XMMATRIX m1 = XMLoadFloat4x4(&M);

    Matrix result;
    XMStoreFloat4x4(&result, XMMatrixMultiply(m0, m1));
    return result;
}

inline Matrix& Matrix::operator*=(const Matrix& M)
{
    XMMATRIX m0 = XMLoadFloat4x4(this);
    XMMATRIX m1 = XMLoadFloat4x4(&M);

    XMStoreFloat4x4(this, XMMatrixMultiply(m0, m1));
    return *this;
}

inline bool Matrix::decompose(Vector3D& outScale, Quaternion& outQuat, Vector3D& outTranslation) const
{
    XMVECTOR s, r, t;

    if (!XMMatrixDecompose(&s, &r, &t, this->toXMMATRIX()))
        return false;

    XMStoreFloat3(&outScale, s);
    XMStoreFloat4(&outQuat, r);
    XMStoreFloat3(&outTranslation, t);

    return true;
}

inline void Matrix::transpose()
{
    XMStoreFloat4x4(this, XMMatrixTranspose(this->toXMMATRIX()));
}

inline void Matrix::inverse()
{
    XMVECTOR det;
    XMStoreFloat4x4(this, XMMatrixInverse(&det, this->toXMMATRIX()));
}

inline float Matrix::determinant()
{
    return XMVectorGetX(XMMatrixDeterminant(this->toXMMATRIX()));
}

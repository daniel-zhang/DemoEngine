#pragma once

#include <DirectXMath.h>

#include "Vector3D.h"
#include "Vector4D.h"
#include "Quaternion.h"
#include "StringFunction.h"

class Matrix : public DirectX::XMFLOAT4X4
{
public:
    Matrix() :
        DirectX::XMFLOAT4X4(
        1.f, 0, 0, 0,
        0, 1.f, 0, 0,
        0, 0, 1.f, 0,
        0, 0, 0, 1.f) {}

    std::string toDebugString() const
    {
        std::string output;
        for (int i = 0; i < 4; ++i)
        {
            output += string_format("row_%d: %f, %f, %f, %f\n", i, this->m[i][0], this->m[i][1], this->m[i][2], this->m[i][3]);
        }
        return output;
    }

    Matrix(float m00, float m01, float m02, float m03,
        float m10, float m11, float m12, float m13,
        float m20, float m21, float m22, float m23,
        float m30, float m31, float m32, float m33) :
        DirectX::XMFLOAT4X4(m00, m01, m02, m03,
        m10, m11, m12, m13,
        m20, m21, m22, m23,
        m30, m31, m32, m33) {}

    explicit Matrix(const Vector3D& r0, const Vector3D& r1, const Vector3D& r2) :
        DirectX::XMFLOAT4X4(r0.x, r0.y, r0.z, 0,
        r1.x, r1.y, r1.z, 0,
        r2.x, r2.y, r2.z, 0,
        0, 0, 0, 1.f) {}
    explicit Matrix(const Vector4D& r0, const Vector4D& r1, const Vector4D& r2, const Vector4D& r3) :
        DirectX::XMFLOAT4X4(r0.x, r0.y, r0.z, r0.w,
        r1.x, r1.y, r1.z, r1.w,
        r2.x, r2.y, r2.z, r2.w,
        r3.x, r3.y, r3.z, r3.w) {}

    void fromXMMATRIX(DirectX::CXMMATRIX M){ DirectX::XMStoreFloat4x4(this, M); }
    DirectX::XMMATRIX toXMMATRIX() const { return DirectX::XMLoadFloat4x4(this); }

    // Operators 
    Matrix operator*(const Matrix& M) const;
    Matrix& operator*= (const Matrix& M);

    bool decompose(Vector3D& outScale, Quaternion& outQuat, Vector3D& outTranslation) const;
    void transpose();
    void inverse();
    float determinant();

    // Construct a x-right, y-down, TopLeft-origin orthogonal projection matrix
    void makeOrthoProjLH(float width, float height, float nearZ, float farZ)
    {
        //fromXMMATRIX(DirectX::XMMatrixOrthographicLH(width, height, nearZ, farZ));
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
    DirectX::XMMATRIX m0 = DirectX::XMLoadFloat4x4(this);
    DirectX::XMMATRIX m1 = DirectX::XMLoadFloat4x4(&M);

    Matrix result;
    DirectX::XMStoreFloat4x4(&result, DirectX::XMMatrixMultiply(m0, m1));
    return result;
}

inline Matrix& Matrix::operator*=(const Matrix& M)
{
    DirectX::XMMATRIX m0 = DirectX::XMLoadFloat4x4(this);
    DirectX::XMMATRIX m1 = DirectX::XMLoadFloat4x4(&M);

    DirectX::XMStoreFloat4x4(this, DirectX::XMMatrixMultiply(m0, m1));
    return *this;
}

inline bool Matrix::decompose(Vector3D& outScale, Quaternion& outQuat, Vector3D& outTranslation) const
{
    DirectX::XMVECTOR s, r, t;

    if (!DirectX::XMMatrixDecompose(&s, &r, &t, this->toXMMATRIX()))
        return false;

    DirectX::XMStoreFloat3(&outScale, s);
    DirectX::XMStoreFloat4(&outQuat, r);
    DirectX::XMStoreFloat3(&outTranslation, t);

    return true;
}

inline void Matrix::transpose()
{
    DirectX::XMStoreFloat4x4(this, DirectX::XMMatrixTranspose(this->toXMMATRIX()));
}

inline void Matrix::inverse()
{
    DirectX::XMVECTOR det;
    DirectX::XMStoreFloat4x4(this, DirectX::XMMatrixInverse(&det, this->toXMMATRIX()));
}

inline float Matrix::determinant()
{
    return DirectX::XMVectorGetX(DirectX::XMMatrixDeterminant(this->toXMMATRIX()));
}

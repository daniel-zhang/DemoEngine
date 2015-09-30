#pragma once

#include <DirectXMath.h> // Included in Win8.1 SDK 
using namespace DirectX; // Required this funny namespace

namespace Math
{
    //
    // Common const values
    //

    XMGLOBALCONST float PI      = 3.1415926535897932f;
    XMGLOBALCONST float INV_PI  = 0.31830988618f;
    XMGLOBALCONST float HALF_PI = 1.57079632679f;

    //
    // Wrappers for std math functions
    //

    // nearest integer not greater in magnitude than arg.
    template <typename T>
    static int32_t trunc(T val);

    // nearest integer not greater than the given value 
    template <typename T>
    static T floor(T val);

    // nearest integer not less than the given value 
    template <typename T>
    static T ceil(T val);

    // nearest integer, rounding away from zero in halfway cases
    template <typename T>
    static T round(T val);

    template <typename T>
    static T clamp(T lower, T upper, T val);

    template <typename T>
    static T radian2degree(const T& rad);

    template <typename T>
    static T degree2radian(const T& degree);

    static XMMATRIX inverseTranspose(CXMMATRIX M);
};

// .INL implementation
template <typename T> 
inline int32_t Math::trunc(T val) { return static_cast<int32_t>(std::trunc(val)); }

template <typename T> 
inline T Math::floor(T val) { return std::floor(val); }

template <typename T> 
inline T Math::ceil(T val) { return std::ceil(val); }

template <typename T> 
inline T Math::round(T val) { return std::round(val); }

template <typename T>
inline T Math::clamp(T lower, T upper, T val)
{
    return val < lower ? lower : (val > upper ? upper : val);
}

template <typename T>
inline T Math::radian2degree(const T& rad)
{
    return rad * 180.f * Math::INV_PI;
}

template <typename T>
inline T Math::degree2radian(const T& degree)
{
    return degree * Math::PI / 180.f;
}

inline XMMATRIX Math::inverseTranspose(CXMMATRIX M)
{
    // Inverse-transpose is just applied to normals.  So zero out 
    // translation row so that it doesn't get into our inverse-transpose
    // calculation--we don't want the inverse-transpose of the translation.
    XMMATRIX A = M;
    A.r[3] = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);

    XMVECTOR det = XMMatrixDeterminant(A);
    return XMMatrixTranspose(XMMatrixInverse(&det, A));
}

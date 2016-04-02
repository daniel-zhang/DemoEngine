#pragma once

#include <cmath>

namespace Math
{
    // Common const values, stolen from ue4
    XMGLOBALCONST float PI                 = 3.1415926535897932f;
    XMGLOBALCONST float INV_PI             = 0.31830988618f;
    XMGLOBALCONST float HALF_PI            = 1.57079632679f;
    XMGLOBALCONST float SMALL_NUMBER       = (1.e-8f);
    XMGLOBALCONST float KINDA_SMALL_NUMBER = (1.e-4f);
    XMGLOBALCONST float BIG_NUMBER         = (3.4e+38f);
    XMGLOBALCONST float EULERS_NUMBER      = (2.71828182845904523536f);

    // trunc, floor, ceil, round
    // Supplement to std::cmath library
    template <typename T>
    FORCEINLINE T clamp(const T& lower, const T& upper, const T& val)
    {
        return val < lower ? lower : (val > upper ? upper : val);
    }

    template <typename T>
    FORCEINLINE T radian2degree(const T& rad)
    {
        return rad * 180.f * INV_PI;
    }

    template <typename T>
    FORCEINLINE T degree2radian(const T& degree)
    {
        return degree * PI / 180.f;
    }

    template <typename T>
    FORCEINLINE bool isNearlyZero(T left, T right, float tolerance = SMALL_NUMBER)
    {
        return std::abs(left - right) < tolerance;
    }

    template <typename T>
    FORCEINLINE bool isNearlyZero(T value, float tolerance = SMALL_NUMBER)
    {
        return std::abs(value) < tolerance;
    }

    FORCEINLINE bool isPowerOfTwo(uint32_t value)
    {
        return ((value & (value - 1)) == 0);
    }

};

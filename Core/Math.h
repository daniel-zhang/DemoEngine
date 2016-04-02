#pragma once

// Wrappers for XNA/DirectX's SSE math.
#include "Vector2D.h"
#include "Vector3D.h"
#include "Vector4D.h"
#include "Quaternion.h"
#include "Matrix.h"
#include "Color.h"
#include "Palette.h"
#include <cmath>

namespace Math
{
    // Common const values, stolen from ue4
    XMGLOBALCONST float PI = 3.1415926535897932f;
    XMGLOBALCONST float INV_PI = 0.31830988618f;
    XMGLOBALCONST float HALF_PI = 1.57079632679f;
    XMGLOBALCONST float SMALL_NUMBER = (1.e-8f);
    XMGLOBALCONST float KINDA_SMALL_NUMBER = (1.e-4f);
    XMGLOBALCONST float BIG_NUMBER = (3.4e+38f);
    XMGLOBALCONST float EULERS_NUMBER = (2.71828182845904523536f);

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

    /** Performs a linear interpolation between two values, Alpha ranges from 0-1 */
    template< class T, class U >
    FORCEINLINE T lerp(const T& A, const T& B, const U& Alpha)
    {
        return (T)(A + Alpha * (B - A));
    }

    /** Performs a linear interpolation between two values, Alpha ranges from 0-1. Handles full numeric range of T */
    template< class T >
    FORCEINLINE T lerpStable(const T& A, const T& B, double Alpha)
    {
        return (T)((A * (1.0 - Alpha)) + (B * Alpha));
    }

    /** Performs a 2D linear interpolation between four values values, FracX, FracY ranges from 0-1 */
    template< class T, class U >
    FORCEINLINE T biLerp(const T& P00, const T& P10, const T& P01, const T& P11, const U& FracX, const U& FracY)
    {
        return lerp(
            lerp(P00, P10, FracX),
            lerp(P01, P11, FracX),
            FracY
            );
    }

    /**
    * Performs a cubic interpolation
    *
    * @param  P - end points
    * @param  T - tangent directions at end points
    * @param  Alpha - distance along spline
    *
    * @return  Interpolated value
    */
    template< class T, class U >
    FORCEINLINE T cubicInterp(const T& P0, const T& T0, const T& P1, const T& T1, const U& A)
    {
        const float A2 = A  * A;
        const float A3 = A2 * A;

        return (T) ( P0 * ((2 * A3) - (3 * A2) + 1) ) 
            + ( T0 * (A3 - (2 * A2) + A) ) 
            + ( T1 * (A3 - A2) ) 
            + ( P1 * ((-2 * A3) + (3 * A2)) );
    }
};

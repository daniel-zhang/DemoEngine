#pragma once

#include <DirectXMath.h>
using namespace DirectX;

class Quaternion : public XMFLOAT4
{
public:

    // Spherical linear interpolation
    static void slerp(const Quaternion& q0, const Quaternion& q1, float t, Quaternion& outQ);
};

inline void Quaternion::slerp(const Quaternion& q0, const Quaternion& q1, float t, Quaternion& outQ)
{
    XMVECTOR Q0 = XMLoadFloat4(&q0);
    XMVECTOR Q1 = XMLoadFloat4(&q1);
    XMStoreFloat4(&outQ, XMQuaternionSlerp(Q0, Q1, t));
}

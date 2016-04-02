#pragma once

#include <DirectXMath.h>

class Quaternion : public DirectX::XMFLOAT4
{
public:

    // Spherical linear interpolation
    static void slerp(const Quaternion& q0, const Quaternion& q1, float t, Quaternion& outQ);
};

inline void Quaternion::slerp(const Quaternion& q0, const Quaternion& q1, float t, Quaternion& outQ)
{
    DirectX::XMVECTOR Q0 = DirectX::XMLoadFloat4(&q0);
    DirectX::XMVECTOR Q1 = DirectX::XMLoadFloat4(&q1);
    DirectX::XMStoreFloat4(&outQ, DirectX::XMQuaternionSlerp(Q0, Q1, t));
}

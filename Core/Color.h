#pragma once
#include <DirectXMath.h>

class Color : public DirectX::XMFLOAT4
{
public:
    Color() : DirectX::XMFLOAT4(0.f, 0.f, 0.f, 1.f){}
    Color(float _r, float _g, float _b) : DirectX::XMFLOAT4(_r, _g, _b, 1.f) {}
    Color(float _r, float _g, float _b, float _a) : DirectX::XMFLOAT4(_r, _g, _b, _a) {}
};

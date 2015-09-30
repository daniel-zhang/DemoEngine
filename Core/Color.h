#pragma once
#include <DirectXMath.h>
using namespace DirectX;

class Color : public XMFLOAT4
{
public:
    Color() : XMFLOAT4(0.f, 0.f, 0.f, 1.f){}
    Color(float _r, float _g, float _b) : XMFLOAT4(_r, _g, _b, 1.f) {}
    Color(float _r, float _g, float _b, float _a) : XMFLOAT4(_r, _g, _b, _a) {}
};

#pragma once
#include "Core/MiscMath.h"

void test_math()
{
    XMMATRIX i = XMMatrixIdentity();
    XMMATRIX j = Math::inverseTranspose(i);

}


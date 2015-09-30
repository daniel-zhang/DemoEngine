#pragma once

#include "Core/Util.h"

void test_util()
{
    check(1 == 2);
    vcheck(3 == 4, L"Some error");

    check_hr(E_FAIL);
    vcheck_hr(E_FAIL, L"Some D3D error");

}
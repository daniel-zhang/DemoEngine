#include "Test_Util.h"
#include "Test_App.h"
#include "dummy.h"
#include <Windows.h>

// TODO: add a test selector GUI
int WINAPI wWinMain(__in HINSTANCE hInstance, __in_opt HINSTANCE hPrevInstance, __in LPWSTR lpCmdLine, __in int nShowCmd)
{
    //test_util();
    testApp();
    dummyTest();

    return 0;
}
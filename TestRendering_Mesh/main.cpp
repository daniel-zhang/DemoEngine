
/*
#include "Application/Application.h"
#include "Application/DebugMessageHandler.h"

int WINAPI wWinMain(__in HINSTANCE hInstance, __in_opt HINSTANCE hPrevInstance, __in LPWSTR lpCmdLine, __in int nShowCmd)
{
CoInitializeEx(nullptr, COINIT_MULTITHREADED);

Application::getInstance()->initialize(nullptr);

WindowDefinition def_normal;
def_normal.HasOSWindowBorder = true;
def_normal.SupportsTransparency = false;

Application::getInstance()->addWindow(def_normal);

TickTimer timer;
timer.reset();

while (Application::getInstance()->hasWindows())
{
Application::getInstance()->pumpMsg();
float dt = (float)timer.tick();
}

CoUninitialize();
return 0;
}
*/

#include "TestRenderer_Mesh.h"
#include "Application/Application.h"
#include "Application/DebugMessageHandler.h"

int WINAPI wWinMain(__in HINSTANCE hInstance, __in_opt HINSTANCE hPrevInstance, __in LPWSTR lpCmdLine, __in int nShowCmd)
{
    CoInitializeEx(nullptr, COINIT_MULTITHREADED);

    DebugMessageHandler dmh;
    Application::getInstance()->initialize(nullptr, &dmh);

    WindowDefinition def_normal;
    def_normal.HasOSWindowBorder = true;
    def_normal.SupportsTransparency = false;

    Window* wnd = Application::getInstance()->addWindow(def_normal);

    TestRenderer_Mesh meshRenderer;
    meshRenderer.initialize(wnd->getHwnd());

    TickTimer timer;
    timer.reset();

    while (Application::getInstance()->hasWindows())
    {
        Application::getInstance()->pumpMsg();

        float dt = (float)timer.tick();

        meshRenderer.testDraw(dt);
    }

    CoUninitialize();
    return 0;
}
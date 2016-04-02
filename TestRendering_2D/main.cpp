#include "Application/Application.h"
#include "Application/DebugMessageHandler.h"
#include "TestRenderer_2D.h"

int WINAPI wWinMain(__in HINSTANCE hInstance, __in_opt HINSTANCE hPrevInstance, __in LPWSTR lpCmdLine, __in int nShowCmd)
{
    CoInitializeEx(nullptr, COINIT_MULTITHREADED);

    DebugMessageHandler dmh;
    Application::getInstance()->initialize(nullptr, &dmh);

    WindowDefinition def_normal;
    def_normal.HasOSWindowBorder = true;
    def_normal.SupportsTransparency = false;

    Window* wnd = Application::getInstance()->addWindow(def_normal);

    TestRenderer_2D renderer;
    renderer.initialize(wnd->getHwnd());

    TickTimer timer;
    timer.reset();
    while (Application::getInstance()->hasWindows())
    {
        Application::getInstance()->pumpMsg();
        float dt = (float)timer.tick();

        renderer.testDraw();
    }

    CoUninitialize();
    return 0;
}

#include "TestRenderer_VE.h"
#include "Application/Application.h"
#include "Application/DebugMessageHandler.h"

int WINAPI wWinMain(__in HINSTANCE hInstance, __in_opt HINSTANCE hPrevInstance, __in LPWSTR lpCmdLine, __in int nShowCmd)
{
    CoInitializeEx(nullptr, COINIT_MULTITHREADED);

    DebugMessageHandler debugMsgHandler;
    Application::getInstance()->initialize(nullptr, &debugMsgHandler);

    WindowDefinition def_normal;
    def_normal.HasOSWindowBorder = true;
    def_normal.SupportsTransparency = false;
    def_normal.DesiredWidth = 800;
    def_normal.DesiredHeight = 600;

    Window* wnd_normal = Application::getInstance()->addWindow(def_normal);

    std::vector<HWND> hwnds;
    hwnds.push_back(wnd_normal->getHwnd());

    TestRenderer_VE veRenderer;
    veRenderer.initialize(hwnds);

    TickTimer timer;
    timer.reset();

    while (Application::getInstance()->hasWindows())
    {
        Application::getInstance()->pumpMsg();

        float dt = (float)timer.tick();

        veRenderer.testDraw(dt);
    }

    CoUninitialize();
    return 0;
}

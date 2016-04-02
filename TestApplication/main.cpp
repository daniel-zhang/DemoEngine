#include "Application/Application.h"
#include "Application/DebugMessageHandler.h"

int WINAPI wWinMain(__in HINSTANCE hInstance, __in_opt HINSTANCE hPrevInstance, __in LPWSTR lpCmdLine, __in int nShowCmd)
{
    DebugMessageHandler debugMsgHandler;

    Application::getInstance()->initialize(nullptr, &debugMsgHandler); 

    WindowDefinition def_normal;
    def_normal.HasOSWindowBorder    = true;
    def_normal.SupportsTransparency = false;

    WindowDefinition def_custom;
    def_custom.HasOSWindowBorder    = false;
    def_custom.SupportsTransparency = true;
    def_custom.Opacity              = .5f;
    def_custom.IsTopmostWindow      = false;

    WindowDefinition def_custom_opac;
    def_custom_opac.HasOSWindowBorder    = false;
    def_custom_opac.SupportsTransparency = false;
    def_custom_opac.HasSizingFrame       = false;
    def_custom_opac.IsTopmostWindow      = false;


    Application::getInstance()->addWindow(def_normal);
    Application::getInstance()->addWindow(def_custom);
    Application::getInstance()->addWindow(def_custom_opac);

    // Run
    while (Application::getInstance()->hasWindows())
    {
        Application::getInstance()->pumpMsg();
    }

    // Pump several times for possible WM_DESTROY messages
    for (uint32_t i = 0; i < 10; ++i)
    {
        Application::getInstance()->pumpMsg();
    }

    return 0;
}
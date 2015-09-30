#include "Test_App.h"
#include "Engine/Application.h"

void testApp()
{
    Application::initialize();

    Window* defaultWnd = Window::buildDefaultWindow();
    Window* noBorderWnd = Window::buildBorderlessWindow();

    Application::getInstance()->addWindow(Window::buildDefaultWindow());
    //Application::getInstance()->addWindow(Window::buildBorderlessWindow());

    Application::getInstance()->run();

}
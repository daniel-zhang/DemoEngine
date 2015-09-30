#pragma once
#include "MessageHandler.h"
#include "Window.h"

class Win32Application;

class Application : public MessageHandler
{
public:
    static void initialize()
    {
        if (mApp == nullptr) mApp = new Application();
    }

    static Application* getInstance(){ return mApp; }

    ~Application();

public:

    /************************************************************************/
    /* MessageHandler interface implementation                              */
    /************************************************************************/
    // App life cycle
    void onApplicationActivationChanged(const bool isActive) override;

    void onWindowActivationChanged(const Win32Window* wnd, const EWindowActivation activationType) override;

    void onWindowClose(const Win32Window* wnd) override;

    // Input
    bool shouldProcessUserInputMessages(const Win32Window* wnd) const override;


    /************************************************************************/
    /*Local methods                                                         */
    /************************************************************************/
    void addWindow(Window* window, bool showImmediately = true);

    void run();

    void closeWindow(Window* window);

private:

    void tick();

    void destroy();

    Window* findWindow(const Win32Window* nativeWindow) const;

private:
    // Make singleton
    static Application* mApp;

    Application();
    Application(const Application&);
    Application& operator=(const Application&);

private:
    Win32Application* mNativeApp;

    std::vector<Window*> mWindows;

    bool mExit;
    bool mIsActivated;
};
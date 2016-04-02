#pragma once
#include "MessageHandler.h"
#include "Window.h"
#include "Win32Application.h"

class Application : public MessageHandler
{
public:
    static Application* getInstance()
    {
        static Application instance;

        return &instance; 
    }

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

    template<typename Lambda>
    void run(Lambda& delegate)
    {
        while (!mExit)
        {
            // Only tick when thread msg queue is empty
            if (mNativeApp->pumpMsg() == false)
            {
                delegate();
            }
        }
    }

    void closeWindow(Window* window);

private:

    void destroy();

    Window* findWindow(const Win32Window* nativeWindow) const;

private:
    Application();
    Application(const Application&);
    Application& operator=(const Application&);

private:
    Win32Application* mNativeApp;

    std::vector<Window*> mWindows;

    bool mExit;
    bool mIsActivated;
};
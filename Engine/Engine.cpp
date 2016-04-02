#include "Engine.h"
#include "QApplication.h"
#include "Scene.h"
#include "Renderer/Renderer.h"

Engine::Engine() : 
    //mScene(nullptr), 
    mRenderer(nullptr), 
    mQApp(nullptr)
{
}

Engine::~Engine()
{

}

void Engine::initialize(UITestInterface* uiTestInterface)
{
    CoInitializeEx(nullptr, COINIT_MULTITHREADED);

    mRenderer = Renderer::getInstance();
    mRenderer->initialize(RendererSettings());

    // QApplication initialization depends on renderer
    mQApp = QApplication::getInstance();
    mQApp->initialize(uiTestInterface);
    mQApp->setModalHandler(this);

    //mScene->initialize();

    mTimer.reset();
}

void Engine::modalTick()
{
    float dt = (float)mTimer.tick();
    // Do not tick QApp here...

    mRenderer->drawGui(mQApp->getDrawData());
}

void Engine::run()
{
    while ( !mQApp->aboutToExit() )
    {
        float dt = (float)mTimer.tick();

        //mScene->update(dt);

        mQApp->tick(dt);

        mRenderer->drawGui(mQApp->getDrawData());
    }

    CoUninitialize();
}


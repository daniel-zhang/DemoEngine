#pragma once

#include "Core/Util.h"
#include "UITestInterface.h"
#include "QApplication.h"

/**************************************************************************************************************************
Memo

Engine class acts as a container for subsystems 

Subsystems are implicit 'engine components'. 

By implicit I mean subsystems are not forced to derive from a common EngineComponent interface(which is supposed to provide 
the arguably 'standard' component communication message facility). That would be an overkill.

But they should act like one.

Subsystems are made independent, each consumes an input and optionally produces an output. e.g:
    GUI takes user input and maintains internal states, and outputs a draw buffer (plus a translated user input)
    Scene takes user input from GUI, does whatever it wants with it, then outputs a draw buffer
    Renderer feeds on draw buffers, and produces a rendered screen.

Each of them is independent, they don't talk to each other. Instead, they talk to the Engine and let the Engine decide
what to do with them.

Tricky stuff:
Scenes are rendered to textures, which are consumed by GameSceneWidget, which, together with other widgets in the GUI hierarchy,
gets rendered to backbuffer.

This is the sane choice, as switching between UI rendering and 3D scene rendering is definitely a performance kill.

So before the GUI hierarchy gets rendered, any GameSceneWidget contained in that hierarchy must have their textures already 
rendered from a scene. 

That implies scenes have to be rendered first. But to render a scene, the renderer has to know multiple things like render target, 
viewport dimensions, projection matrix, etc., which are determined by GUI.

It's the engine's job to deal with this mess. To draw a frame, the engine has to:
    1. Find all GameSceneWidgets in the GUI hierarchy
    2. Figure out which scene a GameSceneWidget is corresponding to
    3. Figure out a GameSceneWidget's render target and camera 
    4. Feed the scene's draw buffer, together with render target//projection data to renderer to perform render-to-texture.
    5. Finally, feeds the GUI draw buffer to renderer

Most of the widgets don't have to know anything beyond the scope of GUI. But for GameSceneWidget, it has to know about Scene
(which camera to use) and Renderer(which render target to draw on).

**************************************************************************************************************************/
class Engine : public IModalHandler
{
    // Make singleton
public:
    static Engine* getInstance()
    {
        static Engine instance;
        return &instance;
    }
    ~Engine();

private:
    Engine();
    Engine(const Engine&);
    Engine& operator=(const Engine&);

public:
    void initialize(UITestInterface* uiTestInterface);

    void run();

    virtual void modalTick() override;

protected:
    TickTimer mTimer;
    //class Scene* mScene;
    class Renderer* mRenderer;
    class QApplication* mQApp;
};
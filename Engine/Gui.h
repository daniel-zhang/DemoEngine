
/*
Some random, irrelevant thoughts that occurred to me. 

>> Automatic content creation 
The concept is appealing. But in reality, not much progress is made yet. 

Someone created a particle system that automatically generates weapon effects, analyzing player 
data via genetic algorithm. 

>> Player guided content creation -- is this doable?
Play-in-editor feature makes life easier for a game designer. But what I love to see is edit-in-game, which 
allows for content creation by (regular) players.

However, this is more than just putting an editor in game window, because: 

    playing game is easy, making game is hard.

>> Mitigate the difficulty
It takes considerable efforts to create/organize animations and states and logics to present convincing character movements
in a game. But once the creation is done, the result is not changeable anymore. Yes a game can be updated, but never at a 
per-animation level. Generally, the complexity of a game stops increasing once shipped. And eventually, the player gets bored, 
the painstakingly created game content is thrown away. 

what a waste!

Is it possible to 'generate' animations on the fly? 

There are papers about generating walking animation via genetic algorithms. Some of the results look very convincing. But despite 
this, and the fancy terms, the method they use is essentially an optimization search in a carefully handcrafted parameter space, 
guided by hard-coded criteria. It's less flexible, and requires too much 'human intervention'.

Deep learning seems more interesting. Some image recognition guys showed that their deep learning program can 'dream'--it generates 
weird pictures when the input is set from itself(i.e., a layer of its neural network). Instead of passively classifying pictures like it
normally does, in the dream it actually creates something new(and occasionally, creepy).

How about targeting a deep learning program to animations? Can it dream new animations? 
*/

#pragma once

#include "Core/Util.h"

// Dummy
class Widget{};

class GameViewport : public Widget
{
public:

    class Scene* mScene; // game scene to draw
    class D3D_OffscreenRenderTarget* rt; // render target for the game scene
};

// Generated by a top level pancake window
struct WindowDrawBuffer
{
    class Hwnd* hwnd;
    class D3D_HwndRenderTarget* rt;
    std::vector< class DrawElement*> drawElements;
};

class Gui
{
public:
    void initialize()
    {
        // Create render targets
        // When a GameViewport is being created, it requires the knowledge of which scene it is going to draw
        // and it decides which camera in that scene is going to be used.

    }

    void update(float dt){}

    // Engine uses this information to construct a proper scene_draw_buffer that is to be consumed by the renderer
    std::vector<GameViewport*>* getGameViewports()
    {
        // Traverse Widgets hierarchy to find out all GameViewports
        // Or make Gameviewport register itself to an dedicated array, which is returned here.
        return nullptr;
    }

    // Engine uses this to feed the renderer for UI rendering
    WindowDrawBuffer* getWindowDrawBuffer(){ return nullptr; }

protected:
};


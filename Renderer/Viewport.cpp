#include "Viewport.h"

#include "Renderer.h"

void Viewport::create()
{
    mCtx = Renderer::getInstance()->getContext();
}

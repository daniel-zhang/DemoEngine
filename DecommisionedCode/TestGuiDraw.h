#pragma once

#include "Renderer.h"

class GuiDrawTest
{
public:
    GuiDrawTest() : mRenderer(nullptr)
    {}

    void initialize(std::vector<HWND>& hwnds);

    void testDraw(float dt) { mRenderer->drawGui(mTestDrawGroups); }

protected:
    Renderer* mRenderer;
    VisualElementGroups mTestDrawGroups;
};
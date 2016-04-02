#pragma once

#include "Renderer/Renderer.h"

class TestRenderer_VE
{
public:
    TestRenderer_VE() : mRenderer(nullptr)
    {}

    void initialize(std::vector<HWND>& hwnds);

    void testDraw(float dt);

protected:
    Renderer* mRenderer;
    VisualElementGroups mTestDrawGroups;
};
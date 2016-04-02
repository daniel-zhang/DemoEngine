#pragma once

#include "QWindow.h"
class UITestInterface
{
public:
    virtual void createUI(std::vector<QWindow*>& rootQWindows) = 0;
};
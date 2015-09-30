#pragma once

#include "Engine/MessageHandler.h"

class MinimaMsgHandler : public MessageHandler
{
public:
    bool shouldProcessUserInputMessages(const Win32Window* wnd) const override
    {
        return true;
    }



};
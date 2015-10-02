#pragma once

#include "Core/Util.h"

class DrawBuffer
{
public:


};

class IRenderResource
{
public:
    virtual void create(){}
    virtual void destroy(){}
    virtual void recreate(){}
    virtual const std::wstring& getName(){ return L"Readable Name"; }
};

class Renderer
{
public:
    static void initialize();

    static Renderer* getInstance();

private:
    Renderer();
    Renderer(const Renderer&);
    Renderer& operator=(const Renderer&);

    static Renderer* mInstance;
};
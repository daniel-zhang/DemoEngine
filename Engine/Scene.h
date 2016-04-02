#pragma once

#include "Core/Util.h"
#include "Entity.h"

// A really simple flat scene structure
class Scene
{
public:
    Scene() { }

    ~Scene() { destroy(); }

    void destroy()
    {
        for (auto node : mEntities)
        {
            delete node;
            node = nullptr;
        }
    }

public:
    void initialize()
    {
    }

    void add(Entity* entity)
    {
        mEntities.emplace_back(entity);
    }

    bool remove(Entity* entity)
    {
        check(entity);

        for (std::vector<Entity*>::iterator it = mEntities.begin(); it != mEntities.end(); ++it)
        {
            if (*it == entity)
            {
                mEntities.erase(it);
                return true;
            }
        }
        return false;
    }

    void update(float dt)
    {
        for (auto entity : mEntities)
            entity->update(dt);
    }

protected:
    std::vector<Entity*> mEntities;
};
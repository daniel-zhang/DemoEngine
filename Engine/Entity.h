#pragma once 

#include "Components.h"

/*
A components container
*/

class Entity
{
public:
    Entity(){}
    ~Entity() {}

    void update(float dt)
    {

    }

protected:
    MeshComponent* mMesh;
    PositionComponent* mPosition;

};

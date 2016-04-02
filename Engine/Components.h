#pragma once


class MeshComponent
{
public:

};

#include "Core/Math.h"

class PositionComponent
{
public:
    void walk(float dt);
    void strife(uint32_t si);

    void pitch(float dt);
    void yaw();
    void roll();

    void setPosition();
    void setRotation();

    void setWalkSpeed();
    void setStrifeSpeed()
    {
    }
     


protected:

protected:
    Vector3D mScaling;
    Vector3D mRotationAngels;
    Vector3D mTranslation;
};


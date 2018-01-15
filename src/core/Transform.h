#pragma once
#include "../defines.h"
#include "../defineTypes.h"
class Transform
{
    Transform(){};
    ~Transform(){};
    Vec3& Pos(){return pos;}
    Vec3& Size(){return size;}
    Vec3& Rotation(){return rotation;}
    Vec3& Center(){return rotation;}
    int& Layer(){return layer;}
private:
    Vec3 pos;
    Vec3 size;
    Vec3 rotation;
    Vec3 center;
    int layer;
}
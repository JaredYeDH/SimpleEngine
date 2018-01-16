#pragma once
#include "../defines.h"
#include "../defineTypes.h"

class Transform
{
public:
    Transform();
    
    ~Transform();
//     Vec3& pos(){return pos;}
//     Vec3& size(){return size;}
//     Vec3& scale(){return scale;}
//     Vec3& rotation(){return rotation;}
//     Vec3& center(){return rotation;}
//     int& layer(){return layer;}
    
// private:
    Vec3 pos;
    Vec3 size;
    Vec3 scale;
    Vec3 rotation;
    Vec3 center;
    int layer;
};
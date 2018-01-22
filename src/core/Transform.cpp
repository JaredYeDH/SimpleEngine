#include "Transform.h"

Transform::Transform()
{
    pos.x = pos.y = pos.z = 0;
    center.x = center.y = center.z = 0;
    size.x = size.y = size.z = 0;
    scale.x= scale.y = scale.z = 1;
    rotation.x= rotation.y = rotation.z = 0;
    
    layer = 0;
};

Transform::~Transform()
{

};
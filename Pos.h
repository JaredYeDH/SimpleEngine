#pragma once
#include "defines.h"
struct Pos
{
    double x,y;
    Pos()
    {}
    Pos(double x, double y)
    {
        this->x =x;
        this->y = y;
    }
};

struct IntPos
{
	int32 x, y;
};
struct BoxPos
{
	int32 x, y;
};
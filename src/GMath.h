#pragma once
#include "Pos.h"
#define PI 3.1415926
#define DegreeToRadian(d) (d*PI/180.0f)
#define RadianToDegree(r) (r*180.0f/PI)

class GMath
{	
public:
	static double Astar_GetDistance(double sx, double sy, double ex, double ey);
    static double Astar_GetDistanceSquare(double sx, double sy, double ex, double ey);
	static double Astar_GetAngle(double sx, double sy, double ex, double ey);
	static int 	  Astar_GetAngleUseBoxXY(int sx, int sy, int ex, int ey);
	static int 	  Astar_GetDirUseInt(int degree);
	static int Astar_GetDir(double degree);
	static int Clamp(int value,int min,int max);
};

namespace utils
{
	inline static bool BoundHitTest(Bound bound, IntPos pt) { if (pt.x >= bound.left && pt.x <= bound.right&&pt.y >= bound.top&&pt.y <= bound.bottom)return true; else return false; }
}


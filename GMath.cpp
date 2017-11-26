#include "GMath.h"
#include <cmath>
#include "Logger.h"
double GMath::Astar_GetDistance(double sx, double sy, double ex, double ey)
{
	double dx = sx - ex;
	double dy = sy - ey;
	return sqrt(pow(dx,2) + pow(dy,2));
}

double GMath::Astar_GetDistanceSquare(double sx, double sy, double ex, double ey)
{
    double dx = sx - ex;
    double dy = sy - ey;
    return pow(dx,2) + pow(dy,2);
}

int GMath::Astar_GetAngleUseBoxXY(int sx, int sy, int ex, int ey)
{
	if(sx == ex && sy == ey )
	{
		Logger::Print("Astar_GetAngleUseBoxXY Error1!!!");
		return 360;  // not possible
	}
	
	int dx = ex - sx;
	int dy = ey - sy;
	if(dx == 0&&dy!=0)
	{	
		return dy > 0 ? 90 : 270;
	}
    if(dy == 0&&dx!=0)
	{
		return dx > 0 ? 0 : 180;
	}
    if(dx > 0&&dy!=0)
	{
		return dy > 0 ? 45 : 315;
	}
    if(dx < 0&&dy!=0)
	{
		return dy > 0 ? 135 : 225;
	}
	Logger::Print("Astar_GetAngleUseBoxXY Error2!!!");
	return 360;  // not possible
}

int GMath::Astar_GetDirUseInt(int degree) {
  //右下，左下，左上，右上，下，左，上，右
  // 0    1    2    3   4  5   6  7
  //  2 3 0 1       6 7   4 5
  //int innerDirs[8] = { 7,3,6,2,5,1,4,0 };
  if(degree<0)
  {
 	Logger::Print("Astar_GetDirUseInt Error!!!");
	return 7;  // not possible 	
  }
  int innerDirs[8] = { 7,0,4,1,5,2,6,3};
  return innerDirs[ degree%360 / 45];
}

double GMath::Astar_GetAngle(double sx, double sy, double ex, double ey)
{
	Logger::Print("Astar_GetAngle: sx=%lf sy=%lf ex=%lf ey=%lf\n", sx, sy, ex, ey);
	double dx = ex - sx;
	double dy = ey - sy;
	
	if (fabs(dx) <= 0.0001)
	{
		if (dy < 0)
			return 270;
		else
			return 90;
	}

	double angle;
	double degree;

	angle = atan(dy / dx);

	degree = RadianToDegree(angle);


	if (dx < 0 && dy >= 0) {
		degree += 180;
		if(fabs(dx) <=0.001f && dy >= 0)
		{
			degree = 270;
		}
	}
	else if (dx <=0 && dy<0) {
		degree += 180;

	}
	else if (dx > 0 && dy <0) {
		degree += 360;

	}
	else if (dx>=0 && dy>=0) {
		if(fabs(dx) <=0.001f && dy >= 0 && fabs(dy)<=0.001f)
		{
			degree = 270;
		}
	}
	return degree;
}

int GMath::Astar_GetDir(double degree) {
  //右下，左下，左上，右上，下，左，上，右
  // 0    1    2    3   4  5   6  7
  //  2 3 0 1       6 7   4 5
  //int innerDirs[8] = { 7,3,6,2,5,1,4,0 };
	int innerDirs[8] = { 7,0,4,1,5,2,6,3};
	if ((360 - 22.5 <= degree&&degree<360) || (degree>=0&& degree < 22.5)) {
		return innerDirs[0];
	}
	else {
		for (int i = 1; i < 8; i++) {
			if (i * 45 - 22.5 <= degree && degree < i * 45 + 22.5) {
				return innerDirs[i];
			}
		}
		return innerDirs[0];
	}
}



int GMath::Clamp(int value,int min,int max)
{
	if(value < min )value=min;
	if(value>max)value=max;
	return value;
}

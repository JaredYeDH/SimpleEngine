#pragma once
#include "../defines.h"
#include "../defineTypes.h"
#include "Shader.h"


class Object2D;
class Line;
class Rect;
class Circle;
class Image;

class Graphics2D
{
private:
	Shader m_Shader;
	Mat4 m_OrthoMat;

public: 
	Graphics2D();
	~Graphics2D();
	
	void Draw( Object2D& obj) ;
	void DrawLine(const Line& line);
	void DrawRect(const Rect& rect) ;
	void DrawCircle(const Circle& circle) ;
	void DrawImage(const Image& img) ;

	
};

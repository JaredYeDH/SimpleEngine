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
	Shader* m_ShaderPtr;
	Mat4 m_OrthoMat;
 	Shader* m_SpriteShaderPtr;
	GLuint m_SpriteVBO;
	GLuint m_SpriteVAO;
	GLfloat m_SpriteVertices[24];
public: 
	Graphics2D();
	~Graphics2D();
	void Begin();
	void End(); 
	void Draw( Object2D& obj) ;
	void DrawLine(const Line& line);
	void DrawRect(const Rect& rect) ;
	void DrawCircle(const Circle& circle) ;
	void DrawImage(const Image& img) ;

};

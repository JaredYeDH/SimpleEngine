#pragma once
#include "defines.h"
#include "defineTypes.h"

#include "Shader.h"
#include "Texture.h"
#include "Environment.h"
#include "Window.h"


enum struct RenderType
{
	Line,
	Rect,
	Circle,
	Texture
};

class RenderData
{

};

class RenderInfo
{
	RenderType type;
	RenderData data;
};

#define RENDER_VERTEX_ARRAY_GEN(VBO,VAO) \
	glGenBuffers(1, &VBO); \
	glGenVertexArrays(1, &VAO)

#define RENDER_VERTEX_ARRAY_DELETE(VBO,VAO) \
	glDeleteBuffers(1,&VBO);\
	glDeleteVertexArrays(1, &VAO)

#define RENDER_VERTEX_ARRAY_BIND(VBO,VAO) \
	glBindVertexArray(VAO); \
	glBindBuffer(GL_ARRAY_BUFFER, VBO)

#define RENDER_VERTEX_ARRAY_UNBIND \
	glBindBuffer(GL_ARRAY_BUFFER, 0);\
	glBindVertexArray(0)

#define RENDER_VERTEX_ARRAY_SCOPE(VBO,VAO,CODE) \
	RENDER_VERTEX_ARRAY_BIND(VBO,VAO);\
		CODE 		\
	RENDER_VERTEX_ARRAY_UNBIND

#define C_O_D_E(CODE) \
		CODE



void DrawLine(Vec2 v1,Vec2 v2,Vec4 color);
void DrawRect(int x,int y,int width,int height, Vec4 color,bool isFill = false);
void DrawCircle(int x, int y, int radius, Vec4 color, bool isFill = false);

class Line
{
	Vec2 v1;
	Vec2 v2;
	Vec3 color;
	GLuint VAO;
	GLuint VBO;
	void Begin()
	{
		RENDER_VERTEX_ARRAY_GEN(VBO, VAO);
		GLfloat lineVertices[4];
		lineVertices[0] = v1.x;
		lineVertices[1] = v1.y;
		lineVertices[2] = v2.x;
		lineVertices[3] = v2.y;
		RENDER_VERTEX_ARRAY_SCOPE(VBO, VAO, C_O_D_E(
			glBufferData(GL_ARRAY_BUFFER, sizeof(lineVertices), lineVertices, GL_STATIC_DRAW);
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), 0);
            glEnableVertexAttribArray(0);
		));
	}
	
	void End()
	{	
		RENDER_VERTEX_ARRAY_DELETE(VAO,VBO);
	}

	void Draw(Shader& shader)
	{	
		RENDER_VERTEX_ARRAY_BIND(VAO, VBO);
			shader.Bind();
			glDrawArrays(GL_LINES, 0, 2);
			shader.Unbind();
		RENDER_VERTEX_ARRAY_UNBIND;	
	};
};
class RenderObject
{
	RenderType type;
	void DrawSelf();
};
class Renderer2D
{
public:
	
	Renderer2D();
	~Renderer2D();
	
	void Render(RenderInfo info); 
	void AddObject(RenderObject obj);
	void RemoveObject(RenderObject obj);
private:
	glm::mat4 m_OrthoMat;//= glm::ortho(0.0f, windowWidth*1.0f, windowHeight*1.0f, 0.0f, -1.0f, 1.0f);
	std::vector<RenderObject> m_ObjectList;
	GLuint m_VBO, m_VAO;
	Shader m_Shader;
};


class RenderThread
{
public:
	RenderThread();
	~RenderThread();

private:

};


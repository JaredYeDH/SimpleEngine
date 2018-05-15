#pragma once
#include "defines.h"
#include "defineTypes.h"

#include "Shader.h"
#include "Texture.h"
#include "Environment.h"
#include "Window.h"
#include "core/Transform.h"
#include "Object2D.h"
#include "Graphics2D.h"
#include "../Singleton.h"

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

class Renderer2D : public Singleton<Renderer2D>
{
public:
	
	Renderer2D();
	~Renderer2D();
	
	void Render(); 
	void AddObject(Object2D* obj);
	void RemoveObject(int index);


private:
	std::list<Object2D*> m_ObjectList;
	Graphics2D m_Graphics;
};
#define RENDERER_2D_INSTANCE Renderer2D::GetInstance()

class RenderThread
{
public:
	RenderThread();
	~RenderThread();

private:

};


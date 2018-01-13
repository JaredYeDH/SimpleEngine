#include "AlphaSence.h"
#include "Texture.h"
#include "Shader.h"
#include "Environment.h"
#include "defineTypes.h"
#include "Window.h"

Shader* shaderPtr;

const GLchar* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec2 position;\n"
"uniform mat4 projection;\n"
"void main()\n"
"{\n"
"gl_Position = projection * vec4(position.x, position.y, 0.0, 1.0);\n"
"}\0";

const GLchar* fragmentShaderSource = "#version 330 core\n"
"out vec4 color;\n"
"uniform vec4 lineColor;\n"
"void main()\n"
"{\n"
"color = lineColor;\n"
"}\n\0";

void DrawCircle(int x, int y, int radius, Vec4 color, bool isFill );

void DrawTexture(int x, int y, int radius, Vec4 color, bool isFill);

AlphaSence::AlphaSence()
{
	//TestTimer
	//TimerManager::GetInstance()->CreateTimer("Deda2", 1000, true, false, [this](){
	//	printf("void AlphaSence::Update(4000)\n");
	//	TimerManager::GetInstance()->CreateTimer("Deda3", 16, true, true,
	//		[](){DrawCircle(250, 150, 100, Vec4(0, 1, 0, 1), false);}	//这个园的显示会出现闪烁，原因是主线程的glClearColor刷新了窗口导致，要解决这个问题，需要把drawcall放在主线程提交
	//	);
	//});
}

AlphaSence::~AlphaSence()
{
	
}

void AlphaSence::Update() 
{

}

/************************************************************************/
/* 画线步骤：
1. 准备定点
2. 设置shader vao vbo
3. glDraw
*/
/************************************************************************/
void DrawLine(Vec2 v1,Vec2 v2,Vec4 color)
{
	std::string  vPath, fPath;
	vPath = Environment::GetShaderPath("LineShader.vs");
	fPath = Environment::GetShaderPath("LineShader.fg");
	Shader* shaderPtr = new Shader(vPath,fPath);

	int windowWidth = Window::GetWidth();
	int windowHeight = Window::GetHeight();
	glm::mat4 projection = glm::ortho(0.0f, windowWidth*1.0f, windowHeight*1.0f, 0.0f, -1.0f, 1.0f);
	shaderPtr->Bind();
	glUniformMatrix4fv(glGetUniformLocation(shaderPtr->GetProgramID(), "projection"), 1, GL_FALSE, (GLfloat*)(&projection));
	glUniform4fv(glGetUniformLocation(shaderPtr->GetProgramID(), "lineColor"), 1,(GLfloat*)(&color));
	shaderPtr->Unbind();

	GLfloat lineVertices[4];
	lineVertices[0] = v1.x;
	lineVertices[1] = v1.y;
	lineVertices[2] = v2.x;
	lineVertices[3] = v2.y;

	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(lineVertices), lineVertices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), 0);
		glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0); 
	glBindVertexArray(0);
	
	// Draw our first triangle
	shaderPtr->Bind();
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glDrawArrays(GL_LINES, 0, 2);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	shaderPtr->Unbind();
}

//#define GL_POINTS                         0x0000
//#define GL_LINES                          0x0001
//#define GL_LINE_LOOP                      0x0002
//#define GL_LINE_STRIP                     0x0003
//#define GL_TRIANGLES                      0x0004
//#define GL_TRIANGLE_STRIP                 0x0005
//#define GL_TRIANGLE_FAN                   0x0006
//#define GL_QUADS                          0x0007
//#define GL_QUAD_STRIP                     0x0008
//#define GL_POLYGON                        0x0009

void DrawRect(int x,int y,int width,int height, Vec4 color,bool isFill = false)
{
	std::string  vPath, fPath;
	vPath = Environment::GetShaderPath("LineShader.vs");
	fPath = Environment::GetShaderPath("LineShader.fg");
	Shader* shaderPtr = new Shader(vPath, fPath);

	int windowWidth = Window::GetWidth();
	int windowHeight = Window::GetHeight();
	glm::mat4 projection = glm::ortho(0.0f, windowWidth*1.0f, windowHeight*1.0f, 0.0f, -1.0f, 1.0f);
	shaderPtr->Bind();
	glUniformMatrix4fv(glGetUniformLocation(shaderPtr->GetProgramID(), "projection"), 1, GL_FALSE, (GLfloat*)(&projection));
	glUniform4fv(glGetUniformLocation(shaderPtr->GetProgramID(), "lineColor"), 1, (GLfloat*)(&color));
	shaderPtr->Unbind();

	GLfloat lineVertices[10];
	lineVertices[0] = x;
	lineVertices[1] = y;
	lineVertices[2] = x+width;
	lineVertices[3] = y;
	lineVertices[4] = x+width;
	lineVertices[5] = y+height;
	lineVertices[6] = x;
	lineVertices[7] = y+height;
	lineVertices[8] = x;
	lineVertices[9] = y;
	

	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(lineVertices), lineVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// Draw our first triangle
	shaderPtr->Bind();
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glDrawArrays(GL_LINE_LOOP, 0, 5);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	shaderPtr->Unbind();
}

void DrawCircle(int x, int y, int radius, Vec4 color, bool isFill = false)
{
	std::string  vPath, fPath;
	vPath = Environment::GetShaderPath("LineShader.vs");
	fPath = Environment::GetShaderPath("LineShader.fg");
	Shader* shaderPtr = new Shader(vPath, fPath);

	int windowWidth = Window::GetWidth();
	int windowHeight = Window::GetHeight();
	glm::mat4 projection = glm::ortho(0.0f, windowWidth*1.0f, windowHeight*1.0f, 0.0f, -1.0f, 1.0f);
	shaderPtr->Bind();
	glUniformMatrix4fv(glGetUniformLocation(shaderPtr->GetProgramID(), "projection"), 1, GL_FALSE, (GLfloat*)(&projection));
	glUniform4fv(glGetUniformLocation(shaderPtr->GetProgramID(), "lineColor"), 1, (GLfloat*)(&color));
	shaderPtr->Unbind();

	int vecSize = 36+1;
	int doubleSize = vecSize * 2;
	GLfloat lineVertices[(36 + 1) * 2];
	GLfloat doublePi = 2.0f * glm::pi<float>();
	for (int i = 0; i < vecSize; i++)
	{
		lineVertices[i * 2] = x + (radius * glm::cos(i *doublePi / (vecSize-1)));
		lineVertices[i * 2 + 1] = y + (radius * glm::sin(i *doublePi / (vecSize-1)));
	}


	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(lineVertices), lineVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// Draw our first triangle
	shaderPtr->Bind();
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glDrawArrays(GL_LINE_LOOP, 0, vecSize);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	shaderPtr->Unbind();
}

void AlphaSence::Draw() 
{
	//DrawLine(Vec2(50, 150), Vec2(200, 150), Vec4(1,0,0,1));

	DrawRect(50,50,100,50, Vec4(0, 1, 0, 1));

	DrawRect(250, 150, 100, 50, Vec4(0, 1, 0, 1));
	
	DrawCircle(200, 200, 50, Vec4(0, 1, 0, 1));

	//DrawLine(Vec2(0, 100), Vec2(200, 50), Vec4(1, 0, 0, 1));
	//DrawLine(Vec3(50, 0, 0), Vec3(50, 200, 0), Vec3(), 5);
}


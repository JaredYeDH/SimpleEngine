#pragma once

#include "defines.h"
#include "Singleton.h"
#include "core/Shader.h"
#include <array>

struct CubeInfo
{
	glm::vec4 FillColor;
	glm::mat4 View;
	glm::mat4 Model;		
	glm::mat4 Projection;
	GLuint TextureID;
};

class CubeBatch final : public Singleton<CubeBatch>
{
public:
	friend Singleton<CubeBatch>;

	void Init();
	void Flush();
	void AddCube(const CubeInfo* cubeInfo);


private:
	CubeBatch();
	~CubeBatch();

	void Begin();
	void End();

	static const uint32 BATCHSIZE = 50;
	static const uint32 VERTEX_AMOUNT = 18;
	static const uint32 UV_AMOUNT = 12;
	static const uint32 FIRST_REAL_ASCII_CHAR = 31;

	std::vector<const CubeInfo*> m_CubeQueue;
	std::vector<GLfloat> mVertices;

	GLuint VAO,VBO;
	
	Shader* m_pShader;


	GLint modelLoc,viewLoc, projLoc;

	std::array<GLfloat, 180> vertices3D;

};

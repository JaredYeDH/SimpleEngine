#include "CubeBatch.h"
#include "Environment.h"




CubeBatch::CubeBatch()
	:Singleton<CubeBatch>()
	, m_CubeQueue()
	, m_pShader(nullptr)
	, vertices3D({
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

		-0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

		0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f
	})
{

}

CubeBatch::~CubeBatch()
{
	delete m_pShader;
}

void CubeBatch::Init()
{
	std::string vPath =  Environment::GetAbsPath("Shader/CubeVertexShader");
	std::string fPath =  Environment::GetAbsPath("Shader/CubeFragmentShader");

	m_pShader = new Shader(vPath,fPath);
	m_pShader->Bind();
  
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	
	m_pShader->Unbind();
}

void CubeBatch::Flush()
{
	m_pShader->Bind();

	Begin();
	End();
	m_pShader->Unbind();
	m_CubeQueue.clear();
}

void CubeBatch::AddCube(const CubeInfo* cubeInfo)
{
	m_CubeQueue.push_back(cubeInfo);
}

void CubeBatch::Begin()
{
	// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices3D),&vertices3D[0], GL_STATIC_DRAW);

    // Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
    // TexCoord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0); // Unbind VAO
}

void CubeBatch::End()
{
	glBindVertexArray(VAO);
	for(const CubeInfo* info : m_CubeQueue)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, info->TextureID);
		glUniform1i(glGetUniformLocation(m_pShader->GetProgramID(), "textureSampler"), 0);
		//glUniform4fv(glGetUniformLocation(m_pShader->GetProgramID(), "solidColor"), 1,  (GLfloat*)(&info->FillColor));
		
		// Get the uniform locations
		modelLoc = glGetUniformLocation(m_pShader->GetProgramID(), "model");
		viewLoc = glGetUniformLocation(m_pShader->GetProgramID(), "view");
		projLoc = glGetUniformLocation(m_pShader->GetProgramID(), "projection");
		
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE,  (GLfloat*)(&info->Model));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, (GLfloat*) (&info->View));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, (GLfloat*)(&info->Projection));

		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
	glBindVertexArray(0);	
}











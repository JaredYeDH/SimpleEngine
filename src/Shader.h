#pragma once

#include "defines.h"

/*
封装OpenGL内部的Shader
暴露出一些方法用来控制OpenGL内部状态
*/
class Shader
{
public:
	Shader();
	Shader(const std::string vertPath,const std::string fragPath,const std::string geomPath = "");
	~Shader();

	void Init(const std::string vert,const std::string frag,const std::string geom = "");

	void Bind();

	void Unbind();

	const GLuint GetProgramID() const;
	

private:
	 void checkCompileErrors(GLuint shader, std::string type);

	GLuint m_ProgramID;
		
	GLuint m_VertexShader;
	GLuint m_FragmentShader;
	GLuint m_GeometryShader;

}; 
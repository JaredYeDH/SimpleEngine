#pragma once
//GLEW
#include <GL/glew.h>
//GLFW
#include <GLFW/glfw3.h>

#include "core/Shader.h"
#include "core/Texture.h"
#include <map>

class TextRenderer
{
	/// Holds all state information relevant to a character as loaded using FreeType
	struct Character {
		GLuint TextureID;   // ID handle of the glyph texture
		glm::ivec2 Size;    // Size of glyph
		glm::ivec2 Bearing;  // Offset from baseline to left/top of glyph
		GLuint Advance;    // Horizontal offset to advance to next glyph
	};

public:
	TextRenderer();
	~TextRenderer();
	static TextRenderer* GetInstance() 
	{
		static TextRenderer* instance = new TextRenderer();
		return instance;
	}
	void RenderText(std::wstring text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color);
	void RenderPlotText(std::wstring text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color);
	void Draw(std::wstring text,GLfloat x, GLfloat y,glm::vec3 color);
private:

	std::map<wchar_t, Character> Characters;
	GLuint VAO, VBO;
	Shader* shader;
};
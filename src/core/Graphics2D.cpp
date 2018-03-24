#include "Graphics2D.h"
#include "Object2D.h"
#include "Renderer.h"
#include "../Environment.h"
#include "../Window.h"
#include "../TextureManager.h"
Graphics2D::Graphics2D()
:m_SpriteVertices({
        // Pos      // Tex
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f,
        
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f
})
{
    String  vPath, fPath;
    vPath = Environment::GetShaderPath("LineShader.vs");
    fPath = Environment::GetShaderPath("LineShader.fs");
    m_ShaderPtr = new Shader(vPath, fPath);

    int windowWidth = Window::GetWidth();
    int windowHeight = Window::GetHeight();
    
    m_OrthoMat = glm::ortho(0.0f, windowWidth*1.0f, windowHeight*1.0f, 0.0f, -1.0f, 1.0f);
    m_ShaderPtr->Bind();
    glUniformMatrix4fv(glGetUniformLocation(m_ShaderPtr->GetProgramID(), "orth"), 1, GL_FALSE, (GLfloat*)(&m_OrthoMat));
    m_ShaderPtr->Unbind();

    
    vPath = Environment::GetShaderPath("sprite.vs");
    fPath = Environment::GetShaderPath("sprite.fs");

    m_SpriteShaderPtr = new Shader(vPath,fPath);

	m_SpriteShaderPtr->Bind();
	glUniform1i(glGetUniformLocation(m_SpriteShaderPtr->GetProgramID(), "image"), 0);
	glUniformMatrix4fv(glGetUniformLocation(m_SpriteShaderPtr->GetProgramID(), "projection"), 1, GL_FALSE, (GLfloat*)(&m_OrthoMat));
    m_SpriteShaderPtr->Unbind();

    RENDER_VERTEX_ARRAY_GEN(m_SpriteVBO, m_SpriteVAO);

    RENDER_VERTEX_ARRAY_SCOPE(m_SpriteVBO, m_SpriteVAO, 
    {
        glBufferData(GL_ARRAY_BUFFER, sizeof(m_SpriteVertices), &m_SpriteVertices[0], GL_STATIC_DRAW);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
    });
    
}

Graphics2D::~Graphics2D()
{

}

void Graphics2D::Begin() 
{
   m_ShaderPtr->Bind();
}

void Graphics2D::Draw(Object2D& obj) 
{
   obj.Draw(*this);
}

void Graphics2D::End() 
{
   m_ShaderPtr->Unbind();
}

void Graphics2D::DrawLine(const Line& line)  
{
    GLfloat vertices[4];
    vertices[0] = line.v1.x;
    vertices[1] = line.v1.y;
    vertices[2] = line.v2.x;
    vertices[3] = line.v2.y;

    const Vec4 color = line.Color();
    glUniform4fv(glGetUniformLocation(m_ShaderPtr->GetProgramID(), "color_in"), 1, (GLfloat*)(&color));

    GLuint VBO,VAO;
    RENDER_VERTEX_ARRAY_GEN(VBO, VAO);
    RENDER_VERTEX_ARRAY_SCOPE(VBO, VAO, 
    {
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), 0);
        glEnableVertexAttribArray(0);
        glDrawArrays(GL_LINES, 0, 2);
    });
    RENDER_VERTEX_ARRAY_DELETE(VBO,VAO);
};

void Graphics2D::DrawRect(const Rect& rect) 
{
    int x = rect.T().pos.x;
    int y = rect.T().pos.y;
    int width = rect.T().size.x;
    int height = rect.T().size.y;

    GLfloat vertices[10];
    vertices[0] = x;
    vertices[1] = y;
    vertices[2] = x+width;
    vertices[3] = y;
    vertices[4] = x+width;
    vertices[5] = y+height;
    vertices[6] = x;
    vertices[7] = y+height;
    vertices[8] = x;
    vertices[9] = y;

    const Vec4 color = rect.Color();
    glUniform4fv(glGetUniformLocation(m_ShaderPtr->GetProgramID(), "color_in"), 1, (GLfloat*)(&color));

    GLuint VBO,VAO;
    RENDER_VERTEX_ARRAY_GEN(VBO, VAO);
    RENDER_VERTEX_ARRAY_SCOPE(VBO, VAO, 
    {
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), 0);
        glEnableVertexAttribArray(0);
        glDrawArrays(GL_LINE_LOOP, 0, 5);
    });
    RENDER_VERTEX_ARRAY_DELETE(VBO,VAO);

}


void Graphics2D::DrawCircle(const Circle& circle ) 
{
    int radius = circle.raduis;
    int x = circle.T().center.x;
    int y = circle.T().center.y;
    int vecSize = 36+1;
    int doubleSize = vecSize * 2;
    GLfloat vertices[(36 + 1) * 2];
    GLfloat doublePi = 2.0f * glm::pi<float>();
    for (int i = 0; i < vecSize; i++)
    {
        vertices[i * 2] = x + (radius * glm::cos(i *doublePi / (vecSize-1)));
        vertices[i * 2 + 1] = y + (radius * glm::sin(i *doublePi / (vecSize-1)));
    }

    const Vec4 color = circle.Color();
    glUniform4fv(glGetUniformLocation(m_ShaderPtr->GetProgramID(), "color_in"), 1, (GLfloat*)(&color));

    GLuint VBO,VAO;
    RENDER_VERTEX_ARRAY_GEN(VBO, VAO);
    RENDER_VERTEX_ARRAY_SCOPE(VBO, VAO, 
    {
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), 0);
        glEnableVertexAttribArray(0);
        glDrawArrays(GL_LINE_LOOP, 0,vecSize );
    });
    RENDER_VERTEX_ARRAY_DELETE(VBO,VAO);

}

void Graphics2D::DrawImage(const Image& image) 
{
    m_SpriteShaderPtr->Bind();
      //  glEnable(GL_BLEND);
	  //  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        Vec3 pos = image.T().pos;
        Vec2 size = image.T().size;
        Vec3 rotation = image.T().rotation;
        Vec4 color = image.Color();
        
        Mat4 model;
        model = glm::translate(model, pos);
        model = glm::translate(model,  glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f)); 
        model = glm::rotate(model, rotation.z ,glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));
        model = glm::scale(model, glm::vec3(size, 1.0f)); 
        
        const Texture* texture = TextureManager::GetInstance()->GetTexture(image.path);
        glUniformMatrix4fv(glGetUniformLocation(m_SpriteShaderPtr->GetProgramID(), "model"), 1, GL_FALSE, (GLfloat*) (&model));
        glUniform3f(glGetUniformLocation(m_SpriteShaderPtr->GetProgramID(), "spriteColor"), 1.0, 1.0, 1.0);
        
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,  texture->GetTextureID());

        RENDER_VERTEX_ARRAY_SCOPE(m_SpriteVBO, m_SpriteVAO,
        {
            

            glDrawArrays(GL_TRIANGLES, 0, 6);
        });
        //glDisable(GL_BLEND);
    m_SpriteShaderPtr->Unbind();
   // m_ShaderPtr->Bind();
}


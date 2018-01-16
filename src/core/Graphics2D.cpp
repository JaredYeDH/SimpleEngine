#include "Graphics2D.h"
#include "Object2D.h"
#include "Renderer.h"

Graphics2D::Graphics2D()
{

}

Graphics2D::~Graphics2D()
{

}

void Graphics2D::Draw( Object2D& obj) 
{
   obj.Draw(*this);
}

void Graphics2D::DrawLine(const Line& line)  
{
    GLfloat lineVertices[4];
    lineVertices[0] = line.v1.x;
    lineVertices[1] = line.v1.y;
    lineVertices[2] = line.v2.x;
    lineVertices[3] = line.v2.y;

    GLuint VAO;
    GLuint VBO;
    RENDER_VERTEX_ARRAY_GEN(VBO, VAO);
    RENDER_VERTEX_ARRAY_SCOPE(VBO, VAO, 
    {
        glBufferData(GL_ARRAY_BUFFER, sizeof(lineVertices), lineVertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), 0);
        glEnableVertexAttribArray(0);
    });

    RENDER_VERTEX_ARRAY_BIND(VAO, VBO);
        m_Shader.Bind();
        glDrawArrays(GL_LINES, 0, 2);
        m_Shader.Unbind();
    RENDER_VERTEX_ARRAY_UNBIND;	
    RENDER_VERTEX_ARRAY_DELETE(VAO,VBO);
};




void Graphics2D::DrawRect(const Rect& rect) 
{
}
//     std::string  vPath, fPath;
//     vPath = Environment::GetShaderPath("LineShader.vs");
//     fPath = Environment::GetShaderPath("LineShader.fg");
//     Shader* shaderPtr = new Shader(vPath, fPath);

//     int windowWidth = Window::GetWidth();
//     int windowHeight = Window::GetHeight();
//     glm::mat4 projection = glm::ortho(0.0f, windowWidth*1.0f, windowHeight*1.0f, 0.0f, -1.0f, 1.0f);
//     shaderPtr->Bind();
//     glUniformMatrix4fv(glGetUniformLocation(shaderPtr->GetProgramID(), "projection"), 1, GL_FALSE, (GLfloat*)(&projection));
//     glUniform4fv(glGetUniformLocation(shaderPtr->GetProgramID(), "lineColor"), 1, (GLfloat*)(&color));
//     shaderPtr->Unbind();

//     GLfloat lineVertices[10];
//     lineVertices[0] = x;
//     lineVertices[1] = y;
//     lineVertices[2] = x+width;
//     lineVertices[3] = y;
//     lineVertices[4] = x+width;
//     lineVertices[5] = y+height;
//     lineVertices[6] = x;
//     lineVertices[7] = y+height;
//     lineVertices[8] = x;
//     lineVertices[9] = y;


//     GLuint VBO, VAO;
//     RENDER_VERTEX_ARRAY_GEN(VBO, VAO);

//     RENDER_VERTEX_ARRAY_SCOPE(VBO, VAO, 
//     {
//         glBufferData(GL_ARRAY_BUFFER, sizeof(lineVertices), lineVertices, GL_STATIC_DRAW);
//         glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), 0);
//         glEnableVertexAttribArray(0);                                            
//     });
       

//     RENDER_VERTEX_ARRAY_SCOPE(VBO, VAO, 
//     {
//         shaderPtr->Bind();
//         glDrawArrays(GL_LINE_LOOP, 0, 5);
//         shaderPtr->Unbind();
//     });

//     RENDER_VERTEX_ARRAY_DELETE(VBO, VAO);

// }

void Graphics2D::DrawCircle(const Circle& c ) 
{
}

void Graphics2D::DrawImage(const Image& img) 
{

}
//     std::string  vPath, fPath;
//     vPath = Environment::GetShaderPath("LineShader.vs");
//     fPath = Environment::GetShaderPath("LineShader.fg");
//     Shader* shaderPtr = new Shader(vPath, fPath);

//     int windowWidth = Window::GetWidth();
//     int windowHeight = Window::GetHeight();
//     glm::mat4 projection = glm::ortho(0.0f, windowWidth*1.0f, windowHeight*1.0f, 0.0f, -1.0f, 1.0f);
//     shaderPtr->Bind();
//     glUniformMatrix4fv(glGetUniformLocation(shaderPtr->GetProgramID(), "projection"), 1, GL_FALSE, (GLfloat*)(&projection));
//     glUniform4fv(glGetUniformLocation(shaderPtr->GetProgramID(), "lineColor"), 1, (GLfloat*)(&color));
//     shaderPtr->Unbind();

//     int vecSize = 36+1;
//     int doubleSize = vecSize * 2;
//     GLfloat lineVertices[(36 + 1) * 2];
//     GLfloat doublePi = 2.0f * glm::pi<float>();
//     for (int i = 0; i < vecSize; i++)
//     {
//         lineVertices[i * 2] = x + (radius * glm::cos(i *doublePi / (vecSize-1)));
//         lineVertices[i * 2 + 1] = y + (radius * glm::sin(i *doublePi / (vecSize-1)));
//     }


//     GLuint VBO, VAO;
//     RENDER_VERTEX_ARRAY_GEN(VBO, VAO);

//     RENDER_VERTEX_ARRAY_SCOPE(VBO, VAO,
//     {
//         glBufferData(GL_ARRAY_BUFFER, sizeof(lineVertices), lineVertices, GL_STATIC_DRAW);
//         glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), 0);
//         glEnableVertexAttribArray(0);
//     });

//     RENDER_VERTEX_ARRAY_SCOPE(VBO, VAO, {
//         shaderPtr->Bind();
//         glDrawArrays(GL_LINE_LOOP, 0, vecSize);
//         shaderPtr->Unbind();
//     });
       
    
//     RENDER_VERTEX_ARRAY_DELETE(VBO, VAO);
    
    
// }

/*******************************************************************
 ** This code is part of Breakout.
 **
 ** Breakout is free software: you can redistribute it and/or modify
 ** it under the terms of the CC BY 4.0 license as published by
 ** Creative Commons, either version 4 of the License, or (at your
 ** option) any later version.
 ******************************************************************/
#pragma once

#include "defines.h"
#include "core/Texture.h"
#include "core/Shader.h"



class SpriteRenderer
{
public:
    // Constructor (inits shaders/shapes)
    SpriteRenderer();
    // Destructor
    ~SpriteRenderer();
    static SpriteRenderer* GetInstance()
    {
        static auto* instance = new SpriteRenderer();
        return instance;
    }
    // Renders a defined quad textured with given sprite
    void DrawSprite(Texture* texture, glm::vec2 position, glm::vec2 size = glm::vec2(10, 10), GLfloat rotate = 0.0f, glm::vec3 color = glm::vec3(1.0f));
    void DrawSprite( glm::vec2 position, glm::vec2 size = glm::vec2(10, 10), GLfloat rotate = 0.0f, glm::vec3 color = glm::vec3(1.0f));
    void DrawMask(Texture* texture, glm::vec2 position, glm::vec2 size = glm::vec2(10, 10), GLfloat rotate = 0.0f, glm::vec3 color = glm::vec3(1.0f));
    void DrawFrameSprite(Texture* texture, glm::vec2 position, glm::vec2 size = glm::vec2(10, 10), GLfloat rotate = 0.0f, glm::vec3 color = glm::vec3(1.0f));
    void DrawMapSprite(Texture* texture, glm::vec2 position, glm::vec2 size = glm::vec2(10, 10), GLfloat rotate = 0.0f, GLfloat alpha=0.0f);

private:
    // Render state
    Shader* m_pShader;
    GLuint quadVAO;
    // Initializes and configures the quad's buffer and vertex attributes
    void initRenderData();
};

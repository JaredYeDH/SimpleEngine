/*******************************************************************
 ** This code is part of Breakout.
 **
 ** Breakout is free software: you can redistribute it and/or modify
 ** it under the terms of the CC BY 4.0 license as published by
 ** Creative Commons, either version 4 of the License, or (at your
 ** option) any later version.
 ******************************************************************/
#include "ResourceManager.h"
#include <SOIL/SOIL.h>
#include "image.h"
#include "Projects/Config.h"
#include "Environment.h"

ResourceManager::ResourceManager()
	:Singleton<ResourceManager>()
{
	Config config(Environment::GetAbsPath("Resource/tables/config.txt"));

	m_ShapeWdfPtr = new NetEase::WDF(config.GetWdfPath("shape.wdf"));
	m_ShapeWd3Ptr = new NetEase::WDF(config.GetWdfPath("shape.wd3"));

	std::string vPath = Environment::GetAbsPath("Shader/sprite.vs");
	std::string fPath = Environment::GetAbsPath("Shader/sprite.fs");
	std::cout << " vPath:" << vPath << std::endl;
	std::cout << " fPath:" << fPath << std::endl;

	LoadShader(vPath, fPath, "", "sprite");
}
ResourceManager::~ResourceManager()
{

}


void ResourceManager::LoadShader(const std::string vShaderFile, const std::string fShaderFile, const std::string gShaderFile, std::string name)
{
    Shaders[name] =new Shader(vShaderFile,fShaderFile);
    std::cout << "LoadShader" << std::endl;
    // return Shaders[name];
}

Shader* ResourceManager::GetShader(std::string name)
{
    return Shaders[name];
}

Texture* ResourceManager::LoadTexture(const uint8 *src, bool alpha, std::string name)
{
    Textures[name] =new Texture( 320,240,alpha,(uint8*)src);
    return Textures[name];
}

Texture* ResourceManager::GetTexture(std::string name)
{
    return Textures[name];
}

void ResourceManager::Clear()
{
    // (Properly) delete all shaders
    // for (auto iter : Shaders)
        // glDeleteProgram(iter.second.GetProgramID());
    // (Properly) delete all textures
    // for (auto iter : Textures)
        // glDeleteTextures(1, &(iter.second.GetTextureID()));
}

Sprite2 ResourceManager::LoadWdfSprite(int wasId)
{

	return m_ShapeWdfPtr->LoadSprite(wasId);
}

void ResourceManager::SaveWdfSprite(int wasId)
{
    m_ShapeWdfPtr->LoadSprite(wasId).SaveImage(0);
}

Sprite2 ResourceManager::LoadWd3Sprite(int wasId)
{
	return m_ShapeWd3Ptr->LoadSprite(wasId);
}



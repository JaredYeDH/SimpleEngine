/*******************************************************************
 ** This code is part of Breakout.
 **
 ** Breakout is free software: you can redistribute it and/or modify
 ** it under the terms of the CC BY 4.0 license as published by
 ** Creative Commons, either version 4 of the License, or (at your
 ** option) any later version.
 ******************************************************************/
#include "ResourceManager.h"
#include "image.h"
#include "Environment.h"

ResourceManager::ResourceManager()
	:Singleton<ResourceManager>()
{

	m_ShapeWdfPtr = new NE::WDF(Environment::GetWDFPath("shape.wdf"));
	m_ShapeWd3Ptr = new NE::WDF(Environment::GetWDFPath("shape.wd3"));

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
    Shaders[name] = new Shader(vShaderFile,fShaderFile);
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

Sprite* ResourceManager::LoadWdfSprite(uint32 wasID)
{
	return m_ShapeWdfPtr->LoadSprite(wasID);
}

static std::vector<String> s_PackToName = {"addon.wdf","atom.wdf","chat.wdf","firework.wdf","goods.wdf","item.wdf","magic.wdf","mapani.wdf","mhimage.wdf","misc.wdf","music.wdf","scene.wdf","shape.wd1","shape.wd2","shape.wd3","shape.wd4","shape.wd5","shape.wd6","shape.wd7","shape.wdf","smap.wdf","sound.wdf","stock.wdf","waddon.wdf","wzife.wd1","wzife.wdf","wzimage.wdf"};
static std::map<int,NE::WDF*> s_Loaders;
Sprite* ResourceManager::LoadWASSprite(int pack, uint32 wasId)
{
    if(s_Loaders.find(pack) ==s_Loaders.end()  )
    {
        s_Loaders[pack] = new NE::WDF(Environment::GetWDFPath(s_PackToName[pack]));
    }
    return s_Loaders[pack]->LoadSprite(wasId);
}


void ResourceManager::SaveWdfSprite(uint32 wasId)
{
    m_ShapeWdfPtr->LoadSprite(wasId)->SaveImage(0);
}

Sprite* ResourceManager::LoadWd3Sprite(uint32 wasID)
{
	return m_ShapeWd3Ptr->LoadSprite(wasID);
}



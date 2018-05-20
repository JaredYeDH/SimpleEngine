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
#include "Singleton.h"
#include "core/Texture.h"
#include "core/Shader.h"
#include <Sprite2.h>
#include <WDF.h>
#include <WAS.h>

// A static singleton ResourceManager class that hosts several
// functions to load Textures and Shaders. Each loaded texture
// and/or shader is also stored for future reference by string
// handles. All functions and resources are static and no
// public constructor is defined.
class ResourceManager final : public Singleton<ResourceManager>
{
public:
	enum PackFile
	{
		ADDONWDF,ATOMWDF,CHATWDF,FIREWORKWDF,GOODSWDF,ITEMWDF,MAGICWDF,MAPANIWDF,MHIMAGEWDF,MISCWDF,MUSICWDF,SCENEWDF,SHAPEWD1,SHAPEWD2,SHAPEWD3,SHAPEWD4,SHAPEWD5,SHAPEWD6,SHAPEWD7,SHAPEWDF,SMAPWDF,SOUNDWDF,STOCKWDF,WADDONWDF,WZIFEWD1,WZIFEWDF,WZIMAGEWDF
	};

	friend Singleton<ResourceManager>;

	std::shared_ptr<Sprite2> LoadWdfSprite(uint32 wasId);
	std::shared_ptr<Sprite2> LoadMagicSprite(uint32 wasId);
	std::shared_ptr<Sprite2> LoadWd3Sprite(uint32 wasId);
	std::shared_ptr<Sprite2> LoadWASSprite(int pack,uint32 wasId);

	void SaveWdfSprite(uint32 wasId);
    // Loads (and generates) a shader program from file loading vertex, fragment (and geometry) shader's source code. If gShaderFile is not nullptr, it also loads a geometry shader
    void LoadShader(const std::string vShaderFile, const std::string fShaderFile, const std::string gShaderFile, std::string name);
    // Retrieves a stored sader
    Shader*   GetShader(std::string name);
    // Loads (and generates) a texture from file
    Texture* LoadTexture(const uint8 *src, bool alpha, std::string name);
    // Retrieves a stored texture
    Texture* GetTexture(std::string name);
    // Properly de-allocates all loaded resources
    void      Clear();

private:
    // Private constructor, that is we do not want any actual resource manager objects. Its members and functions should be publicly available (static).
	ResourceManager();
	~ResourceManager();

	std::map<uint32,std::shared_ptr<Sprite2>> m_WDFSpriteCache;
	std::map<uint32,std::shared_ptr<Sprite2>> m_WD3SpriteCache;
	
	NetEase::WDF* m_ShapeWdfPtr;
	NetEase::WDF* m_ShapeWd3Ptr;
	
	// Resource storage
	std::map<std::string, Shader*>  Shaders;
	std::map<std::string, Texture*> Textures;

};
#define  RESOURCE_MANAGER_INSTANCE ResourceManager::GetInstance()
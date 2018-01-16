#pragma once
#include "defines.h"
#include "defineTypes.h"
#include "Singleton.h"
#include "core/Texture.h"
#include <unordered_map>
class TextureManager final : public Singleton<TextureManager>
{
public:

using Map=std::unordered_map<String,Texture*>;

    TextureManager();
    ~TextureManager();
    void LoadTexture(String path);
    void UnloadTexture(String path);
    Texture* GetTexture(String path) ;

private:
    
    Map m_Textures;

};

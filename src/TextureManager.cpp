#include "TextureManager.h"

TextureManager::TextureManager()
{

}

TextureManager::~TextureManager()
{

}

void TextureManager::LoadTexture(String path)
{
    if(m_Textures.find(path) == m_Textures.end())
    {

        m_Textures[path]=new Texture(path);
    }
    else
    {
        return;
    }
}

void TextureManager::UnloadTexture(String path)
{
    if(m_Textures.find(path) == m_Textures.end())
    {
        return;
    }
    else
    {
        delete m_Textures[path];
        m_Textures[path]=nullptr;
        m_Textures.erase(path);
        
    }
}

 Texture*  TextureManager::GetTexture(String path)
{
    if(m_Textures.find(path) != m_Textures.end())
    {
        return m_Textures[path];
    }
    else
    {
        return nullptr;
    }
}


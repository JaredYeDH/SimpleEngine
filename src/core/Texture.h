#pragma once

#include "defines.h"


class Texture
{
public:
	Texture();
	Texture(std::string filePath);
	Texture(int width,int height,bool alpha,uint8 *lImageBuffer);
	Texture(const Texture& t) ;



	Texture& operator=(const Texture& t)
	{
		this->mTextureId = t.mTextureId ;
		this->mFormat = t.mFormat ;
		this->mWidth= t.mWidth ;
		this->mHeight= t.mHeight ;
		this->mPath= t.mPath ;
		return *this;
	}
	
	~Texture();
	
	int32 GetWidth() const {return mWidth;}
	int32 GetHeight() const {return mHeight;}
	GLuint GetTextureID() const {return mTextureId;}

private:
	GLuint mTextureId;
	GLint mFormat;
	int32 mWidth,mHeight;
	std::string mPath;

};

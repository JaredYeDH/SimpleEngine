#include "Texture.h"
#include <SOIL/SOIL.h>
#include "image.h"

Texture::Texture() 
{

}
Texture::Texture(std::string filePath) 
: mTextureId(0),
mFormat(0),
mWidth(0),
mHeight(0)
{
	mPath = filePath;
	mFormat = GL_RGBA;
	
	uint8* lImageBuffer = SOIL_load_image(mPath.c_str(), &mWidth,&mHeight, 0,SOIL_LOAD_RGB);
	if(lImageBuffer == NULL)
	{
		std::cout << "ReadSOIL FAILED - NO IMAGE BUFFER" << std::endl;
		std::cout << "width: " << mWidth << " height: "<< mHeight << std::endl;

		return;
	}

	std::cout << "width: " << mWidth << " height: "<< mHeight << std::endl;

	glGenTextures(1, &mTextureId);
	glBindTexture(GL_TEXTURE_2D, mTextureId);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA , mWidth, mHeight, 0,GL_RGB, GL_UNSIGNED_BYTE, lImageBuffer);

	delete[] lImageBuffer;
}


Texture::Texture(int width,int height,bool alpha,uint8 *lImageBuffer) 
: mTextureId(0),
mFormat(0),
mWidth(width),
mHeight(height)
{
	if (alpha)
    {
        mFormat = GL_RGBA;
    }
    else
    {
		mFormat = GL_RGB;
    }    
	glGenTextures(1, &mTextureId);
	glBindTexture(GL_TEXTURE_2D, mTextureId);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mWidth, mHeight, 0, mFormat, GL_UNSIGNED_BYTE, lImageBuffer);

	delete[] lImageBuffer;
}

Texture::~Texture()
{
	if(mTextureId != 0)
	{
		glDeleteTextures(1, &mTextureId);
		mTextureId = 0;
	}
	mWidth = 0;
	mHeight = 0;
	mFormat = 0;
}

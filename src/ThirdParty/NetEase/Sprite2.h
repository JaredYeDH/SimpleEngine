#ifndef SPRITE2_H
#define SPRITE2_H 
#include "../../defines.h"
#include "../../defineTypes.h"
#include <vector>
/*
һ������������
*/


class Sprite2
{

public:
	Sprite2();
	~Sprite2();

	struct Sequence
	{
		int key_x;
		int key_y;
		int width;
		int height;
		uint32 format;
		uint32* src;
	};

	int mGroupSize;		//������
	int mFrameSize;		//֡��
	int mWidth;			//���
	int mHeight;		//�߶�
	int mKeyX;			//�ؼ�֡X
	int mKeyY;			//�ؼ�֡Y
	String mID;
	String mPath;
	Sequence** mFrames;
	void SaveImage(int index);
};
#endif
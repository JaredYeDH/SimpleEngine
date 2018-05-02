#ifndef WAS_H
#define WAS_H 
#include "../../defines.h"
#include <string>
#include <fstream>
#include <memory.h>
using namespace std;




class WAS
{
public:

	// ���鶯�����ļ�ͷ
	struct Header
	{
		uint16 flag;		// �����ļ���־ SP 0x5053
		uint16 len;		// �ļ�ͷ�ĳ��� Ĭ��Ϊ 12
		uint16 group;		// ����ͼƬ����������������
		uint16 frame;		// ÿ���ͼƬ������֡��
		uint16 width;		// ���鶯���Ŀ�ȣ���λ����
		uint16 height;		// ���鶯���ĸ߶ȣ���λ����
		uint16 key_x;		// ���鶯���Ĺؼ�λX
		uint16 key_y;		// ���鶯���Ĺؼ�λY
	};

	// ֡���ļ�ͷ
	struct FrameHeader
	{
		uint32 key_x;			// ͼƬ�Ĺؼ�λX
		uint32 key_y;			// ͼƬ�Ĺؼ�λY
		uint32 width;			// ͼƬ�Ŀ�ȣ���λ����
		uint32 height;			// ͼƬ�ĸ߶ȣ���λ����
	};


	WAS(std::string filename, int offset, int size);
	WAS(fstream &infile, int offset, int size);
	static uint32 RGB565to888(uint16 color, uint8 alpha); // 565ת888
	static uint16 Alpha565(uint16 src, uint16 des, uint8 alpha);
	static uint8 MixAlpha(uint8 color, uint8 alpha);
	~WAS();
	Header mHeader;
	uint32 mPalette32[256];
	uint32* mFrameIndecies;
	std::string mFileName;
	uint32 mFileOffset;
	uint32 mFileSize;

};
#endif

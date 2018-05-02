#ifndef WDF_H
#define WDF_H 
// #include <string>
// #include <vector>
#include <map>
// #include <iostream>
#include <fstream>
#include "WAS.h"
#include "Sprite2.h"

// #include "Sprite.h"
using namespace std;
namespace NetEase {
	class WDF
	{
		struct Header
		{
			uint32 flag; // �����ı�ǩ
			uint32 number; // �����е��ļ�����
			uint32 offset; // �����е��ļ��б�ƫ��λ��
		};

		struct Index
		{
			uint32 hash; // �ļ�������ɢ��
			uint32 offset; // �ļ���ƫ��
			uint32 size; // �ļ��Ĵ�С
			uint32 spaces; // �ļ��Ŀռ�
		};

	public:
		WDF();
		WDF(std::string path) :mFilePath(path) { Init(); }
		void DataHandler(char *pData, uint32* pBmpStart, int pixelOffset, int pixelLen);
		void Init();
		// Sprite LoadSprite(int id);
		WAS GetWAS(uint32 id);
		std::shared_ptr<Sprite2> LoadSprite(uint32 id);
		std::vector<uint32> GetAllWASIDs()
		{
			std::vector<uint32> ids;
			for(int i=0;i<mHeader.number;i++)
			{
				ids.push_back(mIndencies[i].hash);
			}
			return ids;
		}
		std::vector<std::shared_ptr<Sprite2>> LoadAllSprite();
		~WDF();

	public:
		Header mHeader;
		Index* mIndencies;
		map<uint32, uint32> mIdToPos;
		fstream mFile;
		uint16 palette16[256];
		string mFilePath;
		uint32 m_Palette32[256];
		string mFileName;

	};
}
#endif

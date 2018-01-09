#pragma once

#include "../../defines.h"

#include <string>
#include <fstream>
#include <vector>

using std::string;
using std::ifstream;
using std::fstream;

namespace NetEase {
	// µØÍ¼µÄÎÄ¼þÍ·(ÃÎ»Ã¡¢´ó»°2)
	struct MapHeader
	{
		//public:
		// ÎÄ¼þÍ·½á¹¹¹²12×Ö½Ú
		uint32		Flag;		//ÎÄ¼þ±êÖ¾
		uint32		Width;		//µØÍ¼¿í
		uint32		Height;		//µØÍ¼¸ß	
	};

	// µØÍ¼µÄµ¥ÔªÍ·
	struct MapUnitHeader
	{
		//public:
		uint32		Flag;		// µ¥Ôª±êÖ¾
		uint32		Size;		// µ¥Ôª´óÐ¡
	};

	// µØÍ¼µÄÊý¾Ý
	struct MapData
	{
		//public:
		uint32		Size;		// Êý¾Ý´óÐ¡
		uint8		*Data;		// Êý¾ÝÄÚÈÝ
	};

	struct MaskHeader
	{
		uint32	Flag;
		uint32	Size;
	};

	struct BaseMaskInfo
	{
		int	StartX;
		int	StartY;
		uint32	Width;
		uint32	Height;
		uint32	Size;	// maskÊý¾Ý´óÐ¡

	};

	struct MaskInfo : BaseMaskInfo
	{
		uint32* Data;
	};

	struct UnKnown
	{
		uint32 Offset;
		uint32 *Data;			//n×Ö½Ú Î´ÖªÓÃÍ¾£¬´óÐ¡Îª£ºµÚÒ»¸öµ¥ÔªÒýË÷Öµ¼õÈ¥ÎÄ¼þÍ·´óÐ¡¡£
	};

	struct MapUnit
	{
		uint8  Cell[192];
		uint8* BitmapRGB24;
		uint32 Index;
		bool bHasLoad = false;
	};

	class MAP
	{
	public:
		MAP(string filename);
		~MAP();
		void ReadUnit(int index);
		void SaveUnit(int index);
		void ReadUnit(int row, int col) { ReadUnit(row*m_ColCount + col); };
		void ReadMask(int index);
		uint8* MapHandler(uint8* jpegData, uint32 inSize, uint32* outSize);
		bool ReadJPEG(ifstream &file, uint32 size, uint32 index);
		bool ReadCELL(ifstream &file, uint32 size, uint32 index);
		bool ReadBRIG(ifstream &file, uint32 size, uint32 index);
		void PrintCellMap();
		void SaveImageFile(char* filename, int width, int height, int pixelDepth, char* data);
		ifstream m_FileStream;
		string m_FileName;
		int m_Width;
		int m_Height;
		int m_MapWidth;
		int m_MapHeight;
		float m_BlockWidth;
		float m_BlockHeight;
		uint32 m_RowCount;
		uint32 m_ColCount;
		uint32 m_Pixcels;
		MapHeader m_Header;
		uint32* m_UnitIndecies;	//4*n×Ö½Ú  µØÍ¼µ¥ÔªµÄÒýË÷ n=µØÍ¼µÄ¿í¶È/640*2 * µØÍ¼¸ß¶È/480*2
		uint32 m_UnitSize;

		MaskHeader m_MaskHeader;
		uint32* m_MaskIndecies;
		uint32 m_MaskSize;
		std::vector<MapUnit> m_MapUnits;
		std::vector<MaskInfo> m_MaskInfos;
		uint8* m_MapPixelsRGB24;
		//uint8* m_Cur_MapPixelsRGB24;


	};
}
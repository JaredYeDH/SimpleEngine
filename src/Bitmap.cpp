#include "Bitmap.h"
#include <memory>

Bitmap::Bitmap()
{

}

Bitmap::~Bitmap()
{
}

bool Bitmap::Load(std::string path,BitmapFile& bitmapFile)
{
	using std::cout;
	using std::endl;
	
	std::fstream f(path, std::ios::in | std::ios::binary);
	if (f.rdstate() == f.failbit)
	{
		cout << "open file error!" << endl;
		return false;
	}

	f.read((char*)&bitmapFile.header, sizeof(BitmapFileHeader)) ;
	if (bitmapFile.header.bfType != BITMAPFILE_ID)
	{
		f.close();
		return false;
	}

	f.read((char*)&bitmapFile.infoHeader, sizeof(BitmapInfoHeader));

	if (bitmapFile.infoHeader.biBitCount == 8)
	{
		memset(bitmapFile.palettes, 0, sizeof(bitmapFile.palettes));
		f.read((char*)bitmapFile.palettes, BITMAPFILE_PAL_SIZE * sizeof(uint32));
	}
	

	f.seekg(bitmapFile.header.bfOffBits, std::ios::beg);

	bitmapFile.imageData = new uint8[bitmapFile.infoHeader.biSizeImage];
	memset(bitmapFile.imageData, 0, bitmapFile.infoHeader.biSizeImage);
	f.read((char*)bitmapFile.imageData, bitmapFile.infoHeader.biSizeImage);
	f.close();
	
	return true;
}

bool Bitmap::CreateBitmapFile(int w,int h,int colorBitCount,std::string path)
{
	
	BitmapFile file;
	memset(&file, 0, sizeof(BitmapFile));
	
	int lineSize = (w*colorBitCount/8 + 3) / 4 * 4;
	int dataSize = h * lineSize;
	int size1 = sizeof(BitmapFileHeader);
	int size2 = sizeof(BitmapInfoHeader);
	int headSize = sizeof(BitmapFileHeader) + sizeof(BitmapInfoHeader);
	int patSize = colorBitCount == 8 ? BITMAPFILE_PAL_SIZE * sizeof(uint32) : 0;

	file.infoHeader.biSize = sizeof(BitmapInfoHeader);
	file.infoHeader.biPlanes = 1;
	file.infoHeader.biCompression = 0;
	file.infoHeader.biWidth = w;
	file.infoHeader.biHeight = h;
	file.infoHeader.biBitCount = colorBitCount;
	file.infoHeader.biSizeImage = dataSize;
	file.infoHeader.biClrUsed = colorBitCount == 8 ? 256 : 0;
	file.infoHeader.biClrImportant = file.infoHeader.biClrUsed;
	
	file.header.bfType = BITMAPFILE_ID;
	file.header.bfSize = headSize + patSize + dataSize;
	file.header.bfOffBits = headSize + patSize;

	std::fstream f(path, std::ios::out | std::ios::binary);
	if (f.rdstate() == f.failbit)
	{
		std::cout << "open file error!" << std::endl;
		return false;
	}

	f.write((char*)&file.header, sizeof(BitmapFileHeader));
	f.write((char*)&file.infoHeader, sizeof(BitmapInfoHeader));

	if (file.infoHeader.biBitCount == 8)
	{
		memset(file.palettes, 0xff, sizeof(file.palettes));
		f.write((char*)file.palettes, BITMAPFILE_PAL_SIZE * sizeof(uint32));
	}

	f.seekp(file.header.bfOffBits, std::ios::beg);
	file.imageData = new uint8[file.infoHeader.biSizeImage];
	memset(file.imageData, 0xff, file.infoHeader.biSizeImage);
	f.write((char*)file.imageData, file.infoHeader.biSizeImage);
	delete file.imageData;
	file.imageData = nullptr;

	f.close();
	
	return true;
	
}


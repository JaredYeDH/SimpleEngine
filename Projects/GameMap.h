#pragma once
#include "../defines.h"
#include "../ThirdParty/NetEase/MAP.h"
#include "../SpriteRenderer.h"
#include "Heap.h"
#include "Astar.h"

#include <list>
#include "../image.h"
#include "../Pos.h"
/*
游戏地图类的封装
*/
class GameMap
{
public:
	
	GameMap(uint32 mapId);

	~GameMap();
	std::list<Pos> Move(int sx, int sy, int ex, int ey);
	NetEase::MAP* GetMapPtr();
	void clamp(int val, int min, int max);
	Astar* GetAstar() { return mAstar; };
	int GetRowCount() { return mRow; };
	int GetColCount() { return mCol; };
	void Draw(SpriteRenderer* renderer, int playerX, int playerY);
	void DrawMask(SpriteRenderer* renderer, int playerX, int playerY);
	void DrawCell(SpriteRenderer* renderer, int cur_x, int cur_y);
	int GetWidth(){return mWidth;}
	int GetHeight(){return mHeight;}
private:
	std::map<int,Texture *> mMapTiles;
	std::vector<Texture *> mMaskTiles;
	// Texture mCellPic;
	int mWidth;
	int mHeight;
	int mMapWidth;
	int mMapHeight;
	int mCellWidth;
	int mCellHeight;
	int** mCell;
	int mRow;
	int mCol;
	NetEase::MAP* mXyqMap;
	Astar* mAstar;

};



#include "GameMap.h"

#include "../astar/include/InputHandler.h"


GameMap::GameMap(uint32 mapId)
{
	std::string fileName = Environment::GetMapPath("1001");

	mXyqMap = new NetEase::MAP(fileName);

	mMapWidth = mXyqMap->MapWidth();
	mMapHeight = mXyqMap->MapHeight();

	mWidth = mXyqMap->SliceWidth();
	mHeight = mXyqMap->SliceHeight();
	
	mRow = mXyqMap->Row();
	mCol = mXyqMap->Col();
	printf("初始化GameMap %d %d ", mRow, mCol);

	mMapTileWidth = 320;
	mMapTileHeight= 240;
    mMapTileCoef = SCREEN_WIDTH/2/mMapTileWidth+1;
	mMapTiles.clear();
	
// 	for (int i = 0; i<mXyqMap->UnitSize(); i++)
// 	{
// 		mXyqMap->ReadUnit(i);
		
// 		mMapTiles[i] = new Texture(mMapTileWidth,mMapTileHeight, false,mXyqMap->GetUnitBitmap(i));
// 	//	 delete[] mXyqMap->m_MapUnits[i].BitmapRGB24;
// //		mXyqMap->m_MapUnits[i].BitmapRGB24 = nullptr;
// 	}
	
	mMaskTiles.clear();
	for (int i = 0; i<mXyqMap->MaskSize(); i++)
	{
		mXyqMap->ReadMask(i);

		mMaskTiles.push_back(new Texture(mXyqMap->GetMaskWidth(i),
			mXyqMap->GetMaskHeight(i),true,(uint8*) mXyqMap->GetMaskBitmap(i) ));

//		 delete[] mXyqMap->m_MaskInfos[i].Data;
//		mXyqMap->m_MaskInfos[i].Data = nullptr;
	}


	
	mCellWidth = mCol * 16;
	mCellHeight = mRow * 12;
	mCell = new int*[mCellWidth];
	for (int i = 0; i < mCellWidth; i++)
	{
		mCell[i] = new int[mCellHeight];
	}

	//mAstar = new Astar(mCellWidth, mCellHeight, mCell);

	// std::ofstream outfile("a.map");
	// for (int y = 0; y < mCellHeight; y++) 
	// {
	// 	for (int x = 0; x < mCellWidth; x++) {
	// 		//printf("%d", mMap[x][y]);
	// 		outfile << mCell[x][y];
	// 	}
	// 	outfile << '\n';
	// 	//printf("\n");
	// }
	// outfile.close();

	

	mCellPic = new Texture(Environment::GetAssetsPath("X.png"));

}

void GameMap::clamp(int val, int min, int max)
{
	if (val < min) val = min;
	if (val > max) val = max;
}

NetEase::MAP* GameMap::GetMapPtr()
{
	return mXyqMap;
}

bool GameMap::CanArriveDirect(Pos src,Pos dest)
{
	int dx = dest.x - src.x;
	int dy = dest.y - src.y;
	if(dx == 0)
	{
		int opt = dy < 0 ? -1 : 1;
		for(int i=0;i<dy*opt;i++)
		{
			int cellX = src.x;
			int cellY = src.y + i*opt;
			if(mCell[cellX][cellY]>=1)
				return false;
		}
		return true;
	}

	if(dy == 0)
	{
		int opt = dx < 0 ? -1 : 1;
		for(int i=0;i<dx*opt;i++)
		{
			int cellX = src.x + i*opt;
			int cellY = src.y;
			if(mCell[cellX][cellY]>=1)
				return false;
		}
		return true;
	}

	double ratio = dy / dx;
	int opt = dx < 0 ? -1 : 1;
	for (int i = 0; i < dx * opt; ++i)
	{
		int cellX = src.x + opt*i;
		int cellY = src.y + ratio*i*opt;
		if(mCell[cellX][cellY]>=1)
			return false;
	}

	ratio = dx / dy;
	opt = dy < 0 ? -1 : 1;
	for (int i = 0; i < dy * opt; ++i)
	{
		int cellX = src.x + ratio*opt*i;
		int cellY = src.y + i*opt;
		if(mCell[cellX][cellY]>=1)
			return false;
	}

	return true;

}
void GameMap::UpdateCell()
{
	int x = 0, y = 0 ,p,q;
	for (int i = 0; i < mRow; i++)
	{
		for (int j = 0; j < mCol; j++)
		{
			int index = i*mCol + j;
			if(!mXyqMap->HasUnitLoad(index)) continue;

			y = 12 * i;
			for (p = 0; p < 12; p++)
			{
				x = 16 * j;
				for (q = 0; q < 16; q++)
				{
					mCell[x++][y] =mXyqMap->GetUnit(i*mCol + j).Cell[p * 16 + q];
				}
				y++;
			}
		}
	}
}

std::list<Pos> GameMap::Move(int sx, int sy, int ex, int ey)
{
	//printf(" GameMap:sx %d sy %d ex %d ey %d\n", sx, sy, ex, ey);
	if (sx < 0 || sy < 0 || ex < 0 || ey < 0) return{};
	if (sx > mCellWidth || ex > mCellWidth || sy > mCellHeight || ey > mCellHeight) return{};
	if (sx == ex && sy == ey) return{};
	if (mCell[sx][sy] > 0 || mCell[ex][ey] > 0) return{};

	// 	13 ，35
	// 115，20
	// if (mAstar->PathFinding(sx, sy, ex, ey))
	// {
	// 	return mAstar->GetMoveList();
	// }
	// else {
	// 	return{ };
	// }
	int dimension = mCellHeight<mCellWidth? mCellWidth:mCellHeight;
	SquareGraph graph(dimension);
	for (int y = 0; y < dimension; y++)
	{	
		for (int x = 0; x < dimension; x++)
		{
			if(x>=mCellWidth || y >= mCellHeight)
			{
				graph.setCellValue(std::make_pair(y,x),'X');
				continue;
			}
			if (mCell[x][y] >= 1)
			{
				graph.setCellValue(std::make_pair(y,x),'X');
			}else
			{
				graph.setCellValue(std::make_pair(y,x),' ');
			}
		}
	}
	graph.setFirstRobotPos(std::make_pair(sy,sx));
	graph.setSecondRobotPos(std::make_pair(ey,ex));
	std::vector<Node> path = graph.executeAStar();
	//graph.printPath(path);

	std::vector<Pos> moveList;
	moveList.clear();
	for(auto i=path.begin(); i != path.end(); i++){
		Node node = *i;
		//cout << "node : (" << node.x << "," << node.y << ")" << endl;
        moveList.push_back(Pos(node.y,node.x));
	}

    // Smooth move List
    /*
     算法：
     1.第一个节点为当前节点，查看下一个节点
     2.如果可以无障碍直接到达，则下一个节点为第一个节点的后继
     3.如果不可到达，当前节点为后继节点
     */
    int currentNode = 0;
    int nextNode = 1;
    std::list<Pos> smoothMoveList;
    smoothMoveList.push_back(moveList[currentNode]);
    while(nextNode!=moveList.size())
    {
    	int lastNextNode = nextNode;
        while(nextNode!=moveList.size() && CanArriveDirect(moveList[currentNode],moveList[nextNode]) )
        {
        	lastNextNode = nextNode;
        	nextNode++;
        }
        currentNode=lastNextNode;
        smoothMoveList.push_back(moveList[currentNode]);
    }

    for(auto i=smoothMoveList.begin(); i != smoothMoveList.end(); i++){
		Pos node = *i;
		cout << "smoothNode : (" << node.x << "," << node.y << ")" << endl;
        
	}

	return smoothMoveList;

}

GameMap::~GameMap()
{
	if (mXyqMap != nullptr) {
		delete mXyqMap;
		mXyqMap = nullptr;
	}
}
void GameMap::Draw(int playerX,int playerY)
{
	SpriteRenderer* renderer = SPRITE_RENDERER_INSTANCE;
	int screenWidth = SCREEN_WIDTH;
	int screenHeight = SCREEN_HEIGHT;
	int halfScreenWidth = screenWidth / 2;
	int halfScreenHeight = screenHeight / 2;

	int mapOffsetX = halfScreenWidth - playerX;
	int mapOffsetY = halfScreenHeight - playerY;

	mapOffsetX = GMath::Clamp(mapOffsetX, -mWidth + screenWidth, 0);
	mapOffsetY = GMath::Clamp(mapOffsetY, -mHeight + screenHeight, 0);

	// mGameMap->Draw(m_RendererPtr, mapOffsetX, mapOffsetY);
    int startRow , endRow,startCol,endCol ;
    startRow = playerY/mMapTileHeight-3;
    startRow = startRow<0?0:startRow;
    endRow = playerY/mMapTileHeight+3;
    endRow = endRow>mRow?mRow:endRow;

    startCol = playerX/mMapTileWidth-3;
    startCol = startCol<0?0:startCol;
    endCol = playerX/mMapTileWidth+3;
    endCol = endCol>mCol?mCol:endCol;


	for (int i = startRow; i<endRow; i++) {
		for (int j = startCol; j<endCol; j++) {
            int unit = i*mCol+j;

            if(mMapTiles.find(unit) == mMapTiles.end())
            {
            	mXyqMap->ReadUnit(unit);
                //mXyqMap->SaveUnit(unit);

                mMapTiles[unit] = new Texture(mMapTileWidth,mMapTileHeight, false,mXyqMap->GetUnitBitmap(unit));
				UpdateCell();
            }

			renderer->DrawSprite(mMapTiles[i*mCol + j],
				glm::vec2(j * mMapTileWidth + mapOffsetX, i * mMapTileHeight + mapOffsetY),
				glm::vec2(mMapTileWidth, mMapTileHeight),
				0.0f,
				glm::vec3(1.0f, 1.0f, 1.0f)
			);
		}
	}
}


void GameMap::DrawCell(int cur_x, int cur_y)
{
	SpriteRenderer* renderer = SPRITE_RENDERER_INSTANCE;
    int minx,maxx,miny,maxy;
    int posx,posy;
	int screenWidth = SCREEN_WIDTH;
	int screenHeight = SCREEN_HEIGHT;
	int halfScreenWidth = screenWidth / 2;
	int halfScreenHeight = screenHeight / 2;

    posx = -cur_x + halfScreenWidth;
    posy = -cur_y + halfScreenHeight;
    minx = posx - (halfScreenWidth+10) ;
    minx = minx<0?0:minx;
    maxx = posx +  (halfScreenWidth+10) ;
    maxx = maxx>mMapWidth?mMapWidth:maxx;

    miny = posy - (halfScreenHeight+10);
    miny = miny<0?0:miny;
    maxy = posy + (halfScreenHeight+10);
    maxy = maxy>mMapHeight?mMapHeight:maxy;


	//for (int y = 0; y < mCellHeight; y++)
    for (int y = miny/20; y < maxy/20; y++)
	{
		//for (int x = 0; x < mCellWidth; x++)
        for (int x = minx/20; x < maxx/20; x++)
		{
			if (mCell[x][y] >= 1)
			{
				 renderer->DrawSprite(mCellPic,
				 	glm::vec2( x*20 + cur_x,
				 		y*20 + cur_y),
				 	glm::vec2(20, 20),
				 	0.0f, glm::vec3(1.0f, 1.0f, 1.0f));
			}
		}
	}
}


void GameMap::DrawMask(int playerX, int playerY)
{
	SpriteRenderer* renderer = SPRITE_RENDERER_INSTANCE;
	int screenWidth = SCREEN_WIDTH;
	int screenHeight = SCREEN_HEIGHT;
	int halfScreenWidth = screenWidth / 2;
	int halfScreenHeight = screenHeight / 2;

	int mapOffsetX = halfScreenWidth - playerX;
	int mapOffsetY = halfScreenHeight - playerY;

	mapOffsetX = GMath::Clamp(mapOffsetX, -mWidth + screenWidth, 0);
	mapOffsetY = GMath::Clamp(mapOffsetY, -mHeight + screenHeight, 0);
	
	int startRow , endRow,startCol,endCol ;
    startRow = GMath::Clamp(playerY/mMapTileHeight-mMapTileCoef, 0,mRow );
    endRow = GMath::Clamp(playerY/mMapTileHeight+mMapTileCoef, 0,mRow );
	startCol = GMath::Clamp(playerX/mMapTileWidth-mMapTileCoef, 0,mCol);
    endCol = GMath::Clamp(playerX/mMapTileWidth+mMapTileCoef, 0,mCol );

	int startX,startY, endX,endY;
	
	startY = GMath::Clamp(startRow*mMapTileHeight,0, mMapHeight);
	endY = GMath::Clamp(endRow*mMapTileHeight,0, mMapHeight);

	startX = GMath::Clamp(startCol*mMapTileWidth,0, mMapWidth);
	endX = GMath::Clamp(endCol*mMapTileWidth,0, mMapWidth);

	
	for (int m = 0; m < mXyqMap->MaskSize(); m++)
	{
		NetEase::MaskInfo& info = mXyqMap->GetMask(m);
		int x = info.StartX;
		int y = info.StartY;
		int w = info.Width;
		int h = info.Height;
		if ( x+w < startX || x > endX || y+h < startY || y>endY)continue;
		renderer->DrawMask(
		mMaskTiles[m],
		glm::vec2(x+mapOffsetX,y+mapOffsetY),
		glm::vec2(w,h),
		0.0f, glm::vec3(1.0f, 1.0f, 1.0f));
	}

	for (int i = startRow; i<endRow; i++) {
		for (int j = startCol; j<endCol; j++) {
            int unit = i*mCol+j;

            if(mMapTiles.find(unit) == mMapTiles.end())
            {
            	mXyqMap->ReadUnit(unit);
                //mXyqMap->SaveUnit(unit);
                mMapTiles[unit] = new Texture(mMapTileWidth,mMapTileHeight, false,mXyqMap->GetUnitBitmap(unit));
				UpdateCell();
            }

			renderer->DrawMapSprite(mMapTiles[i*mCol + j],
				glm::vec2(j * mMapTileWidth + mapOffsetX, i * mMapTileHeight + mapOffsetY),
				glm::vec2(mMapTileWidth, mMapTileHeight),
				0.0f,
				0.5f
			);
		}
	}
}


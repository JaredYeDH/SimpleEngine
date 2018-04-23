#pragma once 

#include "ThirdParty/NetEase/Sprite2.h"
#include "core/Texture.h"
#include "TextureManager.h"
#include <vector>
#include "SpriteRenderer.h"

class FrameAnimation
{
public:

	enum struct Dir : int
	{
		N = 6,
		S = 4,
		W = 5,
		E = 7,
		N_E = 3,
		N_W = 2,
		S_E = 0,
		S_W = 1,
	};

	FrameAnimation(Sprite2 sprite);
	FrameAnimation();
	FrameAnimation& operator=(const FrameAnimation& rhs);
	~FrameAnimation();

	int GetCurrentFrame() { return m_CurrentFrame;}
	int GetFrameCount() { return m_FrameCount;}
	int GetCurrentGroup() { return m_CurrentGroup;}

	void SetCurrentGroup(int group);
	int GetGroupFrameCount() { return m_GroupFrameCount;}

	int GetKeyX() { return m_KeyX; }
	int GetKeyY() { return m_KeyY; }

	int GetPosX() { return m_PosX; }
	int GetPosY() { return m_PosY; }

	void SetPosX(int x) { m_PosX = x; }
	void SetPosY(int y) { m_PosY = y; }

	void SetVisible(bool visible);

	int GetWidth() { return m_Width; }
	int GetHeight() { return m_Height; }

	void Reset(int group );
	void ResetAnim(int group);
	void SetPlayLoop(bool loop) {m_bLoop = loop;};
	Texture* GetFrame(); 

	void OnUpdate(double dt);
	void Draw(SpriteRenderer* renderer,int x, int y);

	int m_FrameCount;
	int m_CurrentFrame;
	int m_CurrentGroup;
	int m_GroupFrameCount;
	int m_KeyX;
	int m_KeyY;
	int m_PosX;
	int m_PosY;
	int m_Width;
	int m_Height;
	bool m_bLoop;

	double m_DeltaTime;
	bool m_bVisible;

	std::vector<String> m_Sprites;
};

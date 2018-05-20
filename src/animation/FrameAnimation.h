#pragma once 

#include <Sprite2.h>
#include "core/Texture.h"
#include "TextureManager.h"
#include <vector>
#include "SpriteRenderer.h"
#include "Pos.h"
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

	FrameAnimation(std::shared_ptr<Sprite2> sprite);
	FrameAnimation();
	FrameAnimation& operator=(const FrameAnimation& rhs);
	~FrameAnimation();

	int GetCurrentFrame() { return m_CurrentFrame;}
	bool IsFirstFrame() { return m_CurrentFrame %m_GroupFrameCount ==0 ;}
	int GetLastFrame() { return m_LastFrame;}
	int GetFrameCount() { return m_FrameCount;}
	int GetCurrentGroup() { return m_CurrentGroup;}
	bool IsCurrentFrameChangedInUpdate() {return m_bCurrentFrameChangedInUpdate;};
	void SetCurrentGroup(int group);
	int GetGroupFrameCount() { return m_GroupFrameCount;}

	int GetKeyX() { return m_KeyX; }
	int GetKeyY() { return m_KeyY; }

	const IntPos GetPos() { return m_Pos; }
	void SetPos(IntPos pos) { m_Pos = pos; }
	

	void SetVisible(bool visible);

	int GetWidth() { return m_Width; }
	int GetHeight() { return m_Height; }

	void ResetAll();
	void Reset(int group );
	void ResetAnim(int group);
	void ResetFrameTimeByGroupCount(int groupCount);
	
	void SetFrameTimeBase(double base);
	
	void SetPlayLoop(bool loop) {m_bLoop = loop;};
	Texture* GetFrame(int index); 
	String GetFramePath(int index); 
	int GetSpritesCount(){ return m_Sprites.size();};
	void SetPivotPos(Pos pos)
	{
		m_Pos.x = pos.x -m_Width/2;
		m_Pos.y = pos.y -m_Height/2;
	}
	void OnUpdate();
	void Draw();
	bool IsNextFrameRestart(){return m_bIsNextFrameRestart;};

	Bound GetBound() { return Bound{ (int)m_Pos.x, (int)(m_Pos.x + m_Width), (int)m_Pos.y, (int)(m_Pos.y + m_Height) }; }
	int m_FrameCount;
	int m_CurrentFrame;
	int m_LastFrame;
	int m_CurrentGroup;
	int m_GroupFrameCount;
	int m_KeyX;
	int m_KeyY;
	IntPos m_Pos;
	int m_Width;
	int m_Height;
	bool m_bLoop;
	bool m_bIsNextFrameRestart;

	double m_DeltaTime;
	bool m_bVisible;
	bool m_bCurrentFrameChangedInUpdate;
	int m_LastNotBlankFrame;

	double m_FrameTime;
	double m_FrameTimeBase;
	std::vector<String> m_Sprites;
	std::vector<int> m_IsBlankFrame;
};

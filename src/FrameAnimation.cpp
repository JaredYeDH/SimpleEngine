#include "FrameAnimation.h"

FrameAnimation::FrameAnimation(Sprite2 sprite)
{
	m_CurrentFrame = 0;
	m_CurrentGroup = 0;
	m_GroupFrameCount = sprite.mFrameSize;
	m_FrameCount = sprite.mFrameSize * sprite.mGroupSize;
	m_DeltaTime = 0;
	
	m_KeyX = sprite.mKeyX;
	m_KeyY = sprite.mKeyY;
	m_Width = sprite.mWidth;
	m_Height = sprite.mHeight;
	
	for (int i = 0; i< m_FrameCount ; i++) {
		int gpos = i / m_GroupFrameCount;
		int cpos = i % m_GroupFrameCount;

		Texture * t= new Texture(
				sprite.mWidth,
				sprite.mHeight,
				true, 
				(uint8*)&(sprite.mFrames[gpos][cpos].src[0]) 
				);

		m_Textures.push_back(t);
	}
	m_bVisible = true;
}

FrameAnimation::~FrameAnimation()
{
	for (auto p_texture: m_Textures)
	{
		delete p_texture;
	}
}

void FrameAnimation::SetCurrentGroup(int group)
{
	m_CurrentFrame = m_CurrentFrame%m_GroupFrameCount + group*m_GroupFrameCount;
	m_CurrentGroup = group;
	//m_DeltaTime = 0;
}


void FrameAnimation::OnUpdate(double dt)
{
	m_DeltaTime += dt;
	if( m_DeltaTime  >= 4*dt)
	{
		m_DeltaTime = 0;
		m_CurrentFrame++;
		if( m_CurrentFrame % m_GroupFrameCount == 0)
		{
			/*m_CurrentGroup++;
			if (m_CurrentGroup >= 8)
			{
				m_CurrentGroup = 0;
			}*/
			m_CurrentFrame = (m_CurrentGroup)* m_GroupFrameCount;
		}
	}
}

void FrameAnimation::Reset(int group)
{
	SetCurrentGroup(group);
	m_DeltaTime = 0;
}


void FrameAnimation::SetVisible(bool visible)
{
	m_bVisible = visible;
}


void FrameAnimation::Draw(SpriteRenderer* renderer,int posX ,int posY)
{

	
		m_PosX = posX;
		m_PosY = posY;
		
		if (m_CurrentFrame >= m_Textures.size()) return;

		Texture* t = m_Textures[m_CurrentFrame];
		renderer->DrawSprite(t,
			glm::vec2(m_PosX, m_PosY),
			glm::vec2(t->GetWidth(), t->GetHeight()), 0.0f, glm::vec3(1.0f, 1.0f, 1.0f));
	
	
	
}
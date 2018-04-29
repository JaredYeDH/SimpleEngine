#include "FrameAnimation.h"


FrameAnimation::FrameAnimation()
:m_PosX(0),
m_PosY(0)
{
	m_CurrentFrame = 0;
	m_CurrentGroup = 0;
	m_GroupFrameCount = 0;
	m_FrameCount = 0;
	m_DeltaTime = 0;
	
	m_bVisible = false;
	m_bLoop = false;
	m_Sprites.clear();
}

FrameAnimation::FrameAnimation(Sprite2 sprite)
:m_PosX(0),
m_PosY(0)
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
	
	m_Sprites.clear();

	for (int i = 0; i< m_FrameCount ; i++) {
		int gpos = i / m_GroupFrameCount;
		int cpos = i % m_GroupFrameCount;
		
		String tPath = sprite.mPath+"/"+std::to_string(i);
		Texture* t = TextureManager::GetInstance()->LoadTexture(
			tPath,
			sprite.mWidth,sprite.mHeight,true,(uint8*)&(sprite.mFrames[gpos][cpos].src[0])
			);

		// Texture * t= new Texture(
		// 		sprite.mWidth,
		// 		sprite.mHeight,
		// 		true, 
		// 		(uint8*)&(sprite.mFrames[gpos][cpos].src[0]) 
		// 		);

		m_Sprites.push_back(tPath);
	}
	m_bVisible = true;
	m_bLoop = true;
	
	m_FrameTime = 1.0 / 60 * 4 ;
}
void FrameAnimation::ResetFrameTime(int groupCount)
{
	m_FrameTime = 1.0 / 60 * 4 * groupCount/ m_GroupFrameCount;
	
}
FrameAnimation& FrameAnimation::operator=(const FrameAnimation& rhs)
{
	this->m_CurrentFrame = rhs.m_CurrentFrame;
	this->m_CurrentGroup = rhs.m_CurrentGroup;
	this->m_GroupFrameCount = rhs.m_GroupFrameCount;
	this->m_FrameCount = rhs.m_FrameCount;
	this->m_DeltaTime = rhs.m_DeltaTime;
	
	this->m_KeyX = rhs.m_KeyX;
	this->m_KeyY = rhs.m_KeyY;
	this->m_Width = rhs.m_Width;
	this->m_Height = rhs.m_Height;
	
	this->m_Sprites = rhs.m_Sprites;
	this->m_bVisible = rhs.m_bVisible;
	this->m_bLoop = rhs.m_bLoop;
	this->m_PosX = rhs.m_PosX;
	this->m_PosY = rhs.m_PosY;
	this->m_FrameTime = rhs.m_FrameTime;
	return *this;
}


FrameAnimation::~FrameAnimation()
{
	// for (auto p_texture: m_Textures)
	// {
	// 	delete p_texture;
	// }
}

Texture* FrameAnimation::GetFrame(int index = -1)
{ 
	if ( index  < 0 ) index = m_CurrentFrame;
	auto path =  m_Sprites[index];		
	return TextureManager::GetInstance()->GetTexture(path);
}

String FrameAnimation::GetFramePath(int index = -1)
{ 
	if ( index  < 0 ) index = m_CurrentFrame;
	return  m_Sprites[index];		
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
	if( m_DeltaTime  >= m_FrameTime)
	{
		m_DeltaTime = m_DeltaTime - m_FrameTime;
		m_CurrentFrame++;
		if( m_CurrentFrame % m_GroupFrameCount == 0 )
		{
			/*m_CurrentGroup++;
			if (m_CurrentGroup >= 8)
			{
				m_CurrentGroup = 0;
			}*/
			if (!m_bLoop)
			{
				m_CurrentFrame--;
			}
			else
			{
				m_CurrentFrame = (m_CurrentGroup)* m_GroupFrameCount;
			}
		}
	}
}

void FrameAnimation::Reset(int group)
{
	SetCurrentGroup(group);
	m_DeltaTime = 0;
	
}

void FrameAnimation::ResetAll()
{
	m_CurrentFrame = 0;
	m_CurrentGroup = 0;
	m_DeltaTime = 0;
}

void FrameAnimation::ResetAnim(int group)
{
	m_CurrentFrame=0;
	SetCurrentGroup(group);
	m_DeltaTime = 0;	
		// m_CurrentFrame = (m_CurrentGroup)* m_GroupFrameCount;
}

void FrameAnimation::SetVisible(bool visible)
{
	m_bVisible = visible;
}


void FrameAnimation::Draw(SpriteRenderer* renderer,int posX ,int posY)
{
		m_PosX = posX;
		m_PosY = posY;
		
		if (m_CurrentFrame >= m_Sprites.size()) return;

		auto path = m_Sprites[m_CurrentFrame];
		auto* t = TextureManager::GetInstance()->GetTexture(path);
		renderer->DrawFrameSprite(t,
			glm::vec2(m_PosX, m_PosY),
			glm::vec2(t->GetWidth(), t->GetHeight()), 0.0f, glm::vec3(1.0f, 1.0f, 1.0f));
}

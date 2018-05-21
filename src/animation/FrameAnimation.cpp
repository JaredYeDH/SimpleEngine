#include "FrameAnimation.h"
#include "Engine.h"


FrameAnimation::FrameAnimation()
:FrameAnimation(nullptr)
{
	
}

FrameAnimation::FrameAnimation(Sprite* sprite)
:m_Pos({0,0}),
m_Sprites(0),
m_IsBlankFrame(0),
m_bIsNextFrameRestart(false),
m_LastFrame(-1),
m_LastNotBlankFrame(0),
m_CurrentFrame(0),
m_CurrentGroup (0),
m_GroupFrameCount(0),
m_FrameCount(0),
m_DeltaTime(0),
m_KeyX (0),
m_KeyY(0),
m_Width(0),
m_Height(0),
m_bVisible (true),
m_bLoop (true),
m_FrameTimeBase(0),
m_FrameTime (0)
{
	if (!sprite) return;

	m_GroupFrameCount = sprite->mFrameSize;
	m_FrameCount = sprite->mFrameSize * sprite->mGroupSize;
	
	m_KeyX = sprite->mKeyX;
	m_KeyY = sprite->mKeyY;
	m_Width = sprite->mWidth;
	m_Height = sprite->mHeight;
	
	m_Frames.resize(m_FrameCount);
	for (int i = 0; i < m_FrameCount; i++) {

		m_Frames[i].key_x = sprite->mFrames[i].key_x;
		m_Frames[i].key_y = sprite->mFrames[i].key_y;
		m_Frames[i].width = sprite->mFrames[i].width;
		m_Frames[i].height= sprite->mFrames[i].height;

		String tPath = sprite->mPath + "/" + std::to_string(i);
		uint8_t* img =(uint8_t*) sprite->mFrames[i].src.data();
		if (img) {

			Texture* t = TextureManager::GetInstance()->LoadTexture(
				tPath,
				m_Frames[i].width, m_Frames[i].height, true,
				img
			);
			m_Sprites.push_back(tPath);
			m_IsBlankFrame.push_back(sprite->mFrames[i].IsBlank);
		}
	
		
	}
	m_bVisible = true;
	m_bLoop = true;
	m_FrameTimeBase = 1.0 / 60 * 4 ;
	m_FrameTime = m_FrameTimeBase;
}
void FrameAnimation::ResetFrameTimeByGroupCount(int groupCount)
{
	m_FrameTime = m_FrameTimeBase * groupCount/ m_GroupFrameCount;
	
}
void FrameAnimation::SetFrameTimeBase(double base)
{
	m_FrameTimeBase = base;
	m_FrameTime = m_FrameTimeBase;
}


FrameAnimation& FrameAnimation::operator=(const FrameAnimation& rhs) 
{
	this->m_LastFrame = 0;
	this->m_CurrentFrame = 0;
	this->m_CurrentGroup = 0;
	this->m_GroupFrameCount = rhs.m_GroupFrameCount;
	this->m_FrameCount = rhs.m_FrameCount;
	this->m_DeltaTime = rhs.m_DeltaTime;

	this->m_bIsNextFrameRestart= rhs.m_bIsNextFrameRestart;
	
	this->m_KeyX = rhs.m_KeyX;
	this->m_KeyY = rhs.m_KeyY;
	this->m_Width = rhs.m_Width;
	this->m_Height = rhs.m_Height;

    this->m_Sprites = rhs.m_Sprites;
	this->m_Frames = rhs.m_Frames;
	this->m_IsBlankFrame = rhs.m_IsBlankFrame;
    this->m_bVisible = rhs.m_bVisible;
	this->m_bLoop = rhs.m_bLoop;
	this->m_Pos = rhs.m_Pos;
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

bool FrameAnimation::IsAttackFrame()
{
	int maxWidthIndex=m_CurrentFrame;
	int maxWidth=0;

	for(int i=0;i<m_GroupFrameCount ;i++)
	{	
		int group =  m_CurrentFrame / m_GroupFrameCount;
		int index=group*m_GroupFrameCount + i;
		if(index< m_Sprites.size())
		{
			auto path = m_Sprites[index];	
			auto* t = TextureManager::GetInstance()->GetTexture(path);
			if(t&& t->GetWidth() > maxWidth)
			{
				maxWidth = t->GetWidth();
				maxWidthIndex = index;
			}
		}		
	}
	return m_CurrentFrame == maxWidthIndex;
}


String FrameAnimation::GetFramePath(int index = -1)
{ 
	if ( index  < 0 ) index = m_CurrentFrame;
	if (m_Sprites.size() == 0)return "";
	return  m_Sprites[index];		
}

void FrameAnimation::SetCurrentGroup(int group)
{
	if (m_GroupFrameCount == 0)
	{
		m_CurrentGroup = group;
	}
	else 
	{
		m_CurrentFrame = m_CurrentFrame%m_GroupFrameCount + group*m_GroupFrameCount;
		m_CurrentGroup = group;
	}
	
}


void FrameAnimation::OnUpdate()
{
	double dt = ENGINE_INSTANCE->GetDeltaTime();
	if(m_CurrentFrame >= 0 &&m_CurrentFrame <m_IsBlankFrame.size()&& m_IsBlankFrame[m_CurrentFrame])
		dt = dt * 2;
	m_DeltaTime += dt;
	m_bIsNextFrameRestart = false;
	m_bCurrentFrameChangedInUpdate = false;
	if( m_DeltaTime  >= m_FrameTime)
	{
		m_bCurrentFrameChangedInUpdate = true;
		m_DeltaTime = m_DeltaTime - m_FrameTime;

		m_LastFrame = m_CurrentFrame;
        if(m_LastFrame>=0&& m_LastFrame<m_IsBlankFrame.size()&& !m_IsBlankFrame[m_LastFrame] )
        {
            m_LastNotBlankFrame = m_LastFrame;
        }
		m_CurrentFrame++;
		if(m_GroupFrameCount!=0 && m_CurrentFrame % m_GroupFrameCount == 0 )
		{
			m_bIsNextFrameRestart = true;
			m_LastFrame = (m_CurrentGroup)* m_GroupFrameCount + m_GroupFrameCount - 1;
            if(m_LastFrame>=0&&m_LastFrame<m_IsBlankFrame.size()&& !m_IsBlankFrame[m_LastFrame] )
            {
                m_LastNotBlankFrame = m_LastFrame;
            }
			m_CurrentFrame = (m_CurrentGroup)* m_GroupFrameCount;
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
	m_bIsNextFrameRestart = false;
	// m_CurrentFrame = (m_CurrentGroup)* m_GroupFrameCount;
}

void FrameAnimation::SetVisible(bool visible)
{
	m_bVisible = visible;
}


void FrameAnimation::Draw()
{
	// m_CurrentFrame = m_Sprites.size()/2;
	if (m_CurrentFrame >= m_Sprites.size()) return;
    SpriteRenderer* renderer = SPRITE_RENDERER_INSTANCE;
	int frameToDraw = m_CurrentFrame;
	if(m_LastNotBlankFrame < m_IsBlankFrame.size() && m_IsBlankFrame[m_CurrentFrame] )
	{
		frameToDraw = m_LastNotBlankFrame  ;
	}

	auto path = m_Sprites[frameToDraw];
	auto* t = TextureManager::GetInstance()->GetTexture(path);
	if(t && m_Frames.size()>m_CurrentFrame)
	{
		int kx = (m_KeyX - m_Frames[m_CurrentFrame].key_x);
		int ky = (m_KeyY - m_Frames[m_CurrentFrame].key_y);
		renderer->DrawFrameSprite(t,
			glm::vec2(m_Pos.x + kx, m_Pos.y + ky),
			glm::vec2(t->GetWidth(), t->GetHeight()), 0.0f, glm::vec3(1.0f, 1.0f, 1.0f));
	}
	
}

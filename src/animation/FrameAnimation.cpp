#include "FrameAnimation.h"
#include "Engine.h"


FrameAnimation::FrameAnimation()
:FrameAnimation(nullptr)
{
	
}

FrameAnimation::FrameAnimation(Sprite* sprite)
:m_pSprite(sprite),
m_Pos({0,0}),
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
	if (!m_pSprite) return;

	m_GroupFrameCount = sprite->mFrameSize;
	m_FrameCount = sprite->mFrameSize * sprite->mGroupSize;
	
	m_KeyX = sprite->mKeyX;
	m_KeyY = sprite->mKeyY;
	m_Width = sprite->mWidth;
	m_Height = sprite->mHeight;

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
	this->m_pSprite = rhs.m_pSprite;
	this->m_bIsNextFrameRestart= rhs.m_bIsNextFrameRestart;
	
	this->m_KeyX = rhs.m_KeyX;
	this->m_KeyY = rhs.m_KeyY;
	this->m_Width = rhs.m_Width;
	this->m_Height = rhs.m_Height;

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

String FrameAnimation::GetFramePath(int index)
{
	if(m_pSprite)
	{
		return m_pSprite->mPath.append("/" + std::to_string(index));
	}
	return "";
}
Texture* FrameAnimation::GetFrame(int index = -1)
{ 
	if ( index  < 0  || index >= m_FrameCount) return nullptr;
	
	auto path =  GetFramePath(index);	
	auto* texture= TextureManager::GetInstance()->GetTexture(path);
	if(texture)
	{
		return texture;
	}
	else 
	{
		auto& frame = m_pSprite->mFrames[index];
		return TextureManager::GetInstance()->LoadTexture(path,frame.width,frame.height,true,(uint8_t*)frame.src.data());
	}
}

bool FrameAnimation::IsAttackFrame()
{
	int maxWidthIndex=m_CurrentFrame;
	int maxWidth=0;

	for(int i=0;i<m_GroupFrameCount ;i++)
	{	
		int group =  m_CurrentFrame / m_GroupFrameCount;
		int index =  group*m_GroupFrameCount + i;
		if(index < m_FrameCount)
		{
			auto* texture = GetFrame(index);
			if(texture &&  texture->GetWidth() > maxWidth)
			{
				maxWidth = texture->GetWidth();
				maxWidthIndex = index;
			}
		}
	}
	return m_CurrentFrame == maxWidthIndex;
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
	if(m_CurrentFrame <  0 || m_CurrentFrame >= m_FrameCount || !m_pSprite) return;
	
	double dt = ENGINE_INSTANCE->GetDeltaTime();
	if(m_pSprite->mFrames[m_CurrentFrame].IsBlank)
		dt = dt * 2;
	m_DeltaTime += dt;
	m_bIsNextFrameRestart = false;
	m_bCurrentFrameChangedInUpdate = false;
	if( m_DeltaTime  >= m_FrameTime)
	{
		m_bCurrentFrameChangedInUpdate = true;
		m_DeltaTime = m_DeltaTime - m_FrameTime;

		m_LastFrame = m_CurrentFrame;
        if(!m_pSprite->mFrames[m_LastFrame].IsBlank)
        {
            m_LastNotBlankFrame = m_LastFrame;
        }
		m_CurrentFrame++;
		if(m_GroupFrameCount!=0 && m_CurrentFrame % m_GroupFrameCount == 0 )
		{
			m_bIsNextFrameRestart = true;
			m_LastFrame = (m_CurrentGroup)* m_GroupFrameCount + m_GroupFrameCount - 1;
            if(!m_pSprite->mFrames[m_LastFrame].IsBlank)
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
	if(m_CurrentFrame <  0 || m_CurrentFrame >= m_FrameCount || !m_pSprite) return;
	
	if(m_pSprite->mFrames[m_CurrentFrame].IsBlank)
	{
		m_CurrentFrame = m_LastNotBlankFrame  ;
	}

	auto* texture = GetFrame( m_CurrentFrame);
	if(texture)
	{
		auto& frame = m_pSprite->mFrames[m_CurrentFrame];
		int kx = (m_KeyX - frame.key_x);
		int ky = (m_KeyY - frame.key_y);
		SPRITE_RENDERER_INSTANCE->DrawFrameSprite(texture,
			glm::vec2(m_Pos.x + kx, m_Pos.y + ky),
			glm::vec2(texture->GetWidth(), texture->GetHeight()), 0.0f, glm::vec3(1.0f, 1.0f, 1.0f));
	}
	
}

#include "BaseComponent.h"
#include "../Objects/Object.h"

BaseComponent::BaseComponent()
	:BaseGameEntity()
	, m_pParentObject(nullptr)
	, m_bInitialized(false)
	, m_bIsActive(false)
{
}

BaseComponent::BaseComponent(Object* parent)
	:BaseGameEntity()
	, m_pParentObject(parent)
	, m_bInitialized(false)
	, m_bIsActive(false)
{
}

BaseComponent::~BaseComponent()
{

}

void BaseComponent::Initialize()
{
	if(m_bInitialized)
	{
		//Logger.e("what the fuck")
		return;
	}
	m_bInitialized = true;
	InitializeComponent();
}

void BaseComponent::Destroy()
{
	m_pParentObject->RemoveComponent(this);
}

void BaseComponent::BaseUpdate()
{
	if(m_bIsActive)
	{
		Update();
	}
}

void BaseComponent::BaseDraw()
{
	if(m_bIsActive)
	{
		Draw();
	}
}

bool BaseComponent::IsInitialized() const
{
	return m_bInitialized;
}

Object* BaseComponent::GetParent() const
{
	return m_pParentObject;
}

void BaseComponent::SetParent(Object * parent)
{
	m_pParentObject = parent;
}

void BaseComponent::SetActive(bool active)
{
	m_bIsActive = active;
}

bool BaseComponent::IsActive() const
{
	return m_bIsActive;
}










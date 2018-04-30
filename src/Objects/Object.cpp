#include "Object.h"
#include "../Logger.h"
#include "../Components/TransformComponent.h"

Object::Object()
:Object("")
{
	//m_pComponents.push_back(new TransformComponent());
}


Object::Object(const std::string & name)
: BaseGameEntity(name)
, m_bIsInitialized(false)
, m_IsActive(false)
, m_pParentGameObject(nullptr)
, m_pGarbageContainer()
, m_pComponents()
, m_pChildren()
{
	m_pComponents.push_back(new TransformComponent());
	std::cout << "push_back is called" << std::endl;
}

Object::~Object()
{
	for(auto & info : m_pGarbageContainer)
	{
		DestroyGarbageElement(info);
	}
	m_pGarbageContainer.clear();

	for(auto comp : m_pComponents)
	{
		SafeDelete(comp);
	}
	m_pComponents.clear();

	for(auto child : m_pChildren)
	{
		SafeDelete(child);
	}
	m_pChildren.clear();
}

void Object::Destroy()
{
	if(m_pParentGameObject)
	{
		m_pParentGameObject->RemoveChild(this);
	}
	// else
	// {
	// 	m_pScene->RemoveObject(this);
	// }
}


Object* Object::GetParent() const
{
	return (m_pParentGameObject);
}


void Object::BaseInitialize()
{
	if (m_bIsInitialized)
	{
		return;
	}

	Initialize();

	for(auto comp : m_pComponents)
	{
		if(comp && !comp->IsInitialized())
		{
			comp->Initialize();
		}
	}

	for(auto child : m_pChildren)
	{
		if(child && !child->m_bIsInitialized)
		{
			//child->SetScene(GetScene());
			child->BaseInitialize();
		}
	}
	BaseAfterInitialized();
	m_bIsInitialized = true;
}


Object::GarbageInfo::GarbageInfo(
	BaseGameEntity* pEntity,
	GarbageType type
	)
: element(pEntity)
, type(type)
{
}

void Object::DestroyGarbageElement(const GarbageInfo & info)
{
	switch(info.type)
	{
		case GarbageType::ActionType:
		{
			// auto action = dynamic_cast<Action*>(info.element);
			// auto it = std::find(m_pActions.begin(), m_pActions.end(), action);
			// m_pActions.erase(it);
		}
		break;
		case GarbageType::ObjectType:
		{
			auto object = dynamic_cast<Object*>(info.element);
			auto it = std::find(m_pChildren.begin(), m_pChildren.end(), object);
			m_pChildren.erase(it);
		}
		break;
		case GarbageType::ComponentType:
		{
			auto component = dynamic_cast<BaseComponent*>(info.element);
			auto it = std::find(m_pComponents.begin(), m_pComponents.end(), component);
			m_pComponents.erase(it);
			//RecalculateDimensions();
		}
		break;
	}
	delete info.element;
}

void Object::Initialize()
{
}

void Object::BaseAfterInitialized()
{
	AfterInitialized();
}

void Object::AfterInitialized()
{
}

void Object::Update()
{

}

void Object::BaseUpdate()
{
	CollectGarbage();
	if(m_IsActive)
	{
		Update();

		// for(auto action : m_pActions)
		// {
		// 	if(action)
		// 	{
		// 		action->BaseUpdate();
		// 	}
		// 	else
		// 	{
		// 		m_pActions.erase(
		// 			std::find(
		// 				m_pActions.begin(),
		// 				m_pActions.end(),
		// 				action)
		// 			);
		// 		LOG(LogLevel::Warning,
		// 			tstring(_T("Object::BaseUpdate: ")) +
		// 			_T("Trying to update nullptr action from object '")
		// 			+ GetName() + _T("'."), STARENGINE_LOG_TAG);
		// 	}
		// }

		for(auto component : m_pComponents)
		{
			if(component)
			{
				component->BaseUpdate();
			}
			else
			{
				m_pComponents.erase(
					std::find(
						m_pComponents.begin(),
						m_pComponents.end(),
						component)
					);
				//Logger::Warning(std::string("Object::BaseUpdate: ") + "Trying to update nullptr component from object '" + + GetName() + ("'."));
			}
		}

		for(auto child : m_pChildren)
		{
			if(child)
			{
				child->BaseUpdate();
			}
			else
			{
				m_pChildren.erase(
					std::find(
						m_pChildren.begin(),
						m_pChildren.end(),
						child)
					);
				//Logger::Warning(std::string("Object::BaseUpdate: ") + "Trying to update nullptr child from object '" + + GetName() + ("'."));
			}
		}
	}
}

void Object::BaseDraw()
{
	if(m_IsActive)
	{
		Draw();

		for(auto component : m_pComponents)
		{
			if(component)
			{
				component->BaseDraw();
			} 
			else
			{
				//Logger::Warning(std::string("Object::BaseDraw: ") + "Trying to draw nullptr component from object '" + + GetName() + ("'."));
			}
		}

		for(auto child : m_pChildren)
		{
			if(child)
			{
				child->BaseDraw();
			}
			else
			{
				//Logger::Warning(std::string("Object::BaseDraw: ") + "Trying to draw nullptr object child from object '" + + GetName() + ("'."));
			}
		}
	}
}

void Object::Draw()
{
}

void Object::SetActive(bool active)
{
	m_IsActive = active;
}

bool Object::IsActive() const
{
	return m_IsActive;
}

bool Object::IsInitialized() const
{
	return m_bIsInitialized;
}

// bool Object::BaseCheckCulling(
// 	float32 left, float32 right, float32 top, float32 bottom
// 	)
// {
// 	for ( auto component : m_pComponents)
// 	{
// 		if(component->CheckCulling(left, right, top, bottom))
// 		{
// 			return true;
// 		}
// 	}

// 	return CheckCulling(left, right, top, bottom);
// }

// bool Object::CheckCulling(
// 	float32 left,
// 	float32 right,
// 	float32 top,
// 	float32 bottom
// 	)
// {
// 	return false;
// }


void Object::Reset()
{
	for(auto child : m_pChildren)
	{
		child->Reset();
	}
}

TransformComponent * Object::GetTransform() const
{
	return GetComponent<TransformComponent>();
}

void Object::AddComponent(BaseComponent *pComponent)
{
	// for(auto comp : m_pComponents)
	// {
	// 	ASSERT_LOG(typeid(*comp) != typeid(*pComponent), 
	// 		_T("Object::AddComponent: \
	// 			Adding 2 components of the same type \
	// 			to the same object is illegal."), STARENGINE_LOG_TAG);
	// }

	// pComponent->SetParent(this);

	if(m_bIsInitialized && ! pComponent->IsInitialized())
	{
		pComponent->Initialize();
	}

	m_pComponents.push_back(pComponent);
}	


void Object::RemoveComponent(BaseComponent * pComponent)
{
	auto it = std::find(m_pComponents.begin(), m_pComponents.end(), pComponent);
	bool isOK = it != m_pComponents.end();
	// ASSERT_LOG(isOK,
	// 	_T("Object::RemoveComponent: The component you tried \
	// 		to remove could not be found."), STARENGINE_LOG_TAG);
	if(isOK)
	{
		m_pGarbageContainer.push_back(
			GarbageInfo(
				*it,
				GarbageType::ComponentType
				)
			);
	}
}

const std::vector<BaseComponent*>& Object::GetComponents() const
{
	return m_pComponents;
}

void Object::AddChild(Object *pChild)
{
	pChild->m_pParentGameObject = this;

	// if(IsChildNameAlreadyInUse(pChild->GetName()))
	// {
	// 	DEBUG_LOG(LogLevel::Warning,
	// 		_T("Object::AddChild: a child with the name '")
	// 		+ pChild->GetName() + _T("' already exists. \
	// 			Child gets added but beware, duplicate names can become the cause of problems."),
	// 		STARENGINE_LOG_TAG);
	// }

	if(m_bIsInitialized && !pChild->m_bIsInitialized)
	{
		//pChild->SetScene(GetScene());
		pChild->BaseInitialize();
	}

	m_pChildren.push_back(pChild);
}

void Object::RemoveChild(const Object* pObject)
{
	auto it = std::find(m_pChildren.begin(), m_pChildren.end(), pObject);
	bool isOK = it != m_pChildren.end();
	if(isOK)
	{
		m_pGarbageContainer.push_back(
			GarbageInfo(
				*it,
				GarbageType::ObjectType
				)
			);
	}
	else
	{
		//Logger::Error("Object::RemoveChild: The object you tried to remove is not a child of this object!");
	}
}

void Object::RemoveChild(const std::string & name)
{
	for(auto child : m_pChildren)
	{
		if(child->GetName()==name)
		{
			RemoveChild(child);
			return;
		}
	}
	//Logger::Error("Object::RemoveChild: The object you tried to remove is not a child of this object!");
}


const std::vector<Object*>& Object::GetChildren() const
{
	return m_pChildren;
}


// bool BaseCheckCulling(float32 left,float32 right,float32 top,float32 bottom);

// virtual bool CheckCulling(
// 	float32 left,
// 	float32 right,
// 	float32 top,
// 	float32 bottom
// 	);


void Object::CollectGarbage()
{
	for(auto & info : m_pGarbageContainer)
	{
		//Logger::Warning("Object::CollectGarbage: " + "Object::CollectGarbage: Removed entity '" + info.element->GetName() + ("'."));		
		DestroyGarbageElement(info);
	}
	m_pGarbageContainer.clear();
}


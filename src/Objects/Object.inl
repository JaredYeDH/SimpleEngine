#pragma once

#include "Object.h"
#include "../Logger.h"
#include <typeinfo>

template <typename T>
T* Object::GetChildByName(const std::string & name)
{
	for(auto child : m_pChildren)
	{
		if(child->GetName() == name)
		{
			auto returnobject = dynamic_cast<T*>(child);
			if(returnobject == nullptr)
			{
				Logger::Error("Object::GetChildByName: couldn't convert object '"
					+ name + "' to the requested type. Returning nullptr...");
			}
			return returnobject;
		}
	}

	Logger::Error("Object::GetChildByName: Trying to get unknown child '" + name + ("'."));
	return nullptr;
}

template <typename T>
void Object::RemoveComponent()
{
	const std::type_info& ti = typeid(T);
	for(auto component : m_pComponents)
	{
		if(component && typeid(*component) == ti)
		{
			m_pGarbageContainer.push_back(
				GarbageInfo(
					component,
					GarbageType::ComponentType
					)
				);
		}
	}	
}

template <typename T>
T* Object::GetComponent(bool searchChildren) const
{
	const std::type_info& ti = typeid(T);
	for(auto component : m_pComponents)
	{
		if(component && typeid(*component) == ti)
		{
			return dynamic_cast<T*>(component);
		}
	}

	if(searchChildren)
	{
		for(auto child : m_pChildren)
		{
			return (child->GetComponent<T>(searchChildren));
		}
	}
	return nullptr;
}

template <typename T>
T* Object::GetChild() const
{
	const std::type_info& ti = typeid(T);
	for(auto child : m_pChildren)
	{
		if(child && typeid(*child) == ti)
		{
			return (dynamic_cast<T*>(child));
		}
	}
	return nullptr;
}

template <typename T>
T* Object::GetChild(const std::string & name) const
{
	const std::type_info& ti = typeid(T);
	for(auto child : m_pChildren)
	{
		if(child && typeid(*child) == ti
			&& child->GetName() == name)
		{
			return dynamic_cast<T*>(child);
		}
	}
	return nullptr;
}


template <typename T>
bool Object::HasComponent(BaseComponent * component) const
{
	const std::type_info& ti = typeid(T);
	for(auto comp : m_pComponents)
	{
		if(comp != component &&
			typeid(*comp) == ti)
		{
			return true;
		}
	}
	return false;
}


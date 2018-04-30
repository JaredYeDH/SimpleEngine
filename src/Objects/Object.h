#pragma once

#include "../Entity.h"
#include "../Components/BaseComponent.h"
#include "../Components/TransformComponent.h"

class BaseComponent;

class Object : public BaseGameEntity
{
public:
	Object();
	Object(const std::string & name);
	virtual ~Object();

	void Destroy();

	Object* GetParent() const; 

	void BaseInitialize();
	void BaseAfterInitialized();
	void BaseUpdate();
	void BaseDraw();
	void SetActive(bool active);
	bool IsActive() const;

	bool IsInitialized() const;

	TransformComponent * GetTransform() const;

	void AddComponent(BaseComponent* pComponent);
	void RemoveComponent(BaseComponent* pComponent);

	template <typename T>
	T * GetChildByName(const std::string & name);

	template <typename T>
	void RemoveComponent();

	template <typename T>
	T* GetComponent(bool searchChildren = false) const;

	const std::vector<BaseComponent*>& GetComponents() const;

	virtual void AddChild(Object * pObject);
	void RemoveChild(const Object* pObject);
	void RemoveChild(const std::string & name);

	const std::vector<Object*>& GetChildren() const;

	template <typename T>
	T* GetChild() const;

	template <typename T>
	T* GetChild(const std::string & name) const;

	template <typename T>
	bool HasComponent(BaseComponent * component) const;

protected:
	enum class GarbageType : uint8
	{
		ComponentType = 0,
		ObjectType  = 1,
		ActionType = 2
	};

	struct GarbageInfo
	{
		GarbageInfo(BaseGameEntity * pEntity, GarbageType type);
		BaseGameEntity * element;
		GarbageType type;
	};

	void DestroyGarbageElement(const GarbageInfo & info);

	virtual void Reset();
	virtual void Initialize();
	virtual void AfterInitialized();
	virtual void Update();
	virtual void Draw();

	// bool BaseCheckCulling(float32 left,float32 right,float32 top,float32 bottom);

	// virtual bool CheckCulling(
	// 	float32 left,
	// 	float32 right,
	// 	float32 top,
	// 	float32 bottom
	// 	);

	bool m_bIsInitialized;
	bool m_IsActive;

	Object * m_pParentGameObject;

	std::vector<GarbageInfo> m_pGarbageContainer;

	std::vector<BaseComponent*> m_pComponents;
	std::vector<Object*> m_pChildren;

private:
	void CollectGarbage();

	Object(const Object& t);
	Object(Object&& t);
	Object& operator=(const Object& t);
	Object& operator=(Object&& t);
};

#include "Object.inl"

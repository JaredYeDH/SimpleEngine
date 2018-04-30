#pragma once

#include "Entity.h"

class Object;

class BaseComponent : public BaseGameEntity
{
public:
	BaseComponent();

	BaseComponent(Object* parent);
	
	virtual ~BaseComponent();

	void Initialize();

	bool IsInitialized() const;

	void Destroy();

	void BaseUpdate();

	void BaseDraw();

	virtual void Update() = 0;

	virtual void Draw() = 0;

	Object* GetParent() const;
 	void SetParent(Object* parent);

	void SetActive(bool isActive);

	bool IsActive() const;

protected:
	virtual void InitializeComponent() = 0;

	Object * m_pParentObject;
	bool m_bInitialized,
		 m_bIsActive;

private:
	BaseComponent(const BaseComponent& t);
	BaseComponent(BaseComponent&& t);
	BaseComponent& operator=(const BaseComponent& t);
	BaseComponent& operator=(BaseComponent&& t);	
};

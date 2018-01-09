#pragma once

#include "BaseComponent.h"

class Object;

class TransformComponent final : public BaseComponent
{
public:
	enum TransformChanged : uint8
	{
		NONE = 0,
		TRANSLATION = 1,
		ROTATION = 2,
		SCALE = 4,
		ALL = 7
	};

	TransformComponent(); 
	TransformComponent(Object* parent); 
	~TransformComponent();

	void Update();

	void Draw();

	void IsChanged(bool isChanged);

	void Translate(const glm::vec3& tranlation);
	void Translate(float32 x, float32 y , float32 z = 0.0f);
	void TranslateX(float32 x);
	void TranslateY(float32 y);
	void TranslateZ(float32 z);

	void Move(const glm::vec3& translation);
	void Move(float32 x, float32 y, float32 z);
	void MoveX(float32 x);
	void MoveY(float32 y);
	void MoveZ(float32 z);

	void Rotate(const glm::vec3& rotation);
	void Rotate(float32 x, float32 y, float32 z);
	void Rotate(const glm::vec3& rotation, const glm::vec3& centerPoint);
	void RotateX(float32 rotation);
	void RotateY(float32 rotation);
	void RotateZ(float32 rotation);

	void Scale(const glm::vec3& scale);
	void Scale(float32 x, float32 y, float32 z);
	void ScaleX(float32 x);
	void ScaleY(float32 y);
	void ScaleZ(float32 z);

	// void Mirror(bool x, bool y, bool z);
	// void MirrorX(bool x);
	// void MirrorY(bool y);
	// void MirrorZ(bool z);

	const glm::vec3& GetWorldPosition();
	const glm::vec3& GetLocalPosition();
	const glm::vec3& GetWorldRotation();
	const glm::vec3& GetLocalRotation();
	const glm::vec3& GetWorldScale();
	const glm::vec3& GetLocalScale();

	void SetCenterPoint(const glm::vec3 & centerPoint);
	void SetCenterPoint(float32 x, float32 y, float32 z);
	void SetCenterX(float32 x);
	void SetCenterY(float32 y);
	void SetCenterZ(float32 z);

	const glm::mat4 & GetWorldMatrix() const;

private:
	void InitializeComponent();
	void CheckForUpdate(const bool force = false);
	void CommonUpdate();
	void SingleUpdate(glm::mat4 & world);

	uint8 m_IsChanged;
	bool m_Invalidate;


	glm::vec3 m_WorldPosition, m_LocalPosition;
	glm::vec3 m_WorldRotation , m_LocalRotation;
	glm::vec3 m_WorldScale, m_LocalScale;

	glm::vec3 m_CenterPosition;
	
	glm::mat4 m_World;

	TransformComponent(const TransformComponent& yRef);
	TransformComponent(TransformComponent&& yRef);
	TransformComponent& operator=(const TransformComponent& yRef);
	TransformComponent& operator=(TransformComponent&& yRef);

};
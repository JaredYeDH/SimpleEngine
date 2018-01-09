#include "TransformComponent.h"

TransformComponent::TransformComponent(Object* parent)
:BaseComponent(parent)
,m_IsChanged(TransformComponent::ALL)
,m_Invalidate(false)
,m_WorldPosition(0,0,0)
,m_LocalPosition(0,0,0)
,m_WorldRotation(0,0,0)
,m_LocalRotation(0,0,0)
,m_WorldScale(1,1,1)
,m_LocalScale(1,1,1)
,m_CenterPosition(0,0,0)
,m_World()
{

}

TransformComponent::TransformComponent()
:TransformComponent(nullptr)
{

}


TransformComponent::~TransformComponent()
{
	
}

void TransformComponent::Translate(const glm::vec3 & translation)
{
	m_LocalPosition.x = translation.x;
	m_LocalPosition.y = translation.y;
	m_LocalPosition.z = translation.z;
	m_IsChanged |= TransformChanged::TRANSLATION;
}

void TransformComponent::Translate(float32 x, float32 y, float32 z)
{
	Translate(glm::vec3(x, y, z));
}

void TransformComponent::TranslateX(float32 x)
{
	m_LocalPosition.x = x;
	m_IsChanged |= TransformChanged::TRANSLATION;
}

void TransformComponent::TranslateY(float32 y)
{
	m_LocalPosition.y = y;
	m_IsChanged |= TransformChanged::TRANSLATION;
}

void TransformComponent::TranslateZ(float32 z)
{
	m_LocalPosition.z = z;
	m_IsChanged |= TransformChanged::TRANSLATION;
}

void TransformComponent::Move(const glm::vec3& translation)
{
	m_LocalPosition.x += translation.x;
	m_LocalPosition.y += translation.y;
	m_LocalPosition.z += translation.z;
	m_IsChanged |= TransformChanged::TRANSLATION;
}

void TransformComponent::Move(float32 x, float32 y, float32 z)
{
	m_LocalPosition.x += x;
	m_LocalPosition.y += y;
	m_LocalPosition.z += z;
	m_IsChanged |= TransformChanged::TRANSLATION;
}

void TransformComponent::MoveX(float32 x)
{
	m_LocalPosition.x += x;
	m_IsChanged |= TransformChanged::TRANSLATION;
}

void TransformComponent::MoveY(float32 y)
{
	m_LocalPosition.y += y;
	m_IsChanged |= TransformChanged::TRANSLATION;
}

void TransformComponent::MoveZ(float32 z)
{
	m_LocalPosition.z += z;
	m_IsChanged |= TransformChanged::TRANSLATION;
}

void TransformComponent::Rotate(const glm::vec3& rotation)
{
	m_LocalRotation.x = rotation.x;
	m_LocalRotation.y = rotation.y;
	m_LocalRotation.z = rotation.z;
	m_IsChanged |= TransformChanged::ROTATION;
}

void TransformComponent::Rotate(const glm::vec3 & rotation, const glm::vec3& centerPoint)
{
	m_LocalRotation.x = rotation.x;
	m_LocalRotation.y = rotation.y;
	m_LocalRotation.z = rotation.z;
	SetCenterPoint(centerPoint);
	m_IsChanged |= TransformChanged::ROTATION;
}

void TransformComponent::Rotate(float32 x,float32 y, float32 z)
{
	m_LocalRotation.x = x;
	m_LocalRotation.y = y;
	m_LocalRotation.z = z;
	m_IsChanged |= TransformChanged::ROTATION;
}

void TransformComponent::RotateX(float32 rotation)
{
	m_LocalRotation.x = rotation;
	m_IsChanged |= TransformChanged::ROTATION;
}

void TransformComponent::RotateY(float32 rotation)
{
	m_LocalRotation.y = rotation;
	m_IsChanged |= TransformChanged::ROTATION;
}

void TransformComponent::RotateZ(float32 rotation)
{
	m_LocalRotation.z = rotation;
	m_IsChanged |= TransformChanged::ROTATION;
}


void TransformComponent::Scale(const glm::vec3 & scale)
{
	m_LocalScale = scale;
	m_IsChanged |= TransformChanged::SCALE;
}

void TransformComponent::Scale(float32 x, float32 y, float32 z)
{
	Scale(glm::vec3(x, y, z));
}

void TransformComponent::ScaleX(float32 x)
{
	m_LocalScale.x = x;
	m_IsChanged |= TransformChanged::SCALE;
}

void TransformComponent::ScaleY(float32 y)
{
	m_LocalScale.y = y;
	m_IsChanged |= TransformChanged::SCALE;
}

void TransformComponent::ScaleZ(float32 z)
{
	m_LocalScale.z = z;
	m_IsChanged |= TransformChanged::SCALE;
}

const glm::vec3 & TransformComponent::GetWorldPosition()
{
	return m_WorldPosition;
}

const glm::vec3 & TransformComponent::GetLocalPosition()
{
	return m_LocalPosition;
}

const glm::vec3 & TransformComponent::GetWorldRotation()
{
	return m_WorldRotation;
}

const glm::vec3 & TransformComponent::GetLocalRotation()
{
	return m_LocalRotation;
}

const glm::vec3 & TransformComponent::GetWorldScale()
{
	return m_WorldScale;
}

const glm::vec3 & TransformComponent::GetLocalScale()
{
	return m_LocalScale;
}

void TransformComponent::SetCenterPoint(const glm::vec3 & centerPoint)
{
	m_CenterPosition = centerPoint;
}

void TransformComponent::SetCenterPoint(float32 x, float32 y, float32 z)
{
	m_CenterPosition.x = x;
	m_CenterPosition.y = y;
	m_CenterPosition.z = z;
}

void TransformComponent::SetCenterX(float32 x)
{
	m_CenterPosition.x = x;
}

void TransformComponent::SetCenterY(float32 y)
{
	m_CenterPosition.y = y;
}

void TransformComponent::SetCenterZ(float32 z)
{
	m_CenterPosition.z = z;
}

const glm::mat4 & TransformComponent::GetWorldMatrix() const
{
	return m_World;
}

void TransformComponent::CheckForUpdate(bool force)
{
	if(m_IsChanged == TransformChanged::NONE && !force && !m_Invalidate)
	{
		return;
	}

	CommonUpdate();

	m_IsChanged = TransformChanged::NONE;

	m_Invalidate = false;
}

void TransformComponent::CommonUpdate()
{
	// for(auto child : GetParent()->GetChildren())
	// {
	// 	child->GetTransform()->IsChanged(true);
	// }

	SingleUpdate(m_World);

	// auto parent = m_pParentObject->GetParent();
	// if(parent != nullptr)
	// {
	// 	m_World = parent->GetTransform()->GetWorldMatrix() * m_World;
	// }

	//DecomposeMatrix(m_World, m_WorldPosition, m_WorldScale, m_WorldRotation);

}

void TransformComponent::SingleUpdate(glm::mat4 & world)
{
	//glm::mat4 matRot, matTrans, matScale, matC, matCI;

	// matTrans = glm::translate(m_LocalPosition.pos3D());
	// matRot   = ToMat4(quat(vec3(0, 0, m_LocalRotation)));
	// matScale = star::Scale(vec3(m_LocalScale.x, m_LocalScale.y, 1.0f));

	// vec3 centerPos(m_CenterPosition.x, m_CenterPosition.y, 0);
	// matC = star::Translate(-centerPos);

	// world = matTrans * matRot * matScale * matC;

	// if(m_IsMirroredX || m_IsMirroredY)
	// {
	// 	world *= star::Translate(
	// 		m_Dimensions.x / 2.0f,
	// 		m_Dimensions.y / 2.0f,
	// 		0
	// 		);

	// 	if(m_IsMirroredX)
	// 	{
	// 		if(m_IsMirroredY)
	// 		{
	// 			world *= star::Scale(vec3(-1,-1,1));
	// 		}
	// 		else
	// 		{
	// 			world *= star::Scale(vec3(-1,1,1));
	// 		}
	// 	}
	// 	else
	// 	{
	// 		world *= star::Scale(vec3(1,-1,1));
	// 	}

	// 	world *= star::Translate(
	// 		m_Dimensions.x / -2.0f,
	// 		m_Dimensions.y / -2.0f,
	// 		0
	// 		);
	// }
}

void TransformComponent::Update()
{
	CheckForUpdate();
}

void TransformComponent::Draw()
{

}

void TransformComponent::IsChanged(bool isChanged)
{
	m_IsChanged = isChanged;
}

void TransformComponent::InitializeComponent()
{
	CheckForUpdate(true);
	m_Invalidate = true;
}











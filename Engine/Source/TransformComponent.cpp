#include "TransformComponent.h"

namespace MarkTech
{
	CTransformComponent::CTransformComponent()
	{
	}

	CTransformComponent::CTransformComponent(uint64_t ownerId)
		:CBaseComponent::CBaseComponent(ownerId)
	{
		m_Transform = MTransform();
	}

	CTransformComponent::~CTransformComponent()
	{
	}

	void CTransformComponent::InitComponent()
	{
	}

	void CTransformComponent::UpdateComponent(float flDeltaTime)
	{
	}

	void CTransformComponent::SetPosition(MVector3 pos)
	{
		m_Transform.Position = pos;
	}

	void CTransformComponent::SetRotation(MRotator rot)
	{
		m_Transform.Rotation = rot;
	}

	void CTransformComponent::SetScale(MVector3 scale)
	{
		m_Transform.Scale = scale;
	}

	/*const MVector3& CTransformComponent::GetForwardVector()
	{
		// TODO: insert return statement here
	}

	const MVector3& CTransformComponent::GetRightVector()
	{
		// TODO: insert return statement here
	}*/
}
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
	MVector3 CTransformComponent::GetForwardVector()
	{
		DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(m_Transform.Rotation.Roll, m_Transform.Rotation.Pitch, m_Transform.Rotation.Yaw);
		DirectX::XMVECTOR tempForward = DirectX::XMVector3TransformCoord(DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f), rotationMatrix);
		tempForward = DirectX::XMVector3Normalize(tempForward);
		return MVector3(DirectX::XMVectorGetX(tempForward), DirectX::XMVectorGetY(tempForward), DirectX::XMVectorGetZ(tempForward));
	}
	MVector3 CTransformComponent::GetRightVector()
	{
		DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(m_Transform.Rotation.Roll, m_Transform.Rotation.Pitch, m_Transform.Rotation.Yaw);
		DirectX::XMVECTOR tempRight = DirectX::XMVector3TransformCoord(DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), rotationMatrix);
		tempRight = DirectX::XMVector3Normalize(tempRight);
		return MVector3(DirectX::XMVectorGetX(tempRight), DirectX::XMVectorGetY(tempRight), DirectX::XMVectorGetZ(tempRight));
	}
}
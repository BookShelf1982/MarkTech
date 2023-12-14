#include "CameraComponent.h"
#include "Level.h"

namespace MarkTech
{
	CCameraComponent::CCameraComponent()
	{
	}

	CCameraComponent::CCameraComponent(uint64_t ownerId)
		:CBaseComponent::CBaseComponent(ownerId)
	{
		m_CamData = {};
	}

	CCameraComponent::~CCameraComponent()
	{
	}

	void CCameraComponent::InitComponent()
	{
		if (GetLevel()->HasComponent<CTransformComponent>(m_nOwnerId))
			GetLevel()->GetComponentFromEntity<CTransformComponent>(m_nOwnerId)->SetPosition(MVector3(-50.0f, 0.0f, 10.0f));

		m_CamData.flNearZ = 0.1f;
		m_CamData.flFarZ = 10000.0f;
		m_CamData.flFov = 0.04f;
		m_CamData.camTarget = MVector3(0.0f, 0.0f, 2000.0f);
	}

	void CCameraComponent::UpdateComponent(float flDeltaTime)
	{
		if (GetLevel()->HasComponent<CTransformComponent>(m_nOwnerId))
		{
			CTransformComponent* comp =  GetLevel()->GetComponentFromEntity<CTransformComponent>(m_nOwnerId);
			m_CamData.camPos = comp->GetPosition();

			CD3D11Renderer::GetD3DRenderer()->UpdateCameraData(m_CamData);
		}
	}
}
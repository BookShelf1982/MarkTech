#include "CameraComponent.h"
#include "Level.h"

namespace MarkTech
{
	CCameraComponent::CCameraComponent()
	{
	}

	CCameraComponent::~CCameraComponent()
	{
	}

	void CCameraComponent::InitComponent()
	{
	}

	void CCameraComponent::UpdateComponent(float flDeltaTime)
	{
		if (GetLevel()->HasComponent<CTransformComponent>(m_nOwnerId))
		{
			CTransformComponent* comp =  GetLevel()->GetComponentFromEntity<CTransformComponent>(m_nOwnerId);

		}
	}
}
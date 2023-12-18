#include "CameraComponent.h"
#include "Level.h"
#include "ModelComponent.h"

namespace MarkTech
{
	CCameraComponent::CCameraComponent()
	{
	}

	CCameraComponent::CCameraComponent(uint64_t ownerId)
		:CBaseComponent::CBaseComponent(ownerId)
	{
		m_CamData = {};
		bIsSpaceDown = false;
	}

	CCameraComponent::~CCameraComponent()
	{
	}

	void CCameraComponent::InitComponent()
	{
		m_CamData.flNearZ = 0.1f;
		m_CamData.flFarZ = 10000.0f;
		m_CamData.flFov = 0.35f;
	}

	void CCameraComponent::UpdateComponent(float flDeltaTime)
	{
		if (GetLevel()->HasComponent<CTransformComponent>(m_nOwnerId))
		{
			CTransformComponent* comp = GetLevel()->GetComponentFromEntity<CTransformComponent>(m_nOwnerId);

			if (CInput::GetInput()->IsKeyDown(MTVK_W))
			{
				comp->SetPosition(comp->GetPosition() + comp->GetForwardVector() * (10.0f * flDeltaTime));
			}
			if (CInput::GetInput()->IsKeyDown(MTVK_S))
			{
				comp->SetPosition(comp->GetPosition() + comp->GetForwardVector() * (-10.0f * flDeltaTime));
			}
			if (CInput::GetInput()->IsKeyDown(MTVK_Left))
			{
				comp->SetRotation(MRotator(0.0f, comp->GetRotation().Yaw - 4.25f * flDeltaTime, 0.0f));
			}
			if (CInput::GetInput()->IsKeyDown(MTVK_Right))
			{
				comp->SetRotation(MRotator(0.0f, comp->GetRotation().Yaw + 4.25f * flDeltaTime, 0.0f));
			}
			if (CInput::GetInput()->IsKeyDown(MTVK_D))
			{
				comp->SetPosition(comp->GetPosition() + comp->GetRightVector() * (10.0f * flDeltaTime));
			}
			if (CInput::GetInput()->IsKeyDown(MTVK_A))
			{
				comp->SetPosition(comp->GetPosition() + comp->GetRightVector() * (-10.0f * flDeltaTime));
			}
			if (CInput::GetInput()->IsKeyDown(MTVK_E))
			{
				comp->SetPosition(comp->GetPosition() + MVector3(0.0f, 0.0f, 1.0f)*10.f*flDeltaTime);
			}
			if (CInput::GetInput()->IsKeyDown(MTVK_Q))
			{
				comp->SetPosition(comp->GetPosition() + MVector3(0.0f, 0.0f, -1.0f) * 10.f*flDeltaTime);
			}
			if (CInput::GetInput()->IsButtonDown(MTVM_Mouse1))
			{
				m_CamData.flFov += 0.1f * flDeltaTime;
			}
			if (CInput::GetInput()->IsButtonDown(MTVM_Mouse2))
			{
				m_CamData.flFov -= 0.1f * flDeltaTime;
			}
			if (CInput::GetInput()->IsKeyDown(MTVK_Space) && bIsSpaceDown == false)
			{
  				OutputDebugStringA("Hello");
				bIsSpaceDown = true;
				uint64_t mdlentid = GetLevel()->CreateEntity();
				GetLevel()->CreateComponent<CTransformComponent>(mdlentid);
				GetLevel()->GetComponentFromEntity<CTransformComponent>(mdlentid)->SetPosition(comp->GetPosition());
				GetLevel()->GetComponentFromEntity<CTransformComponent>(mdlentid)->SetRotation(comp->GetRotation());
				GetLevel()->CreateComponent<CModelComponent>(mdlentid);
			}
			if (CInput::GetInput()->IsKeyUp(MTVK_Space))
			{
				bIsSpaceDown = false;
			}


			if (GetLevel()->HasComponent<CTransformComponent>(m_nOwnerId))
			{
				m_CamData.camPos = comp->GetPosition();
				m_CamData.camTarget = comp->GetForwardVector();
				CD3D11Renderer::GetD3DRenderer()->UpdateCameraData(m_CamData);
			}
		}
	}
}
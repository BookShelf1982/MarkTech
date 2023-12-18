#pragma once
#include "Core.h"
#include "D3D11Renderer.h"
#include "BaseComponent.h"
#include "TransformComponent.h"

namespace MarkTech
{
	class CCameraComponent : public CBaseComponent
	{
	public:
		CCameraComponent();
		CCameraComponent(uint64_t ownerId);
		~CCameraComponent();

		virtual void InitComponent() override;
		virtual void UpdateComponent(float flDeltaTime) override;

	protected:
		MCameraData m_CamData;
		bool bIsSpaceDown;
	};
}
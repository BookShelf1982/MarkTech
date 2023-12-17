#pragma once
#include "Core.h"
#include "BaseComponent.h"
#include "TransformComponent.h"
#include "D3D11Renderer.h"

namespace MarkTech
{
	class CModelComponent : public CBaseComponent
	{
	public:
		CModelComponent();
		CModelComponent(uint64_t ownerId);
		~CModelComponent();

		virtual void InitComponent() override;
		virtual void UpdateComponent(float flDeltaTime) override;

	protected:
		const char* m_pszAssetPath;
		CAssetHandle m_Model;
	};
}
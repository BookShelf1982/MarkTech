#include "ModelComponent.h"

namespace MarkTech
{
	CModelComponent::CModelComponent()
		:m_pszAssetPath("")
	{
	}


	CModelComponent::CModelComponent(uint64_t ownerId)
		:CBaseComponent::CBaseComponent(ownerId), m_pszAssetPath("teapot.mmdl")
	{
	}

	CModelComponent::~CModelComponent()
	{
	}

	void CModelComponent::InitComponent()
	{
		m_Model = GetLevel()->LoadAsset(m_pszAssetPath, MModel);
	}

	void CModelComponent::UpdateComponent(float flDeltaTime)
	{
		if (dynamic_cast<CModel*>(m_Model.GetAssetDataPtr()) != nullptr)
		{
			//GetD3DRenderer()->SubmitModel(dynamic_cast<CModel*>(m_Model.GetAssetDataPtr()));
			GetD3DRenderer()->SubmitModel(dynamic_cast<CModel*>(m_Model.GetAssetDataPtr()));
		}
	}
}
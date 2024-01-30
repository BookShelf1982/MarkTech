#include "Level.h"
#include <random>

template<> size_t CLevel::GetIndexOfComponentFromVesselId<MTransformComponent>(uint64_t vesselId)
{
	for (size_t i = 0; i < m_TransformComponents.GetActivatedSize(); i++)
	{
		if (m_TransformComponents.GetElement(i).nOwnerId == vesselId)
		{
			return i;
		}
	}
	return -1;
}

template<> size_t CLevel::GetIndexOfComponentFromVesselId<MModelComponent>(uint64_t vesselId)
{
	for (size_t i = 0; i < m_ModelComponents.GetActivatedSize(); i++)
	{
		if (m_ModelComponents.GetElement(i).nOwnerId == vesselId)
		{
			return i;
		}
	}
	return -1;
}

CLevel::CLevel()
{
}

CLevel::~CLevel()
{
}

bool CLevel::InitLevel()
{
	m_Vessels.InitPool(MAX_VESSELS);
	m_ModelComponents.InitPool(MAX_COMPONENTS);
	m_TransformComponents.InitPool(MAX_COMPONENTS);
	return true;
}

void CLevel::UpdateLevel(float flDeltaTime)
{
	//m_SceneBuffer.Initialize(m_ModelComponents.GetActivatedSize());

	for (size_t i = 0; i < m_ModelComponents.GetActivatedSize(); i++)
	{
		MSceneElement element;
		MModelComponent comp = m_ModelComponents.GetElement(i);
		size_t compIndex = GetIndexOfComponentFromVesselId<MTransformComponent>(comp.nOwnerId);
		MTransformComponent transformComp = m_TransformComponents.GetElement(compIndex);

		element.assetId = comp.nAssetId;
		element.type = ESceneElementType::Model;
		element.transform = DirectX::XMMatrixTranslation(transformComp.Transform.Position.y, transformComp.Transform.Position.z, transformComp.Transform.Position.x);
		//m_SceneBuffer.PushSceneElement(element);
	}
}

void CLevel::DestroyLevel()
{
	m_Vessels.DestroyPool();
	m_ModelComponents.DestroyPool();
	m_TransformComponents.DestroyPool();
}

uint64_t CLevel::CreateVessel()
{
	std::random_device rd;
	std::uniform_int_distribution<uint64_t> dist;
	MVessel tempVessel;
	tempVessel.nId = dist(rd);
	tempVessel.nParentId = 0;

	m_Vessels.CreateElement(tempVessel);
	return tempVessel.nId;
}

void CLevel::DestroyVessel(uint64_t vesselId)
{
	for (size_t i = 0; i < m_Vessels.GetActivatedSize(); i++)
	{
		if (m_Vessels.GetElement(i).nId == vesselId)
		{
			m_Vessels.DisableElement(i);
			DestroyAllComponentsFromVessel(vesselId);
			return;
		}
	}
}

void CLevel::DestroyAllComponentsFromVessel(uint64_t vesselId)
{
	for (size_t i = 0; i < m_ModelComponents.GetActivatedSize(); i++)
	{
		if (m_ModelComponents.GetElement(i).nOwnerId == vesselId)
		{
			m_ModelComponents.DisableElement(i);
		}
	}

	for (size_t i = 0; i < m_TransformComponents.GetActivatedSize(); i++)
	{
		if (m_TransformComponents.GetElement(i).nOwnerId == vesselId)
		{
			m_TransformComponents.DisableElement(i);
		}
	}
}

void CLevel::GiveSceneBuffer(IRenderer* pRenderer)
{
	//pRenderer->SubmitSceneBuffer(m_SceneBuffer);
	//m_SceneBuffer.Clear();
}

template<> MModelComponent CLevel::CreateComponent()
{
	std::random_device rd;
	std::uniform_int_distribution<uint64_t> dist;
	MModelComponent temp;
	temp.nId = dist(rd);
	return temp;
}

template<> MTransformComponent CLevel::CreateComponent()
{
	std::random_device rd;
	std::uniform_int_distribution<uint64_t> dist;
	MTransformComponent temp;
	temp.nId = dist(rd);
	return temp;
}

template<> void CLevel::AttachComponentToVessel(uint64_t vesselId, MModelComponent component)
{
	component.nOwnerId = vesselId;
	m_ModelComponents.CreateElement(component);
}

template<> void CLevel::AttachComponentToVessel(uint64_t vesselId, MTransformComponent component)
{
	component.nOwnerId = vesselId;
	m_TransformComponents.CreateElement(component);
}

template<> void CLevel::DestroyComponent<MModelComponent>(uint64_t compId)
{
	for (size_t i = 0; i < m_ModelComponents.GetActivatedSize(); i++)
	{
		if (m_ModelComponents.GetElement(i).nId == compId)
		{
			m_ModelComponents.DisableElement(i);
			return;
		}
	}
}

template<> void CLevel::DestroyComponent<MTransformComponent>(uint64_t compId)
{
	for (size_t i = 0; i < m_TransformComponents.GetActivatedSize(); i++)
	{
		if (m_TransformComponents.GetElement(i).nId == compId)
		{
			m_TransformComponents.DisableElement(i);
			return;
		}
	}
}
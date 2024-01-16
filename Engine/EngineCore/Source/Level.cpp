#include "Level.h"
#include <random>

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
	return true;
}

void CLevel::UpdateLevel(float flDeltaTime)
{
	
}

void CLevel::DestroyLevel()
{
	m_Vessels.DestroyPool();
	m_ModelComponents.DestroyPool();
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
}

template<> MModelComponent CLevel::CreateComponent()
{
	std::random_device rd;
	std::uniform_int_distribution<uint64_t> dist;
	MModelComponent temp;
	temp.nId = dist(rd);
	return temp;
}

template<> void CLevel::AttachComponentToVessel(uint64_t vesselId, MModelComponent component)
{
	component.nOwnerId = vesselId;
	m_ModelComponents.CreateElement(component);
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
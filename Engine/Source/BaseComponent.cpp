#include "BaseComponent.h"
#include <random>

namespace MarkTech
{
	CBaseComponent::CBaseComponent()
		: m_nOwnerId(0)
	{
		std::random_device rd;
		std::uniform_int_distribution<uint64_t> dist;
		m_nId = dist(rd);
	}

	CBaseComponent::CBaseComponent(uint64_t ownerId)
		: m_nOwnerId(ownerId)
	{
		std::random_device rd;
		std::uniform_int_distribution<uint64_t> dist;
		m_nId = dist(rd);
	}

	CBaseComponent::~CBaseComponent()
	{
	}
}
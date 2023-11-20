#include "BaseEntity.h"
#include <random>

namespace MarkTech
{
	CBaseEntity::CBaseEntity()
	{
		std::random_device rd;
		std::uniform_int_distribution<uint64_t> dist;
		m_nId = dist(rd);
	}

	CBaseEntity::~CBaseEntity()
	{
	}
}
#pragma once
#include "Core.h"

class CBaseComponent
{
public:
	CBaseComponent();
	CBaseComponent(uint64_t onwerId);
	~CBaseComponent();

	uint64_t GetId() { return m_nId; }
	uint64_t GetOwnerId() { return m_nOwnerId; }

	void UpdateComponent(float flDeltaTime) {}
protected:
	uint64_t m_nId;
	uint64_t m_nOwnerId;
};
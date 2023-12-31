#pragma once
#include "StringType.h"
#include "ComponentRegistry.h"
#include "VesselRegistry.h"

class CLevel
{
public:
	CLevel();
	~CLevel();

	bool InitLevel();
	void UpdateLevel(float flDeltaTime);
	void DestroyLevel();

	uint64_t CreateVessel();

	template<class T>
	uint64_t AddComponentToVessel(uint64_t id);

private:
	CVesselRegistry m_pVessels;
	CComponentRegistry m_pComponents;
};
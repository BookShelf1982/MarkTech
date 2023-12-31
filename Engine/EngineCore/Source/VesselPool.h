#pragma once
#include "Core.h"
#include "Vessel.h"

class CVesselPool
{
public:
	CVesselPool();
	~CVesselPool();

	void InitComponentPool(size_t nInitialSize);
	void UpdateComponentPool(float flDeltaTime);
	void DestroyComponentPool();

	void CreateVessel();
	void DisableVessel(uint64_t id);
private:
	CVessel* m_pVessels;
	bool m_bInitialized;
	size_t m_nDisabled;
	size_t m_nEnabled;
};

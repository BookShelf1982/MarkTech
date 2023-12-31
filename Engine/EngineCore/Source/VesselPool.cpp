#include "VesselPool.h"

CVesselPool::CVesselPool()
	:m_bInitialized(false), m_nDisabled(0), m_nEnabled(0), m_pVessels(nullptr)
{
}

CVesselPool::~CVesselPool()
{
}

void CVesselPool::InitComponentPool(size_t nInitialSize)
{
}

void CVesselPool::UpdateComponentPool(float flDeltaTime)
{
}

void CVesselPool::DestroyComponentPool()
{
	if (m_bInitialized)
		delete[] m_pVessels;
}

void CVesselPool::CreateVessel()
{
}

void CVesselPool::DisableVessel(uint64_t id)
{
}



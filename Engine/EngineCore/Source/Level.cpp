#include "Level.h"

CLevel::CLevel()
{
}

CLevel::~CLevel()
{
}

bool CLevel::InitLevel()
{
	m_pComponents.InitComponentRegistry();
	return true;
}

void CLevel::UpdateLevel(float flDeltaTime)
{
	m_pComponents.UpdateComponentRegistry(flDeltaTime);
}

void CLevel::DestroyLevel()
{
	m_pComponents.DestroyComponentRegistry();
}

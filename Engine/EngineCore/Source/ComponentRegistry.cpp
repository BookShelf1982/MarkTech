#include "ComponentRegistry.h"

CComponentRegistry::CComponentRegistry()
{
}

CComponentRegistry::~CComponentRegistry()
{
}

void CComponentRegistry::InitComponentRegistry()
{
	m_pBaseComponents.InitComponentPool(MAX_COMPONENTS);
}

void CComponentRegistry::UpdateComponentRegistry(float flDeltaTime)
{
	m_pBaseComponents.UpdateComponentPool(flDeltaTime);
}

void CComponentRegistry::DestroyComponentRegistry()
{
	m_pBaseComponents.DestroyComponentPool();
}

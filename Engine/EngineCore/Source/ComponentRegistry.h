#pragma once
#include "Core.h"
#include "ComponentPool.h"
#include "BaseComponent.h"

#define MAX_COMPONENTS 65536
#define MAX_VESSELS 16384

class CComponentRegistry
{
public:
	CComponentRegistry();
	~CComponentRegistry();

	void InitComponentRegistry();
	void UpdateComponentRegistry(float flDeltaTime);
	void DestroyComponentRegistry();
private:
	CComponentPool<CBaseComponent> m_pBaseComponents;
};
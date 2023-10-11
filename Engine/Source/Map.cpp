#include "Map.h"

CMap::CMap()
{
	
}

CMap::~CMap()
{
	
}

void CMap::SpawnEntity(CBaseEntity* entityClass, MTransform transform)
{
	entityClass->Spawn(transform);
	entities.push_back(entityClass);
}

void CMap::UpdateEntities(float flDeltaTime)
{
	for (int i = 0; i < entities.size(); i++)
	{
		entities[i]->Update(flDeltaTime);
	}
}

void CMap::PrintEntities()
{
	
}

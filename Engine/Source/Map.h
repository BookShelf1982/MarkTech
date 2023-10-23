#pragma once
#include "Core.h"
#include "Vectors.h"
#include "BaseEntity.h"
#include "BasePointEntity.h"

class CMap
{
public:
	CMap();

	~CMap();

	void SpawnEntity(CBaseEntity* entityClass);

	void SpawnEntityWithTransform(CBasePointEntity* entityClass, MTransform transform);

	void UpdateEntities(float flDeltaTime);

	void PrintEntities();

	std::vector<CBaseEntity*> entities;
};

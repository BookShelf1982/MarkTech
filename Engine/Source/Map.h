#pragma once
#include "Core.h"
#include "Vectors.h"
#include "BaseEntity.h"

class CMap
{
public:
	CMap();

	~CMap();

	void SpawnEntity(CBaseEntity* entityClass, MTransform transform);

	void UpdateEntities(float flDeltaTime);

	void PrintEntities();

	std::vector<CBaseEntity*> entities;
};

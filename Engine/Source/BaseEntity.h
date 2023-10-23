#pragma once
#include "Core.h"

class CBaseEntity
{
public:
	CBaseEntity(std::string name = "") :Name(name) {}

	virtual void Spawn();
	virtual void Update(float flDeltaTime);

protected:
	std::string Name;
};


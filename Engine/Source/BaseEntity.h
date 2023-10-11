#pragma once
#include "Core.h"

class CBaseEntity
{
public:
	CBaseEntity();

	MTransform Transform;
	std::string Name;

	virtual void Spawn(MTransform transform);
	virtual void Update(float flDeltaTime);
	void SetEntityPosition(MVector3 pos);
	MVector3 GetEntityPosition() { return Transform.Position; }
};


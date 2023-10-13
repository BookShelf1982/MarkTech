#pragma once
#include "Core.h"
#include "Vectors.h"

class CBaseEntity
{
public:
	CBaseEntity();

	virtual void Spawn(MTransform transform);
	virtual void Update(float flDeltaTime);
	void SetEntityPosition(MVector3 pos);
	MVector3 GetEntityPosition() { return Transform.Position; }

protected:
	MTransform Transform;
	std::string Name;
};


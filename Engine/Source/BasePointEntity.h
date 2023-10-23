#pragma once
#include "Core.h"
#include "Vectors.h"
#include "BaseEntity.h"

class CBasePointEntity : public CBaseEntity
{
public:
	virtual void SpawnWithTransform(MTransform transform);
	virtual void Update(float flDeltaTime) override;

	void SetEntityPosition(MVector3 pos);
	MVector3 GetEntityPosition() { return Transform.Position; }

protected:
	MTransform Transform;
};
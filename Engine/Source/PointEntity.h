#pragma once
#include "Core.h"
#include "BaseEntity.h"

class CPointEntity : public CBaseEntity
{
public:

	virtual void Spawn(MTransform transform) override;
	virtual void Update(float flDeltaTime) override;

	void PrintPos();
};
#pragma once
#include "Core.h"
#include "MModel.h"
#include "BasePointEntity.h"

class CModelEntity : public CBasePointEntity
{
public:
	CModelEntity(std::string name = "")
	{
		Name = name;
	}

	virtual void SpawnWithTransform(MTransform transform) override;
	virtual void Update(float flDeltaTime) override;

protected:
	CMModel model;
};
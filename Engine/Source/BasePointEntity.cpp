#include "BasePointEntity.h"

void CBasePointEntity::SpawnWithTransform(MTransform transform)
{
	this->Transform = transform;
}

void CBasePointEntity::Update(float flDeltaTime)
{
}

void CBasePointEntity::SetEntityPosition(MVector3 pos)
{
	this->Transform.Position.x = pos.x;
	this->Transform.Position.y = pos.y;
	this->Transform.Position.z = pos.z;
}

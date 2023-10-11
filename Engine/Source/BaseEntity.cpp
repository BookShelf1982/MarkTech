#include "BaseEntity.h"

CBaseEntity::CBaseEntity()
{

}

void CBaseEntity::Spawn(MTransform transform)
{
	this->Transform = transform;
}

void CBaseEntity::Update(float flDeltaTime)
{

}

void CBaseEntity::SetEntityPosition(MVector3 pos)
{
	this->Transform.Position.x = pos.x;
	this->Transform.Position.y = pos.y;
	this->Transform.Position.z = pos.z;
}

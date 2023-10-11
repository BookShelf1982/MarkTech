#include "PointEntity.h"

void CPointEntity::Spawn(MTransform transform)
{
	this->Transform = transform;
}

void CPointEntity::Update(float flDeltaTime)
{
	SetEntityPosition(MVector3(GetEntityPosition().x + 10 * flDeltaTime, GetEntityPosition().y, GetEntityPosition().z));

	PrintPos();
}

void CPointEntity::PrintPos()
{
	std::wstring wstr;
	std::string str;
	str = Transform.Position.ToString();
	wstr = std::wstring(str.begin(), str.end());
	OutputDebugString(wstr.c_str());
}



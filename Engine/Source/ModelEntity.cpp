#include "ModelEntity.h"
#include <Windows.h>

void CModelEntity::SpawnWithTransform(MTransform transform)
{
	CBasePointEntity::SpawnWithTransform(transform);
}

void CModelEntity::Update(float flDeltaTime)
{
	std::string dstr;
	dstr = GetEntityPosition().ToString();
	std::wstring wstr(dstr.begin(), dstr.end());
	OutputDebugString(wstr.c_str());
}

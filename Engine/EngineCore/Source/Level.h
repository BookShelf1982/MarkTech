#pragma once
#include "StringType.h"
#include "ComponentRegistry.h"

class CLevel
{
public:
	CLevel();
	~CLevel();

	bool InitLevel();
	void UpdateLevel(float flDeltaTime);
	void DestroyLevel();
private:
	CComponentRegistry m_pComponents;
};
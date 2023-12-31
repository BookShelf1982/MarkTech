#pragma once
#include "Core.h"

#define MAX_VESSELS 16384

class CVesselRegistry
{
public:
	CVesselRegistry();
	~CVesselRegistry();

	void InitComponentRegistry();
	void UpdateComponentRegistry(float flDeltaTime);
	void DestroyComponentRegistry();
private:

};
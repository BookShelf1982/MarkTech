#pragma once
#include "Core.h"

class IMarkTechEngine
{
public:
	IMarkTechEngine() {}
	~IMarkTechEngine() {}
	virtual void Release() = 0;
};

IMarkTechEngine* CreateMarkTechEngine(const char** ppStrings, uint32_t nStringCount);
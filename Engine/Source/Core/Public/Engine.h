#pragma once
#include "Core.h"

class IMarkTechEngine
{
public:
	IMarkTechEngine() {}
	~IMarkTechEngine() {}
	virtual void Update() = 0;
	virtual void Release() = 0;
	virtual bool WantToQuit() = 0;
};

IMarkTechEngine* CreateMarkTechEngine(const std::vector<std::string>& modules);
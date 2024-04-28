#pragma once
#include "Core.h"

struct MEngineData
{
	CTList<CString> modules;
	CString defaultMap;
	CString gameName;
};

class IMarkTechEngine
{
public:
	IMarkTechEngine() {}
	~IMarkTechEngine() {}
	virtual void Update() = 0;
	virtual void Release() = 0;
	virtual bool WantToQuit() = 0;
};

IMarkTechEngine* CreateMarkTechEngine(const MEngineData& data);
#pragma once
#include "Engine.h"
#include "Module.h"
#ifdef MT_PLATFORM_WINDOWS
#include "WinModule.h"
#endif

#include <vector>


class CEngine : public IMarkTechEngine
{
public:
	void Release() override;

	static CEngine* CreateEngine(const char** ppRequiredModules, uint32_t nModulesCount);

private:
	CEngine(const char** ppStrings, uint32_t nStringCount);
	~CEngine();

	static void DestroyEngine();

	static CEngine* m_pIstance;

	std::vector<IModule*> m_LoadedModules;
};
#pragma once
#include "ModulesShared.h"
#include "Engine.h"
#include <Windows.h>

class CEngineCoreModule : public IModuleInterface
{
public:
	CEngineCoreModule();
	~CEngineCoreModule();

	virtual void PreInit( HINSTANCE hInstance, pfnStopGameLoop pfn ) override;
	virtual void Init() override;
	virtual void Update() override;
	virtual void Shutdown() override;

	static CEngineCoreModule* g_pEngineCoreModule;
private:
	CEngine* m_pEngine;
};
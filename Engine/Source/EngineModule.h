#pragma once
#include "Core.h"
#include "ModulesShared.h"
#include "Engine.h"

extern "C"
{
}


class CEngineModule : public IModuleInterface
{
public:
	CEngineModule();
	~CEngineModule();

	virtual void PreInit(HINSTANCE hInstance) override;
	virtual void Init() override;
	virtual void Update() override;
	virtual void Shutdown() override;

	static CEngineModule* g_pEngineModule;
private:
	MarkTech::CEngine* m_pEngine;
};
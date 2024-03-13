#pragma once
#include "Module.h"
#include <Windows.h>

class CWinModule : public IModule
{
public: 
	CWinModule(const char* pModName);
	~CWinModule();

	virtual void* GetModuleProcAddress(const char* pName) override;
private:
	HMODULE m_Module;
};

class CWinModuleLoader : public IModuleLoader
{
public:
	virtual IModule* LoadModule(const char* pModName) override;
};
#pragma once
#include "Core.h"

class IModule
{
public:
	IModule() {}
	virtual ~IModule() {}

	virtual void* GetModuleProcAddress(const char* pName) = 0;
};

class IModuleLoader
{
public:
	virtual IModule* LoadModule(const char* pModName) = 0;
};
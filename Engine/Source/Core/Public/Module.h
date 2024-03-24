#pragma once
#include "Core.h"

typedef void* (*pfn_void_func_t)();

class IModule
{
public:
	IModule() {}
	virtual ~IModule() {}

	pfn_void_func_t pfnInit;
	pfn_void_func_t pfnUpdate;
	pfn_void_func_t pfnShutdown;

	virtual void* GetModuleProcAddress(const char* pName) = 0;
};

class IModuleLoader
{
public:
	virtual IModule* LoadModule(const char* pModName) = 0;
};
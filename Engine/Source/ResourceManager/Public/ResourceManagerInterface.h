#pragma once
#include "ResourceManager.h"

typedef IResourceManager* (*PFN_CreateResourceManager)();

struct resource_manager_funcs_t
{
	PFN_CreateResourceManager pfnCreateResourceManager;
};

extern "C"
{
	DLLEXPORT IResourceManager* CreateResourceManager();
	DLLEXPORT void InitModule();
	DLLEXPORT void UpdateModule();
	DLLEXPORT void ShutdownModule();
}
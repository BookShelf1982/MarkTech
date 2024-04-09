#pragma once
#include "Core.h"

class IResourceManager
{
public:
	IResourceManager() {}
	virtual ~IResourceManager() {}

	virtual void* ReadShaderFromFile(const char* pFilepath) = 0;
};
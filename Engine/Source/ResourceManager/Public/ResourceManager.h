#pragma once
#include "Core.h"
#include "ShaderResource.h"

class IResourceManager
{
public:
	IResourceManager() {}
	virtual ~IResourceManager() {}

	virtual IShaderResource* ReadShaderFromFile(const char* pFilepath) = 0;
};
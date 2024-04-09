#pragma once
#include "ResourceManager.h"

class CResourceManager : public IResourceManager
{
public:
	CResourceManager();
	~CResourceManager();

	virtual void* ReadShaderFromFile(const char* pFilepath) override;
};
#pragma once
#include "ResourceManager.h"
#include <vector>

class CResourceManager : public IResourceManager
{
public:
	CResourceManager();
	~CResourceManager();

	virtual IShaderResource* ReadShaderFromFile(const char* pFilepath) override;
private:
};
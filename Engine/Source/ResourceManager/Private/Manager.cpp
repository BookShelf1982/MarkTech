#include "Manager.h"
#include "ShaderResourceImpl.h"
#include <fstream>

CResourceManager::CResourceManager()
{
}

CResourceManager::~CResourceManager()
{
}

IShaderResource* CResourceManager::ReadShaderFromFile(const char* pFilepath)
{
	std::fstream file(pFilepath, std::ios::in | std::ios::binary);
	if (!file.is_open())
		return nullptr;

	file.seekg(0, std::ios::end);
	size_t fileSize = file.tellg();
	file.seekg(0);
	uint32_t* pData = new uint32_t[fileSize];
	file.read((char*)pData, fileSize);
	file.close();

	return new CShaderResource(pData, (uint32_t)fileSize);
}

#include "Manager.h"
#include <fstream>

CResourceManager::CResourceManager()
{
}

CResourceManager::~CResourceManager()
{
}

void* CResourceManager::ReadShaderFromFile(const char* pFilepath)
{
	std::fstream file(pFilepath, std::ios::in | std::ios::binary);
	if (!file.is_open())
		return nullptr;

	file.seekg(0, std::ios::end);
	size_t fileSize = file.tellg();
	file.seekg(0);
	uint32_t* pData = new uint32_t[fileSize];
	file.read((char*)pData, fileSize);

	return pData;
}

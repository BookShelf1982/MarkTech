#include "resource_manager.h"
#include "alloc_uitls.h"
#include "djb2.h"

#include <FileSystem.h>

uint32_t g_extensionTable[16] = {
	{0}, {0}, {0}, {RESOURCE_TYPE_TEXTURE}, {0}, {0}, {0}, // 0 - 6
	{RESOURCE_TYPE_SHADER_BINARY}, {0}, {0}, // 7 - 9
	{0}, {0}, {RESOURCE_TYPE_SHADER}, {0}, {0}, {0}  // 10 - 15
};

uint8_t LoadResource(ResourceManager& manager, const char* path, uint32_t* id)
{
	if (!FSPathExists((Path*)path))
		return 0;

	uint32_t extHash = djb2(FSFindExtension((Path*)path));

	File file;
	FSOpen((Path*)path, OPEN_TYPE_READ, &file);
	if (!file._handle)
		return 0;

	char* data = (char*)AlignedAlloc(file.size, 4);
	FSRead(&file, data, file.size);
	FSClose(&file);

	Resource res = {};
	res.data = data;
	res.dataSize = file.size;
	res.type = (ResourceType)g_extensionTable[extHash % (sizeof(g_extensionTable) / sizeof(g_extensionTable[0]))];
	
	*id = (uint32_t)manager.resources.count;
	manager.resources[manager.resources.count] = res;

	if (!manager.postLoadFns[res.type].fn(*id, path, &manager.resources[manager.resources.count], manager.postLoadFns->user))
	{
		AlignedFree(data);
		return 0;
	}
	
	manager.resources.count++;

	return 1;
}

uint8_t GetResource(ResourceManager& manager, uint32_t id, Resource** resource)
{
	return 1;
}

void FreeResources(ResourceManager& manager)
{
	for (size_t i = 0; i < manager.resources.count; i++)
	{
		AlignedFree(manager.resources[i].data);
	}
}

#pragma once

#include "data_structures.h"
#include <stdint.h>

#define MAX_RESOURCES 64

enum ResourceType : uint32_t
{
	RESOURCE_TYPE_SHADER,
	RESOURCE_TYPE_SHADER_BINARY,
	RESOURCE_TYPE_TEXTURE
};

struct Resource 
{
	char* data;
	size_t dataSize;
	ResourceType type;
};

typedef bool (*PFN_PostLoadResource)(uint32_t id, const char* filepath, const Resource* resource, void* user);

struct PostLoadFn
{
	void* user;
	PFN_PostLoadResource fn;
};

struct ResourceManager 
{
	FixedArray<Resource, MAX_RESOURCES> resources;
	PostLoadFn postLoadFns[3];
};

uint8_t LoadResource(ResourceManager& manager, const char* path, uint32_t* id);
uint8_t GetResource(ResourceManager& manager, uint32_t id, Resource** resource);
void FreeResources(ResourceManager& manager);
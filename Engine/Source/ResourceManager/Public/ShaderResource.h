#pragma once
#include "Core.h"

class IShaderResource
{
public:
	IShaderResource() {}
	virtual ~IShaderResource() {}
	virtual uint32_t* GetPtr() = 0;
	virtual uint32_t GetSizeInBytes() = 0;
};
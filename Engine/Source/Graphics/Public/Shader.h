#pragma once
#include "Core.h"

enum class EShaderStage
{
	VERTEX,
	FRAGMENT
};

struct MCreateShaderInfo
{
	EShaderStage stage;
	void* pShaderData;
	uint32_t nShaderDataSize;
	const char* pEntrypoint;
};

class IShader
{
public:
	IShader() {}
	IShader(MCreateShaderInfo info) {}
	virtual ~IShader() {}
};
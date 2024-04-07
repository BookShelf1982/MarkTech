#pragma once
#include "Core.h"

enum class EBindingTarget
{
	VERTEX_ARRAY,
	INDEX_ARRAY,
	UNIFORM_BUFFER
};

enum class EBufferUsage
{
	STATIC,
	DYNAMIC
};

struct MCreateBufferInfo
{
	EBindingTarget target;
	uint32_t nSize;
	void* pData;
	EBufferUsage usage;
};

class IBuffer
{
public:
	IBuffer() {}
	IBuffer(MCreateBufferInfo info) {}
	virtual ~IBuffer() {}
};
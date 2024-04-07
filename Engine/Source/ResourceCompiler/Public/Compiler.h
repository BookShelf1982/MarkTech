#pragma once
#include "Core.h"

struct MCompiledShader
{
	void* pData;
	uint32_t nDataSize;
};

class ICompiler
{
public:
	ICompiler() {}
	virtual ~ICompiler() {}
	virtual MCompiledShader CompileShader(const char* pSource, uint32_t nType, const char* pFilename) = 0;
	virtual void ReleaseCompiledShader(MCompiledShader& shader) = 0;
};
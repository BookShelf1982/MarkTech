#pragma once
#include "Compiler.h"
#include <shaderc\shaderc.hpp>

class CResourceCompiler : public ICompiler
{
public:
	CResourceCompiler();
	~CResourceCompiler();
	virtual MCompiledShader CompileShader(const char* pSource, uint32_t nType, const char* pFilename) override;
	virtual void ReleaseCompiledShader(MCompiledShader& shader) override;
private:

};